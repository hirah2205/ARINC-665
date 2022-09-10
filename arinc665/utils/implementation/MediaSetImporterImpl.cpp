/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetImporterImpl.
 **/

#include "MediaSetImporterImpl.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc645/Arinc645Crc.hpp>
#include <arinc645/CheckValueGenerator.hpp>

namespace Arinc665::Utils {

MediaSetImporter& MediaSetImporterImpl::fileSizeHandler(
  FileSizeHandler fileSizeHandler )
{
  fileSizeHandlerV = std::move( fileSizeHandler );
  return *this;
}

MediaSetImporter& MediaSetImporterImpl::readFileHandler(
  ReadFileHandler readFileHandler )
{
  readFileHandlerV = std::move( readFileHandler );
  return *this;
}

MediaSetImporter& MediaSetImporterImpl::checkFileIntegrity(
  const bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
  return *this;
}

MediaSetImporterImpl::Result MediaSetImporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>();

  // 1st Medium
  loadFirstMedium();

  // load further media
  loadFurtherMedia();

  // finally, add all files (regular, load headers, batches) to the media set
  addFiles();

  return { std::move( mediaSet ), std::move( checkValues ) };
}

void MediaSetImporterImpl::loadFirstMedium()
{
  // Load "list of files" file
  fileListFile = readFileHandlerV( 1U, Arinc665::ListOfFilesName );

  bool listOfLoadsFilePresent{ false };

  for ( const auto &fileInfo : fileListFile.files() )
  {
    // get file type
    // skip list files and handle load headers and batch files separate
    switch ( Arinc665::Files::Arinc665File::fileType( fileInfo.filename ) )
    {
      case Arinc665::FileType::FileList:
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Load List not in FILES.LUM" } );

      case Arinc665::FileType::LoadList:
        // check that List of Loads is in Root Directory
        if ( fileInfo.pathName != "\\" )
        {
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "LOADS.LUM not in Root Directory" } );
        }

        listOfLoadsFilePresent = true;

        // store load list file check value
        mediaSet->listOfLoadsCheckValueType( std::get< 0 >( fileInfo.checkValue ) );

        // List of Loads not added to file information
        continue ;

      case Arinc665::FileType::BatchList:
        // check that List of Loads is in Root Directory
        if ( fileInfo.pathName != "\\" )
        {
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "LOADS.LUM not in Root Directory" } );
        }

        // set bath list file present indicator
        batchListFilePresent = true;

        // store batch list file check value
        mediaSet->listOfBatchesCheckValueType( std::get< 0 >( fileInfo.checkValue ) );

        // List of Batches not added to file information
        continue ;

      default:
        break;
    }

    // update files information
    filesInfos.try_emplace( fileInfo.filename, fileInfo );
  }

  // Load List File

  // Check Existence of Load List File within Files Information
  if ( !listOfLoadsFilePresent )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Load List not in FILES.LUM" } );
  }

  // check file integrity on current medium
  checkMediumFiles( fileListFile.files(), 1U );

  // store list of files user defined data
  mediaSet->filesUserDefinedData( fileListFile.userDefinedData() );
  // store file list file check value
  mediaSet->listOfFilesCheckValueType( fileListFile.checkValueType() );
  // Set Media Set Parameter
  mediaSet->partNumber( fileListFile.mediaSetPn());
  mediaSet->numberOfMedia( fileListFile.numberOfMediaSetMembers() );


  // Load list of loads file
  loadListFile = readFileHandlerV( 1, Arinc665::ListOfLoadsName );

  for ( const auto &load : loadListFile.loads() )
  {
    // check existence of load header file
    const auto fileIt{ filesInfos.find( load.headerFilename ) };

    if ( fileIt == filesInfos.end() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "load header file not found" }
        << boost::errinfo_file_name{ load.headerFilename } );
    }

    // checks that the load list and file list entry maps to the same file entry
    if ( load != fileIt->second )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "data inconsistency" }
        << boost::errinfo_file_name{ load.headerFilename } );
    }

    // add load to "loads information"
    loadsInfos.try_emplace( load.headerFilename, load );
  }

  // store list of loads user defined data
  mediaSet->loadsUserDefinedData( loadListFile.userDefinedData() );

  // Load list of batches file
  if ( batchListFilePresent )
  {
    batchListFile = readFileHandlerV( 1, Arinc665::ListOfBatchesName );

    for ( const auto &batch : batchListFile.batches() )
    {
      // check existence of load header file
      const auto fileIt{ filesInfos.find( batch.filename ) };

      if ( fileIt == filesInfos.end() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "batch file not found" }
          << boost::errinfo_file_name{ batch.filename } );
      }

      // checks that the load list and file list entry maps to the same file entry
      if ( batch != fileIt->second )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "data inconsistency" }
          << boost::errinfo_file_name{ batch.filename } );
      }

      // add batch to "batches information"
      batchesInfos.try_emplace( batch.filename, batch );
    }

    // store list of batches user defined data
    mediaSet->batchesUserDefinedData( batchListFile.userDefinedData() );
  }
}

void MediaSetImporterImpl::loadFurtherMedia() const
{
  // iterate over media
  for (
    uint8_t mediumNumber = 2U;
    mediumNumber <= mediaSet->numberOfMedia();
    ++mediumNumber )
  {
    // Load "list of files" file
    const Files::FileListFile mediumFileListFile{
      readFileHandlerV( mediumNumber, Arinc665::ListOfFilesName ) };

    // compare current list of files to first one
    if ( !mediumFileListFile.belongsToSameMediaSet( fileListFile )
      || ( mediumNumber != mediumFileListFile.mediaSequenceNumber() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "inconsistent file list file" }
        << boost::errinfo_file_name{ std::string{ Arinc665::ListOfFilesName } } );
    }

    // check file integrity on current medium
    checkMediumFiles( mediumFileListFile.files(), mediumNumber );

    // Load "List of Loads" file

    // check against stored version
    if (
      const Files::LoadListFile mediumLoadListFile{
        readFileHandlerV( mediumNumber, Arinc665::ListOfLoadsName ) };
      !mediumLoadListFile.belongsToSameMediaSet( loadListFile )
        || ( mediumNumber != mediumLoadListFile.mediaSequenceNumber() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ std::string{ Arinc665::ListOfLoadsName }
          + " is not consistent to other loads list" } );
    }

    // Load "List of Batches" file
    if ( batchListFilePresent )
    {
      // check against stored version
      if (
        const Files::BatchListFile mediumBatchListFile{
          readFileHandlerV( mediumNumber, Arinc665::ListOfBatchesName ) };
        !mediumBatchListFile.belongsToSameMediaSet( batchListFile )
        || ( mediumNumber != mediumBatchListFile.mediaSequenceNumber() ) )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception() << Helper::AdditionalInfo{
            std::string{ Arinc665::ListOfBatchesName }
            + " is not consistent to other batches list" } );
      }
    }
  }
}

void MediaSetImporterImpl::addFiles()
{
  // iterate over all files
  for ( const auto &[ fileName, fileInfo ] : filesInfos )
  {
    if ( loadsInfos.contains( fileName ) || batchesInfos.contains( fileName ) )
    {
      // skip load header and batch files
      continue;
    }

    // get directory, where file will be placed into.
    const auto container{ checkCreateDirectory(
      fileInfo.memberSequenceNumber,
      fileInfo.path().parent_path() ) };

    // place file
    const auto filePtr{ container->addRegularFile( fileInfo.filename ) };
    assert( filePtr );

    // set check value indicator
    filePtr->checkValueType( std::get< 0 >( fileInfo.checkValue ) );

    // update check values
    checkValues.try_emplace( filePtr, fileInfo.checkValue );
  }

  addLoads();
  addBatches();
}

void MediaSetImporterImpl::addLoads()
{
  BOOST_LOG_FUNCTION()

  // iterate over load headers
  for ( const auto &[ filename, loadInfo ] : loadsInfos )
  {
    addLoad( loadInfo );
  }
}

void MediaSetImporterImpl::addLoad( const Files::LoadInfo &loadInfo )
{
  BOOST_LOG_FUNCTION()

  // obtain file information for load header
  const auto &fileInfo { fileInformation( loadInfo.headerFilename ) };

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Header File " << fileInfo.path().generic_string();

  // decode load header
  const auto rawLoadHeaderFile{
    readFileHandlerV( fileInfo.memberSequenceNumber, fileInfo.path() ) };
  const Files::LoadHeaderFile loadHeaderFile{ rawLoadHeaderFile };

  // validate load part number to load information
  if ( loadInfo.partNumber != loadHeaderFile.partNumber() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Load part number inconsistent" }
      << boost::errinfo_file_name{ std::string{ loadInfo.headerFilename } } );
  }

  // validate THW IDs of load header against list of loads
  if ( std::multiset< std::string >{
      loadInfo.targetHardwareIds.begin(),
      loadInfo.targetHardwareIds.end() }
    != std::multiset< std::string >{
      loadHeaderFile.targetHardwareIds().begin(),
      loadHeaderFile.targetHardwareIds().end() } )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Load THW IDs inconsistent" }
      << boost::errinfo_file_name{ std::string{ loadInfo.headerFilename } } );
  }

  // obtain container (directory, medium), which will contain the load.
  const auto container{ checkCreateDirectory(
    fileInfo.memberSequenceNumber,
    fileInfo.path().parent_path() ) };

  // create load
  const auto loadPtr{ container->addLoad( loadInfo.headerFilename ) };
  assert( loadPtr );

  // set check value indicator
  loadPtr->checkValueType( std::get< 0 >( fileInfo.checkValue ) );

  loadPtr->partFlags( loadHeaderFile.partFlags() );
  loadPtr->partNumber( loadHeaderFile.partNumber() );
  loadPtr->loadType( loadHeaderFile.loadType() );
  Media::Load::TargetHardwareIdPositions thwIdsPositions{};
  for ( const auto &[ thwId, positions ] : loadHeaderFile.targetHardwareIdsPositions() )
  {
    thwIdsPositions.try_emplace( thwId, positions.begin(), positions.end() );
  }
  for ( const auto &thwId : loadHeaderFile.targetHardwareIds() )
  {
    // if not previously added - add it now with empty positions
    thwIdsPositions.try_emplace( thwId );
  }
  loadPtr->targetHardwareIdPositions( std::move( thwIdsPositions ) );

  // Load Check CRC and Load Check Value
  Arinc645::Arinc645Crc32 loadCrc{};
  auto loadCheckValueGenerator{
    Arinc645::CheckValueGenerator::create( loadHeaderFile.loadCheckValueType() ) };
  assert( loadCheckValueGenerator );

  if ( checkFileIntegrityV )
  {
    Files::LoadHeaderFile::processLoadCrc( rawLoadHeaderFile, loadCrc );
    Files::LoadHeaderFile::processLoadCheckValue( rawLoadHeaderFile, *loadCheckValueGenerator );
  }

  // iterate over data files
  for ( const auto &dataFile : loadHeaderFile.dataFiles() )
  {
    const auto dataFiles{ mediaSet->regularFiles( dataFile.filename ) };

    if ( dataFiles.empty() )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
          << Helper::AdditionalInfo{ "Data file not found" }
          << boost::errinfo_file_name{ dataFile.filename } );
    }

    if ( dataFiles.size() > 1U )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Data file not unique" }
        << boost::errinfo_file_name{ dataFile.filename } );
    }

    // perform file check
    // in ARINC 665-2 File Size of Data File is stored as multiple of 16 bit
    checkLoadFile(
      loadCrc,
      *loadCheckValueGenerator,
      dataFile,
      loadHeaderFile.arincVersion() == SupportedArinc665Version::Supplement2 );

    loadPtr->dataFile(
      dataFiles.front(),
      dataFile.partNumber,
      std::get< 0 >( dataFile.checkValue ) );
  }

  // iterate over support files
  for ( const auto &supportFile : loadHeaderFile.supportFiles() )
  {
    auto supportFiles{ mediaSet->regularFiles( supportFile.filename ) };

    if ( supportFiles.empty() )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Support file not found" }
        << boost::errinfo_file_name{ supportFile.filename } );
    }

    if ( supportFiles.size() > 1U )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Support file not unique" }
        << boost::errinfo_file_name{ supportFile.filename } );
    }

    checkLoadFile( loadCrc, *loadCheckValueGenerator, supportFile, false );

    loadPtr->supportFile(
      supportFiles.front(),
      supportFile.partNumber,
      std::get< 0 >( supportFile.checkValue ) );
  }

  // Check Load CRC and Load Check Value
  if ( checkFileIntegrityV )
  {
    if ( Files::LoadHeaderFile::decodeLoadCrc( rawLoadHeaderFile ) != loadCrc.checksum() )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Load CRC inconsistent" }
        << boost::errinfo_file_name{ fileInfo.filename } );
    }

    if ( Files::LoadHeaderFile::decodeLoadCheckValue( rawLoadHeaderFile ) != loadCheckValueGenerator->checkValue() )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Load Check Value inconsistent" }
        << boost::errinfo_file_name{ fileInfo.filename } );
    }
  }

  // User Defined Data
  loadPtr->userDefinedData( loadHeaderFile.userDefinedData() );
  // Load Check Value
  loadPtr->loadCheckValueType( loadHeaderFile.loadCheckValueType() );

  // update check values
  checkValues.try_emplace( loadPtr, fileInfo.checkValue );
}

void MediaSetImporterImpl::addBatches()
{
  BOOST_LOG_FUNCTION()

  // iterate over batches
  for ( const auto &[ filename, batchInfo ] : batchesInfos )
  {
    addBatch( batchInfo );
  }
}

void MediaSetImporterImpl::addBatch( const Files::BatchInfo &batchInfo )
{
  BOOST_LOG_FUNCTION()

  const auto &fileInfo{ fileInformation( batchInfo.filename ) };

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Batch File " << fileInfo.path().generic_string();

  // Decode batch File
  Files::BatchFile batchFile{ readFileHandlerV(
    fileInfo.memberSequenceNumber,
    fileInfo.path() ) };

  // validate batch part number to batch information
  if ( batchInfo.partNumber != batchFile.partNumber() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Batch part number inconsistent" }
      << boost::errinfo_file_name{ std::string{ batchInfo.filename } } );
  }

  // obtain container (directory, medium), which will contain the batch.
  const auto container{ checkCreateDirectory(
    fileInfo.memberSequenceNumber,
    fileInfo.path().parent_path() ) };

  // create batch
  const auto batchPtr{ container->addBatch( batchInfo.filename ) };
  assert( batchPtr );

  // set check value indicator
  batchPtr->checkValueType( std::get< 0 >( fileInfo.checkValue  ) );

  batchPtr->partNumber( batchFile.partNumber() );
  batchPtr->comment( batchFile.comment() );

  // iterate over target hardware
  for ( const auto &targetHardware : batchFile.targetsHardware() )
  {
    Media::ConstLoads batchLoads{};

    // iterate over loads
    for ( const auto& load : targetHardware.loads )
    {
      const auto loads{ mediaSet->loads( load.headerFilename ) };

      if ( loads.empty() )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Load not found" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      if ( loads.size() > 1U )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Load not unique" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      // check that Part Number Information matches
      if ( loads.front()->partNumber() != load.partNumber )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Load part Number does not match Batch Info" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      batchLoads.emplace_back( loads.front() );
    }

    // add Target Hardware/ Position
    batchPtr->target( targetHardware.targetHardwareIdPosition, batchLoads );
  }

  // update check values
  checkValues.try_emplace( batchPtr, fileInfo.checkValue );
}

const Files::FileInfo& MediaSetImporterImpl::fileInformation(
  std::string_view filename ) const
{
  const auto fileInfoIt{ filesInfos.find( filename ) };

  if ( filesInfos.end() == fileInfoIt )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  return fileInfoIt->second;
}

Media::ContainerEntityPtr MediaSetImporterImpl::checkCreateDirectory(
  const uint8_t mediumNumber,
  const std::filesystem::path &directoryPath )
{
  // make path relative (remove leading slash)
  const auto dirPath{ directoryPath.relative_path() };

  // we are in root-directory
  if ( dirPath.empty() )
  {
    return mediaSet->medium( mediumNumber );
  }

  Media::ContainerEntityPtr dir{ mediaSet->medium( mediumNumber ) };

  // iterate over path elements
  for ( auto &subPath : dirPath )
  {
    auto subDir{ dir->subdirectory( subPath.string() ) };

    // if subdirectory does not exist - create it
    if ( !subDir )
    {
      subDir = dir->addSubdirectory( subPath.string() );
      assert( subDir );
    }

    dir = subDir;
  }

  return dir;
}

void MediaSetImporterImpl::checkMediumFiles(
  const Files::FilesInfo &filesInfo,
  const uint8_t mediumNumber ) const
{
  // iterate over files
  for ( const auto &fileInfo : filesInfo )
  {
    // skip files, which are not part of the current medium
    if ( fileInfo.memberSequenceNumber != mediumNumber )
    {
      continue;
    }

    // skip file integrity checks if requested
    if ( !checkFileIntegrityV )
    {
      continue;
    }

    checkFileIntegrity( fileInfo );
  }
}

void MediaSetImporterImpl::checkFileIntegrity(
  const Files::FileInfo &fileInfo ) const
{
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Check file " << fileInfo.path().generic_string();

  const auto rawFile{
    readFileHandlerV(
      fileInfo.memberSequenceNumber,
      fileInfo.path() ) };

  // compare checksums
  if (
    const auto crc{ Files::Arinc665File::calculateChecksum( rawFile ) };
    crc != fileInfo.crc )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "CRC of file invalid" }
      << boost::errinfo_file_name{ fileInfo.path().string() } );
  }

  // Check and compare Check Value
  if (
    const auto &[ checkValueType, checkValue ]{ fileInfo.checkValue };
    Arinc645::CheckValueType::NotUsed != checkValueType )
  {
    const auto checkValueCalculated{ Arinc645::CheckValueGenerator::checkValue(
      checkValueType,
      rawFile ) };

    if ( fileInfo.checkValue != checkValueCalculated )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Check Value of file invalid" }
        << boost::errinfo_file_name{ fileInfo.path().string() } );
    }
  }
}

void MediaSetImporterImpl::checkLoadFile(
  Arinc645::Arinc645Crc32 &loadCrc,
  Arinc645::CheckValueGenerator &loadCheckValueGenerator,
  const Files::LoadFileInfo &loadFile,
  bool fileSize16Bit ) const
{
  const auto &fileInfo{ fileInformation( loadFile.filename ) };

  // get memorised file size ( only when file integrity is checked)
  if ( checkFileIntegrityV )
  {
    const auto fileSize{
      fileSizeHandlerV( fileInfo.memberSequenceNumber, fileInfo.path() ) };

    // check load data file size - we divide by 2 to work around 16-bit size
    // storage within Supplement 2 LUHs (Only Data Files)
    if ( ( fileSize16Bit && ( fileSize / 2 != loadFile.length / 2 ) )
      || ( !fileSize16Bit && ( fileSize != loadFile.length ) ) )
    {
      BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
        << "Load File Size inconsistent "
        << loadFile.filename << " "
        << fileSize << " "
        << loadFile.length;

      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Load File Size inconsistent" }
        << boost::errinfo_file_name{ loadFile.filename } );
    }
  }

  // Check CRC
  if ( fileInfo.crc != loadFile.crc )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Load File CRC inconsistent" }
      << boost::errinfo_file_name{ loadFile.filename } );
  }

  // Check File Check Value
  const bool fileCheckValueChecked{
    checkCheckValues( fileInfo.checkValue, loadFile.checkValue ) };

  // Load CRC, Load Check Value and File Check Value Check
  if ( checkFileIntegrityV )
  {
    const auto rawDataFile{
      readFileHandlerV( fileInfo.memberSequenceNumber, fileInfo.path() ) };

    loadCrc.process_bytes( std::data( rawDataFile ), rawDataFile.size() );
    loadCheckValueGenerator.process( rawDataFile );

    // Load file Check Value
    if ( !fileCheckValueChecked
      && Arinc645::CheckValueGenerator::checkValue(
        std::get< 0 >( loadFile.checkValue ),
        rawDataFile ) != loadFile.checkValue )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
          << Helper::AdditionalInfo{ "Load File Check Value inconsistent" }
          << boost::errinfo_file_name{ loadFile.filename } );
    }
  }
}

bool MediaSetImporterImpl::checkCheckValues(
  const Arinc645::CheckValue &fileListCheckValue,
  const Arinc645::CheckValue &loadFileCheckValue ) const
{
  if ( std::get< 0 >( loadFileCheckValue ) == Arinc645::CheckValueType::NotUsed )
  {
    // no load file check value provided
    return true;
  }

  if ( std::get< 0 >( fileListCheckValue ) == std::get< 0 >( loadFileCheckValue ) )
  {
    if ( std::get< 1 >( fileListCheckValue ) != std::get< 1 >( loadFileCheckValue ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Load File Check Value inconsistent" } );
    }

    return true;
  }

  return false;
}

}

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

Media::MediaSetPtr MediaSetImporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>();

  // iterate over media
  for (
    uint8_t mediumIndex = 1U;
    !loadMedium( mediumIndex );
    ++mediumIndex )
  {
    // noting to do here
  }

  // Set Media Set Parameter
  mediaSet->partNumber( fileListFile->mediaSetPn());
  mediaSet->numberOfMedia( fileListFile->numberOfMediaSetMembers() );

  // finally, add all files (regular, load headers, batches) to the media set
  addFiles();

  return mediaSet;
}

bool MediaSetImporterImpl::loadMedium( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Medium " << static_cast< unsigned int >( mediumIndex );

  loadFileListFile( mediumIndex );
  loadLoadListFile( mediumIndex );
  loadBatchListFile( mediumIndex );
  loadLoadHeaderFiles( mediumIndex );
  loadBatchFiles( mediumIndex );

  // if current medium Index equals number of media set members -> read finished
  return mediumIndex == fileListFile->numberOfMediaSetMembers();
}

void MediaSetImporterImpl::loadFileListFile( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load File List File " << Arinc665::ListOfFilesName;

  // Load "list of files" file
  Files::FileListFile currentFileListFile{
    readFileHandlerV( mediumIndex, Arinc665::ListOfFilesName ) };

  // store first list of files for further tests
  if ( 1U == mediumIndex )
  {
    fileListFile = currentFileListFile;
    fileInfos.clear();
    for ( const auto &fileInfo : currentFileListFile.files() )
    {
      fileInfos.try_emplace( fileInfo.filename, fileInfo );
    }

    // store list of files user defined data
    mediaSet->filesUserDefinedData( currentFileListFile.userDefinedData() );

    // store file list file check value
    mediaSet->listOfFilesCheckValueType( currentFileListFile.checkValueType() );
  }
  else
  {
    // otherwise, compare current list of files to first one
    if ( !fileListFile->belongsToSameMediaSet( currentFileListFile )
      || ( mediumIndex != currentFileListFile.mediaSequenceNumber() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "inconsistent file list file" }
        << boost::errinfo_file_name{ std::string{ Arinc665::ListOfFilesName } } );
    }
  }

  // check file integrity on current medium
  checkMediumFiles( mediumIndex );
}

void MediaSetImporterImpl::loadLoadListFile( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Load List File " << Arinc665::ListOfLoadsName;

  // Check Existence of Load List File within Files Information
  const auto loadListFileInfo{ fileInfos.find( Arinc665::ListOfLoadsName ) };
  if ( fileInfos.end() == loadListFileInfo )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Load List not in FILES.LUM" } );
  }

  // Load list of loads file
  Files::LoadListFile currentLoadListFile{
    readFileHandlerV( mediumIndex, Arinc665::ListOfLoadsName ) };

  if ( 1U == mediumIndex )
  {
    loadListFile = currentLoadListFile;
    loads.clear();
    for ( const auto &load : currentLoadListFile.loads() )
    {
      loads.emplace( load.headerFilename );

      // check existence of load header file
      const auto fileIt{ fileInfos.find( load.headerFilename ) };

      if ( fileIt == fileInfos.end() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "load header file not found" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      // checks that the load list and file list entry maps to the same file entry
      if ( load != fileIt->second )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
            << Helper::AdditionalInfo{ "data inconsistency" } );
      }
    }

    // store list of loads user defined data
    mediaSet->loadsUserDefinedData( currentLoadListFile.userDefinedData() );

    // store load list file check value
    mediaSet->listOfLoadsCheckValueType(
      std::get< 0 >( loadListFileInfo->second.checkValue ) );
  }
  else
  {
    // otherwise, check against stored version
    if ( !this->loadListFile->belongsToSameMediaSet( currentLoadListFile )
      || ( currentLoadListFile.mediaSequenceNumber() != mediumIndex ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{
          std::string{ Arinc665::ListOfLoadsName }
          + " is not consistent to other loads list" } );
    }
  }
}

void MediaSetImporterImpl::loadBatchListFile( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  // Check Existence of optional Batch List File within Files Information
  const auto batchListFileInfo{ fileInfos.find( Arinc665::ListOfBatchesName ) };
  if ( fileInfos.end() == batchListFileInfo )
  {
    return;
  }

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Batch List File " << Arinc665::ListOfBatchesName;

  // Load list of batches file
  Files::BatchListFile currentBatchListFile{
    readFileHandlerV( mediumIndex, Arinc665::ListOfBatchesName ) };

  if ( 1U == mediumIndex )
  {
    batchListFile = currentBatchListFile;
    batches.clear();
    for ( const auto &batch : currentBatchListFile.batches() )
    {
      batches.emplace( batch.filename );

      // check existence of batch file
      const auto fileIt{ fileInfos.find( batch.filename ) };

      if ( fileIt == fileInfos.end() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "batch file not found" }
          << boost::errinfo_file_name( std::string{ batch.filename } ) );
      }

      // checks that the batch list and file list entry maps to the same file entry
      if ( batch != fileIt->second )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "File inconsistency" } );
      }
    }

    // store list of batches user defined data
    mediaSet->batchesUserDefinedData( currentBatchListFile.userDefinedData() );

    // store batch list file check value
    mediaSet->listOfBatchesCheckValueType(
      std::get< 0 >( batchListFileInfo->second.checkValue ) );
  }
  else
  {
    // otherwise, check against stored version
    if ( !batchListFile->belongsToSameMediaSet( currentBatchListFile )
      || ( currentBatchListFile.mediaSequenceNumber() != mediumIndex ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{
          std::string{ Arinc665::ListOfBatchesName }
            + " is not consistent to other batches list" } );
    }
  }
}

void MediaSetImporterImpl::loadLoadHeaderFiles( const uint8_t mediumIndex )
{
  // iterate over all load information
  for ( const auto &load : loads )
  {
    // find load header in file list
    auto loadHeaderFileIt{ fileInfos.find( load ) };

    // Check existence
    if ( fileInfos.end() == loadHeaderFileIt )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Load header file not found in file list" }
        << boost::errinfo_file_name{ load } );
    }

    // skip load headers, which are not present on current medium
    if ( loadHeaderFileIt->second.memberSequenceNumber != mediumIndex )
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
      << "Load Header File " << loadHeaderFileIt->second.path().generic_string();

    // decode load header
    Files::LoadHeaderFile loadHeaderFile{
      readFileHandlerV( mediumIndex, loadHeaderFileIt->second.path() ) };

    // TODO Check Load Check Value
    // TODO Check Load CRC

    // add load header to global information
    loadHeaderFiles.try_emplace(
      loadHeaderFileIt->second.filename,
      std::move( loadHeaderFile ) );
  }
}

void MediaSetImporterImpl::loadBatchFiles( const uint8_t mediumIndex )
{
  for ( const auto &batch : batches )
  {
    // find batch in file list
    auto batchFileIt{ fileInfos.find ( batch ) };

    if ( fileInfos.end() == batchFileIt )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Medium is not consistent to media set" }
        << boost::errinfo_file_name{ batch } );
    }

    // Skip batch files not located on this medium
    if ( batchFileIt->second.memberSequenceNumber != mediumIndex )
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
      << "Load Batch File " << batchFileIt->second.path().generic_string();

    // Decode batch File
    Files::BatchFile batchFile{
      readFileHandlerV( mediumIndex, batchFileIt->second.path() ) };

    // add batch file to batch file list
    batchFiles.try_emplace( batchFileIt->second.filename, std::move( batchFile ) );
  }
}

void MediaSetImporterImpl::addFiles()
{
  // iterate over all files
  for ( const auto &[ fileName, fileInfo ] : fileInfos )
  {
    if ( loads.contains( fileName ) || batches.contains( fileName ) )
    {
      // skip load header and batch files
      continue;
    }

    // get file type
    // skip list files and handle load headers and batch files separate
    switch ( Arinc665::Files::Arinc665File::fileType( fileName ) )
    {
      case Arinc665::FileType::FileList:
      case Arinc665::FileType::LoadList:
      case Arinc665::FileType::BatchList:
        // List files are ignored
        continue;

      default:
        break;
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
  }

  addLoads();
  addBatches();
}

void MediaSetImporterImpl::addLoads()
{
  BOOST_LOG_FUNCTION()

  // iterate over load headers
  for ( const auto &[ filename, loadHeaderFile ] : loadHeaderFiles )
  {
    addLoad( filename, loadHeaderFile );
  }
}

void MediaSetImporterImpl::addLoad(
  std::string_view filename,
  const Files::LoadHeaderFile &loadHeaderFile )
{
  BOOST_LOG_FUNCTION()

  // obtain file information for load header
  const auto fileInfoIt{ fileInfos.find( filename ) };

  if ( fileInfos.end() == fileInfoIt )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Load not found" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  // obtain container (directory, medium), which will contain the load.
  const auto container{ checkCreateDirectory(
    fileInfoIt->second.memberSequenceNumber,
    fileInfoIt->second.path().parent_path() ) };

  // create load
  const auto loadPtr{ container->addLoad( filename ) };
  assert( loadPtr );

  // set check value indicator
  loadPtr->checkValueType( std::get< 0 >( fileInfoIt->second.checkValue ) );

  loadPtr->partFlags( loadHeaderFile.partFlags() );
  loadPtr->partNumber( loadHeaderFile.partNumber() );
  loadPtr->loadType( loadHeaderFile.loadType() );
  loadPtr->targetHardwareIdPositions(
    loadHeaderFile.targetHardwareIdPositions() );

  // iterate over data files
  for ( const auto &dataFile : loadHeaderFile.dataFiles() )
  {
    auto dataFilePtr{ mediaSet->regularFile( dataFile.filename ) };

    if ( !dataFilePtr )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
          << Helper::AdditionalInfo{ "Data file not found" }
          << boost::errinfo_file_name{ dataFile.filename } );
    }

    const auto &dataFileInfo{ fileInfos.find( dataFile.filename ) };

    if ( fileInfos.end() == dataFileInfo )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Data file not found" }
        << boost::errinfo_file_name{ dataFile.filename } );
    }

    // get memorised file size (only when file integrity is checked)
    if ( checkFileIntegrityV )
    {
      const auto dataFileSize{ fileSizeHandlerV(
        dataFileInfo->second.memberSequenceNumber,
        dataFileInfo->second.path() ) };

      // check load data file size - we divide by 2 to work around 16-bit size
      // storage within Supplement 2 LUHs (Only Data Files)
      if ( ( dataFileSize / 2 ) != ( dataFile.length / 2 ) )
      {
        BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
          << "Data File Size inconsistent " << dataFile.filename << " "
          << dataFileSize << " " << dataFile.length;

        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Data File Size inconsistent" }
          << boost::errinfo_file_name{ dataFile.filename } );
      }
    }

    // Check CRC
    if ( dataFileInfo->second.crc != dataFile.crc )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Data File CRC inconsistent" }
        << boost::errinfo_file_name{ dataFile.filename } );
    }

    // Check File Check Value
    if ( std::get< 0 >( dataFile.checkValue ) != Arinc645::CheckValueType::NotUsed )
    {
      if ( std::get< 0 >( dataFile.checkValue ) == std::get< 0 >( dataFileInfo->second.checkValue ) )
      {
        if ( std::get< 1 >( dataFile.checkValue ) != std::get< 1 >( dataFileInfo->second.checkValue ) )
        {
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "Data File Check Value inconsistent" }
            << boost::errinfo_file_name{ dataFile.filename } );
        }
      }
      else
      {
        //! @todo Check Check Type Value
      }
    }

    loadPtr->dataFile(
      dataFilePtr,
      dataFile.partNumber,
      std::get< 0 >( dataFile.checkValue ) );
  }

  // iterate over support files
  for ( const auto &supportFile : loadHeaderFile.supportFiles() )
  {
    auto supportFilePtr{ mediaSet->regularFile( supportFile.filename ) };

    if ( !supportFilePtr )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Support file not found" }
        << boost::errinfo_file_name{ supportFile.filename } );
    }

    const auto &supportFileInfo{ fileInfos.find( supportFile.filename ) };

    if ( fileInfos.end() == supportFileInfo )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
        << Helper::AdditionalInfo{ "Support file not found" }
        << boost::errinfo_file_name{ supportFile.filename } );
    }

    // get memorised file size ( only when file integrity is checked)
    if ( checkFileIntegrityV )
    {
      const auto supportFileSize{ fileSizeHandlerV(
        supportFileInfo->second.memberSequenceNumber,
        supportFileInfo->second.path() ) };

      // check load data file size
      if ( supportFileSize != supportFile.length )
      {
        BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
          << "Support File Size inconsistent "
          << supportFile.filename << " "
          << supportFileSize << " "
          << supportFile.length;

        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Support File Size inconsistent" }
          << boost::errinfo_file_name{ supportFile.filename } );
      }
    }

    // Check CRC
    if ( supportFileInfo->second.crc != supportFile.crc )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
          << Helper::AdditionalInfo{ "Support File CRC inconsistent" }
          << boost::errinfo_file_name{ supportFile.filename } );
    }

    // Check File Check Value
    if ( std::get< 0 >( supportFile.checkValue ) != Arinc645::CheckValueType::NotUsed )
    {
      if ( std::get< 0 >( supportFile.checkValue ) == std::get< 0 >( supportFileInfo->second.checkValue ) )
      {
        if ( std::get< 1 >( supportFile.checkValue ) != std::get< 1 >( supportFileInfo->second.checkValue ) )
        {
          BOOST_THROW_EXCEPTION(
            Arinc665Exception()
              << Helper::AdditionalInfo{ "Data File Check Value inconsistent" }
              << boost::errinfo_file_name{ supportFile.filename } );
        }
      }
      else
      {
        //! @todo Check Check Type Value
      }
    }

    loadPtr->supportFile(
      supportFilePtr,
      supportFile.partNumber,
      std::get< 0 >( supportFile.checkValue ) );
  }

  // User Defined Data
  loadPtr->userDefinedData( loadHeaderFile.userDefinedData() );
  // Load Check Value
  loadPtr->loadCheckValueType( loadHeaderFile.loadCheckValueType() );
}

void MediaSetImporterImpl::addBatches()
{
  BOOST_LOG_FUNCTION()

  // iterate over batches
  for ( const auto &[ filename, batchFile ] : batchFiles )
  {
    addBatch( filename, batchFile );
  }
}

void MediaSetImporterImpl::addBatch(
  std::string_view filename,
  const Files::BatchFile &batchFile )
{
  BOOST_LOG_FUNCTION()

  const auto fileInfoIt{ fileInfos.find( filename ) };

  if ( fileInfos.end() == fileInfoIt )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
      << Helper::AdditionalInfo{ "Batch not found" }
      << boost::errinfo_file_name{ std::string{ filename } } );
  }

  // obtain container (directory, medium), which will contain the batch.
  const auto container{ checkCreateDirectory(
    fileInfoIt->second.memberSequenceNumber,
    fileInfoIt->second.path().parent_path() ) };

  // create batch
  const auto batchPtr{ container->addBatch( filename ) };
  assert( batchPtr );

  // set check value indicator
  batchPtr->checkValueType( std::get< 0 >( fileInfoIt->second.checkValue  ) );

  batchPtr->partNumber( batchFile.partNumber() );
  batchPtr->comment( batchFile.comment() );

  // iterate over target hardware
  for ( const auto &targetHardware : batchFile.targetsHardware() )
  {
    Media::ConstLoads batchLoads{};

    // iterate over loads
    for ( const auto& load : targetHardware.loads )
    {
      auto loadPtr{ mediaSet->load( load.headerFilename ) };

      if ( !loadPtr )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Load not found" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      // check that Part Number Information matches
      if ( loadPtr->partNumber() != load.partNumber )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
          << Helper::AdditionalInfo{ "Load part Number does not match Batch Info" }
          << boost::errinfo_file_name{ std::string{ load.headerFilename } } );
      }

      batchLoads.emplace_back( std::move( loadPtr ) );
    }

    // add Target Hardware/ Position
    batchPtr->target( targetHardware.targetHardwareIdPosition, batchLoads );
  }
}

MediaSetImporterImpl::ContainerEntityPtr
MediaSetImporterImpl::checkCreateDirectory(
  const uint8_t mediumIndex,
  const std::filesystem::path &directoryPath )
{
  // make path relative (remove leading slash)
  const auto dirPath{ directoryPath.relative_path() };

  // we are in root-directory
  if ( dirPath.empty() )
  {
    return mediaSet->medium( mediumIndex );
  }

  ContainerEntityPtr dir{ mediaSet->medium( mediumIndex ) };

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

void MediaSetImporterImpl::checkMediumFiles( const uint8_t mediumIndex )
{
  // iterate over files
  for ( const auto &[ fileName, fileInfo ] : fileInfos )
  {
    // skip files, which are not part of the current medium
    if ( fileInfo.memberSequenceNumber != mediumIndex )
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

}

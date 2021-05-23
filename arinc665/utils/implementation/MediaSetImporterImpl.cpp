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

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Crc.hpp>

#include <arinc665/media/Directory.hpp>

namespace Arinc665::Utils {

MediaSetImporterImpl::MediaSetImporterImpl(
  Arinc665Utils::ReadFileHandler readFileHandler ):
  readFileHandler{ std::move( readFileHandler ) }
{
}

Media::MediaSetPtr MediaSetImporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  // iterate over additional media
  for (
    uint8_t mediumIndex = 1U;
    !loadMedium( mediumIndex );
    ++mediumIndex )
  {
  }

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>();
  mediaSet->partNumber( fileListFile->mediaSetPn());
  mediaSet->numberOfMedia( fileListFile->numberOfMediaSetMembers());

  // store list of files user defined data
  mediaSet->filesUserDefinedData( fileListFile->userDefinedData());
  // store list of loads user defined data
  mediaSet->loadsUserDefinedData( loadListFile->userDefinedData());
  // store list of batches user defined data
  if ( batchListFile )
  {
    mediaSet->batchesUserDefinedData( batchListFile->userDefinedData() );
  }

  // finally add all files (regular, load headers, batches) to the media set
  addFiles();

  return mediaSet;
}

bool MediaSetImporterImpl::loadMedium( const uint8_t mediumIndex )
{
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Medium " << (unsigned int)mediumIndex;

  loadFileListFile( mediumIndex );
  loadLoadListFile( mediumIndex );
  loadBatchListFile( mediumIndex );
  loadLoadHeaderFiles( mediumIndex );
  loadBatchFiles( mediumIndex );

  return mediumIndex == fileListFile->mediaSequenceNumber();
}

void MediaSetImporterImpl::loadFileListFile( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load File List File " << Arinc665::ListOfFilesName;

  // Load list of files file
  File::FileListFile currentFileListFile{
    readFileHandler( mediumIndex, Arinc665::ListOfFilesName ) };

  // store first list of files for further tests
  if ( 1U == mediumIndex )
  {
    fileListFile = currentFileListFile;
    fileInfos.clear();
    for ( const auto &fileInfo : currentFileListFile.files() )
    {
      fileInfos.emplace( fileInfo.filename(), fileInfo );
    }
  }
  else
  {
    // otherwise compare current list of files to first one
    if ( !this->fileListFile->belongsToSameMediaSet( currentFileListFile )
      || ( mediumIndex != currentFileListFile.mediaSequenceNumber() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{
          std::string{ Arinc665::ListOfFilesName }
          + " is not consistent to other file list" } );
    }
  }

  // iterate over files
  for ( const auto &[fileName, fileInfo] : fileInfos )
  {
    // skip files, which are not part of the current medium
    if ( fileInfo.memberSequenceNumber() != mediumIndex )
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
      << "Check file " << fileInfo.path();

    const auto rawFile{ readFileHandler( mediumIndex, fileInfo.path() ) };

    const uint16_t crc{ File::Arinc665File::calculateChecksum( rawFile, 0 ) };

    // compare checksums
    if ( crc != fileInfo.crc() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ fileInfo.path().string() + ": CRC of file invalid" } );
    }

    // remember file size
    fileSizes.emplace( fileName, rawFile.size() );
  }
}

void MediaSetImporterImpl::loadLoadListFile( const uint8_t mediumIndex )
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Load List File " << Arinc665::ListOfLoadsName;

  // Load list of loads file
  File::LoadListFile currentLoadListFile{
    readFileHandler( mediumIndex, Arinc665::ListOfLoadsName ) };

  if ( 1U == mediumIndex )
  {
    loadListFile = currentLoadListFile;
    loads.clear();
    for ( const auto &load : currentLoadListFile.loads() )
    {
      loads.emplace( load.headerFilename() );

      // check existence of load header file
      const auto fileIt{ fileInfos.find( load.headerFilename() ) };

      if ( fileIt == fileInfos.end() )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
            << Helper::AdditionalInfo( "load header file not found")
            << boost::errinfo_file_name( std::string{ load.headerFilename() } ) );
      }

      // checks that the load list and file list entry maps to the same file entry
      if ( load != fileIt->second )
      {
        //! @throw Arinc665Exception When load file and list file reference does not match
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
            << Helper::AdditionalInfo( "data inconsistency" ) );
      }
    }
  }
  else
  {
    // otherwise check against stored version
    if ( !this->loadListFile->belongsToSameMediaSet( currentLoadListFile )
      || ( currentLoadListFile.mediaSequenceNumber() != mediumIndex ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{
          std::string{ Arinc665::ListOfLoadsName}
          + " is not consistent to other loads list" } );
    }
  }
}

void MediaSetImporterImpl::loadBatchListFile( const uint8_t mediumIndex)
{
  BOOST_LOG_FUNCTION()

  assert( mediumIndex > 0U );

  // Check for optional batch list file
  if ( fileInfos.find( std::string{ ListOfBatchesName } ) == fileInfos.end())
  {
    return;
  }

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
    << "Load Batch List File " << Arinc665::ListOfBatchesName;

  // Load list of batches file
  File::BatchListFile currentBatchListFile{
    readFileHandler( mediumIndex, Arinc665::ListOfBatchesName ) };

  if ( 1U == mediumIndex )
  {
    batchListFile = currentBatchListFile;
    batches.clear();
    for ( const auto &batch : currentBatchListFile.batches() )
    {
      batches.emplace( batch.filename() );

      // check existence of batch file
      const auto fileIt{ fileInfos.find( batch.filename() ) };

      if ( fileIt == fileInfos.end() )
      {
        //! @throw Arinc665Exception When batch file is not found.
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
            << Helper::AdditionalInfo{ "batch file not found" }
            << boost::errinfo_file_name( std::string{ batch.filename() } ) );
      }

      // checks that the batch list and file list entry maps to the same file entry
      if ( batch != fileIt->second )
      {
        //! @throw Arinc665Exception When batch list file and list file reference does not match
        BOOST_THROW_EXCEPTION(
          Arinc665Exception() << Helper::AdditionalInfo{ "File inconsistency" } );
      }
    }
  }
  else
  {
    // otherwise check against stored version
    if ( !this->batchListFile->belongsToSameMediaSet( currentBatchListFile )
      || ( currentBatchListFile.mediaSequenceNumber() != mediumIndex ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{
          std::string{ Arinc665::ListOfBatchesName}
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
        << Helper::AdditionalInfo{ "Load header file not found in file list" } );
    }

    // skip load headers, which are not present on current medium
    if ( loadHeaderFileIt->second.memberSequenceNumber() != mediumIndex )
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
      << "Load Header File " << loadHeaderFileIt->second.path();

    // decode load header
    File::LoadHeaderFile loadHeaderFile{
      readFileHandler( mediumIndex, loadHeaderFileIt->second.path() ) };

    // add load header to global information
    loadHeaderFiles.emplace_back(
      loadHeaderFileIt->second.filename(),
      std::move( loadHeaderFile ) );
  }
}

void MediaSetImporterImpl::loadBatchFiles( const uint8_t mediumIndex )
{
  for ( const auto &batch : batches )
  {
    // find batch in file list
    auto batchFileIt{ fileInfos.find ( batch ) };

    if ( fileInfos.end() == batchFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo( "Medium is not consistent to media set" ) );
    }

    // Skip batch files not located on this medium
    if ( batchFileIt->second.memberSequenceNumber() != mediumIndex )
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::trace )
      << "Load Batch File " << batchFileIt->second.path();

    // Decode batch File
    File::BatchFile batchFile{
      readFileHandler( mediumIndex, batchFileIt->second.path() ) };

    // add batch file to to batch file list
    batchFiles.emplace_back(
      batchFileIt->second.filename(),
      std::move( batchFile ) );
  }
}

void MediaSetImporterImpl::addFiles()
{
  // iterate over all files
  for ( const auto &fileInfo : fileInfos )
  {
    if ( loads.end() != loads.find( fileInfo.first ) )
    {
      // skip load
      continue;
    }

    if ( batches.end() != batches.find( fileInfo.first ) )
    {
      // skip batch
      continue;
    }

    // get file type
    // skip list files and handle load headers and batch files separate
    switch ( Arinc665::File::Arinc665File::fileType( fileInfo.first ) )
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
    ContainerEntityPtr container{ checkCreateDirectory(
      fileInfo.second.memberSequenceNumber(),
      fileInfo.second.path().parent_path() ) };

    // place file
    auto filePtr{ container->addFile( fileInfo.second.filename() ) };
  }

  addLoads();
  addBatches();
}

void MediaSetImporterImpl::addLoads()
{
  BOOST_LOG_FUNCTION()

  // iterate over load headers
  for ( const auto &loadHeaderFile : loadHeaderFiles )
  {
    const auto fileInfoIt{ fileInfos.find( loadHeaderFile.first ) };

    // obtain container (directory, medium), which will contain the load.
    auto container{ checkCreateDirectory(
      fileInfoIt->second.memberSequenceNumber(),
      fileInfoIt->second.path().parent_path() ) };

    // create load
    auto loadPtr{ container->addLoad( loadHeaderFile.first ) };

    loadPtr->partNumber( loadHeaderFile.second.partNumber() );
    loadPtr->loadType( loadHeaderFile.second.loadType() );
    loadPtr->targetHardwareIdPositions(
      loadHeaderFile.second.targetHardwareIdPositions() );

    // iterate over data files
    for ( const auto &dataFile : loadHeaderFile.second.dataFiles() )
    {
      auto dataFilePtr{ mediaSet->file( dataFile.filename() ) };

      if ( !dataFilePtr )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Data file not found" )
          << boost::errinfo_file_name( std::string{ dataFile.filename() } ) );
      }

      // get memorised file size
      const auto dataFileSize = fileSizes.find( dataFile.filename() );
      if ( dataFileSize == fileSizes.end() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "No Size Info for Data File" )
          << boost::errinfo_file_name( std::string{ dataFile.filename() } ) );
      }

      // check load data file size - we divide by 2 to work around 16-bit size
      // storage within Supplement 2 LUHs (Only Data Files)
      if ( (dataFileSize->second / 2) != ( dataFile.length() / 2) )
      {
        BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
          << "Data File Size inconsistent "
          << dataFile.filename() << " "
          << dataFileSize->second << " "
          << dataFile.length();

        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Data File Size inconsistent" )
          << boost::errinfo_file_name( std::string{ dataFile.filename() } ) );
      }

      // Check CRC
      const auto &dataFileInfo{ fileInfos.find( dataFile.filename() ) };
      assert( dataFileInfo != fileInfos.end() );
      if ( dataFileInfo->second.crc() != dataFile.crc() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Data File CRC inconsistent" )
          << boost::errinfo_file_name( std::string{ dataFile.filename() } ) );
      }

      // TODO Check Type Value ?!?

      dataFilePtr->partNumber( dataFile.partNumber() );

      loadPtr->dataFile( dataFilePtr );
    }

    // iterate over support files
    for ( const auto &supportFile : loadHeaderFile.second.supportFiles() )
    {
      auto supportFilePtr{ mediaSet->file( supportFile.filename() ) };

      if ( !supportFilePtr )
      {
        BOOST_THROW_EXCEPTION(
          Arinc665Exception()
            << Helper::AdditionalInfo( "Support file not found" )
            << boost::errinfo_file_name( std::string{ supportFile.filename() } ) );
      }

      // get memorised file size
      const auto dataFileSize = fileSizes.find( supportFile.filename() );
      if ( dataFileSize == fileSizes.end() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "No Size Info for Support File" )
          << boost::errinfo_file_name( std::string{ supportFile.filename() } ) );
      }

      // check load data file size
      if ( dataFileSize->second != supportFile.length() )
      {
        BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
          << "Support File Size inconsistent "
            << supportFile.filename() << " "
            << dataFileSize->second << " "
            << supportFile.length();

        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Support File Size inconsistent" )
          << boost::errinfo_file_name( std::string{ supportFile.filename() } ) );
      }

      // Check CRC
      const auto &supportFileInfo{ fileInfos.find( supportFile.filename() ) };
      assert( supportFileInfo != fileInfos.end() );
      if ( supportFileInfo->second.crc() != supportFile.crc() )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Support File CRC inconsistent" )
          << boost::errinfo_file_name( std::string{ supportFile.filename() } ) );
      }

      // TODO Check Type Value ?!?

      supportFilePtr->partNumber( supportFile.partNumber() );

      loadPtr->supportFile( supportFilePtr );
    }

    // User Defined Data
    loadPtr->userDefinedData( loadHeaderFile.second.userDefinedData() );
  }
}

void MediaSetImporterImpl::addBatches()
{
  BOOST_LOG_FUNCTION()

  // iterate over batches
  for ( const auto &batchFile : batchFiles )
  {
    const auto fileInfoIt{ fileInfos.find( batchFile.first ) };

    // obtain container (directory, medium), which will contain the batch.
    auto container{ checkCreateDirectory(
      fileInfoIt->second.memberSequenceNumber(),
      fileInfoIt->second.path().parent_path() ) };

    // create batch
    auto batchPtr{ container->addBatch( batchFile.first ) };

    batchPtr->partNumber( batchFile.second.partNumber());
    batchPtr->comment( batchFile.second.comment());

    // iterate over target hardware
    for ( const auto &targetHardware : batchFile.second.targetsHardware() )
    {
      Media::WeakLoads loads;

      // iterate over loads
      for ( const auto& load : targetHardware.loads() )
      {
        auto loadPtr{ mediaSet->load( load.headerFilename() ) };

        if ( !loadPtr )
        {
          BOOST_THROW_EXCEPTION(
            Arinc665Exception()
              << Helper::AdditionalInfo( "Load not found" )
              << boost::errinfo_file_name( std::string{ load.headerFilename() } ) );
        }

        loads.push_back( loadPtr);
      }

      // add Target Hardware/ Position
      batchPtr->target( targetHardware.targetHardwareIdPosition(), loads);
    }
  }
}

MediaSetImporterImpl::ContainerEntityPtr
MediaSetImporterImpl::checkCreateDirectory(
  const uint8_t mediumIndex,
  const std::filesystem::path &directoryPath)
{
  // make path relative (remove leading slash)
  auto dirPath{ directoryPath.relative_path()};

  auto medium( mediaSet->medium( mediumIndex));

  // we are in root-directory
  if ( dirPath.empty() )
  {
    return medium;
  }

  ContainerEntityPtr dir{ medium };

  // iterate over path elements
  for ( auto &subPath : dirPath )
  {
    auto subDir{ dir->subDirectory( subPath.string())};

    // if sub-directory does not exists - create it
    if (!subDir)
    {
      subDir = dir->addSubDirectory( subPath.string());

      if (!subDir)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Cannot create sub-directory" ) );
      }
    }

    dir = subDir;
  }

  return dir;
}

}

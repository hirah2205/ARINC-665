/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetExporterImpl.
 **/

#include "MediaSetExporterImpl.hpp"

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/file/LoadListFile.hpp>
#include <arinc665/file/BatchListFile.hpp>
#include <arinc665/file/FileListFile.hpp>
#include <arinc665/file/LoadHeaderFile.hpp>
#include <arinc665/file/BatchFile.hpp>

#include <arinc665/Arinc665Crc.hpp>

#include <utility>

namespace Arinc665::Utils {

MediaSetExporterImpl::MediaSetExporterImpl(
  Media::ConstMediaSetPtr mediaSet,
  Arinc665Utils::CreateMediumHandler createMediumHandler,
  Arinc665Utils::CreateDirectoryHandler createDirectoryHandler,
  Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler,
  Arinc665Utils::CreateFileHandler createFileHandler,
  Arinc665Utils::WriteFileHandler writeFileHandler,
  Arinc665Utils::ReadFileHandler readFileHandler,
  const SupportedArinc665Version arinc665Version,
  const FileCreationPolicy createBatchFiles,
  const FileCreationPolicy createLoadHeaderFiles):
  arinc665Version{ arinc665Version },
  createBatchFiles{ createBatchFiles },
  createLoadHeaderFiles{ createLoadHeaderFiles },
  mediaSet{ std::move( mediaSet ) },
  createMediumHandler{ std::move( createMediumHandler ) },
  createDirectoryHandler{ std::move( createDirectoryHandler ) },
  checkFileExistenceHandler{ std::move( checkFileExistenceHandler ) },
  createFileHandler{ std::move( createFileHandler ) },
  writeFileHandler{ std::move( writeFileHandler ) },
  readFileHandler{ std::move( readFileHandler ) }
{
}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Media Set " << mediaSet->partNumber();

  for ( const auto &[mediumNumber, medium] : mediaSet->media() )
  {
    exportMedium( medium );
  }
}

void MediaSetExporterImpl::exportMedium( const Media::ConstMediumPtr &medium )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Medium #" << (unsigned int)medium->mediumNumber();

  // create the medium (i.e. create directory)
  createMediumHandler( medium );

  // export sub-directories
  for ( const auto &directory : medium->subDirectories() )
  {
    exportDirectory( directory );
  }

  // export files
  for ( const auto &file : medium->files() )
  {
    exportFile( file );
  }

  // export list of loads
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Loads";

  Arinc665::File::LoadListFile loadListFile{ arinc665Version };

  loadListFile.mediaSequenceNumber( medium->mediumNumber() );
  loadListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  loadListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all load to loads list */
  for ( const auto &load : medium->mediaSet()->loads() )
  {
    loadListFile.load( File::LoadInfo{
      std::string{ load->partNumber() },
      std::string{ load->name() },
      load->medium()->mediumNumber(),
      load->targetHardwareIds() } );
  }

  loadListFile.userDefinedData( mediaSet->loadsUserDefinedData() );

  writeFileHandler(
    medium->mediumNumber(),
    "/" + std::string{ ListOfLoadsName },
    static_cast< File::RawFile >( loadListFile ) );

  // export list of batches (if present)
  if ( medium->mediaSet()->numberOfBatches() != 0U )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "Export List of Batches";

    Arinc665::File::BatchListFile batchListFile{ arinc665Version };
    batchListFile.mediaSequenceNumber( medium->mediumNumber() );
    batchListFile.mediaSetPn( medium->mediaSet()->partNumber() );
    batchListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

    /* add all batches to batches list */
    for ( const auto &batch : medium->mediaSet()->batches() )
    {
      batchListFile.batch( File::BatchInfo{
        std::string{ batch->partNumber() },
        std::string{ batch->name() },
        batch->medium()->mediumNumber() } );
    }

    batchListFile.userDefinedData( mediaSet->batchesUserDefinedData() );

    writeFileHandler(
      medium->mediumNumber(),
      "/" + std::string{ ListOfBatchesName },
      static_cast< File::RawFile >( batchListFile ) );
  }

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Files";

  Arinc665::File::FileListFile fileListFile{ arinc665Version };
  fileListFile.mediaSequenceNumber( medium->mediumNumber() );
  fileListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  fileListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all files, load header files, and batch files to file list */
  for ( const auto &file : medium->mediaSet()->files() )
  {
    const auto rawFile{ readFileHandler( medium->mediumNumber(), file->path() ) };
    const uint16_t crc{ File::Arinc665File::calculateChecksum( rawFile, 0 ) };

    fileListFile.file( {
      std::string{ file->name() },
      File::Arinc665File::encodePath( file->path().parent_path() ),
      file->medium()->mediumNumber(),
      crc,
      {} } );
  }

  // add list of loads
  const auto rawListOfLoadsFile{
    readFileHandler(
      medium->mediumNumber(),
      "/" + std::string{ ListOfLoadsName } ) };
  const uint16_t listOfLoadsFileCrc{
    File::Arinc665File::calculateChecksum( rawListOfLoadsFile, 0 ) };

  fileListFile.file({
    std::string{ ListOfLoadsName },
    File::Arinc665File::encodePath( "/" ),
    medium->mediumNumber(),
    listOfLoadsFileCrc,
    {} } );

  // add list of batches - if present
  if ( medium->mediaSet()->numberOfBatches() != 0 )
  {
    const auto rawListOfBatchesFile{
      readFileHandler(
        medium->mediumNumber(),
        "/" + std::string{ ListOfBatchesName } ) };
    const uint16_t listOfBatchesFileCrc{
      File::Arinc665File::calculateChecksum( rawListOfBatchesFile, 0 ) };

    fileListFile.file( {
      std::string{ ListOfBatchesName },
      File::Arinc665File::encodePath( "/" ),
      medium->mediumNumber(),
      listOfBatchesFileCrc,
      {} } );
  }

  fileListFile.userDefinedData( mediaSet->filesUserDefinedData() );

  writeFileHandler(
    medium->mediumNumber(),
    "/" + std::string{ ListOfFilesName },
    static_cast< File::RawFile >( fileListFile ) );
}

void MediaSetExporterImpl::exportDirectory(
  const Media::ConstDirectoryPtr &directory )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export Directory to ["
    << (unsigned int)directory->medium()->mediumNumber()
    << "]:"
    << directory->path();

  createDirectoryHandler( directory );

  // export sub-directories
  for ( const auto &subDirectory : directory->subDirectories() )
  {
    exportDirectory( subDirectory );
  }

  // export files
  for ( const auto &file : directory->files() )
  {
    exportFile( file );
  }
}

void MediaSetExporterImpl::exportFile( const Media::ConstFilePtr &file ) const
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export File to ["
    << (unsigned int)file->medium()->mediumNumber()
    << "]:"
    << file->path();

  switch (file->fileType())
  {
    case Media::File::FileType::RegularFile:
      createFileHandler( file );
      break;

    case Media::File::FileType::LoadFile:
      switch (createLoadHeaderFiles)
      {
        case FileCreationPolicy::None:
          createFileHandler( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if (checkFileExistenceHandler( file ) )
          {
            createFileHandler( file );
          }
          else
          {
            createLoadHeaderFile( file );
          }
          break;

        case FileCreationPolicy::All:
          createLoadHeaderFile( file );
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "Invalid value of createLoadHeaderFiles" } );
      }
      break;

    case Media::File::FileType::BatchFile:
      switch (createBatchFiles)
      {
        case FileCreationPolicy::None:
          createFileHandler( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if ( checkFileExistenceHandler( file ) )
          {
            createFileHandler( file );
          }
          else
          {
            createBatchFile( file );
          }
          break;

        case FileCreationPolicy::All:
          createBatchFile( file );
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception()
            << Helper::AdditionalInfo{ "Invalid value of createBatchFiles" } );
      }
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid file type" } );
        /* no break: because THROW */
  }
}

void MediaSetExporterImpl::createLoadHeaderFile(
  const Media::ConstFilePtr &loadFile ) const
{
  // up-cast load file
  auto load{ std::dynamic_pointer_cast< const Media::Load>( loadFile ) };

  if ( !load )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Cannot cast file to load" } );
  }

  File::LoadHeaderFile loadHeaderFile{ arinc665Version };
  loadHeaderFile.partNumber( load->partNumber() );
  loadHeaderFile.targetHardwareIdPositions( load->targetHardwareIdPositions() );
  loadHeaderFile.loadType( load->loadType() );

  // calculate data files CRC and set data.
  for ( const auto &[ file,partNumber ] : load->dataFiles() )
  {
    auto dataFilePtr{ file.lock() };
    auto rawDataFile{ readFileHandler(
      dataFilePtr->medium()->mediumNumber(),
      dataFilePtr->path() ) };
    uint16_t fileCrc{
      File::Arinc665File::calculateChecksum( rawDataFile, 0 ) };

    loadHeaderFile.dataFile( {
      dataFilePtr->name(),
      partNumber,
      rawDataFile.size(),
      fileCrc } );
  }

  // calculate support files CRC and set data.
  for ( const auto &[file,partNumber] : load->supportFiles() )
  {
    auto supportFilePtr{ file.lock() };
    auto rawSupportFile{ readFileHandler(
      supportFilePtr->medium()->mediumNumber(),
      supportFilePtr->path() ) };
    uint16_t supportFileCrc{
      File::Arinc665File::calculateChecksum( rawSupportFile, 0 ) };

    loadHeaderFile.supportFile( {
      supportFilePtr->name(),
      partNumber,
      rawSupportFile.size(),
      supportFileCrc } );
  }

  // User Defined Data
  loadHeaderFile.userDefinedData( load->userDefinedData() );

  // calculate load CRC
  Arinc665Crc32 loadCrc;

  // load header load CRC calculation
  {
    File::RawFile rawLoadHeader{ loadHeaderFile };

    loadCrc.process_bytes(
      &( *rawLoadHeader.begin() ),
      rawLoadHeader.size() - sizeof( uint32_t ) );
  }

  // load data files for load CRC.
  for ( const auto &[file,partNumber] : load->dataFiles() )
  {
    auto dataFilePtr{ file.lock() };
    auto rawDataFile{ readFileHandler(
      dataFilePtr->medium()->mediumNumber(),
      dataFilePtr->path() ) };

    loadCrc.process_bytes( &(*rawDataFile.begin()), rawDataFile.size() );
  }

  // load support files for load CRC.
  for ( const auto &[file,partNumber] : load->supportFiles() )
  {
    auto supportFilePtr{ file.lock() };
    auto rawSupportFile{ readFileHandler(
      supportFilePtr->medium()->mediumNumber(),
      supportFilePtr->path() ) };

    loadCrc.process_bytes( &(*rawSupportFile.begin()), rawSupportFile.size() );
  }

  // set load CRC
  loadHeaderFile.loadCrc( loadCrc.checksum() );

  writeFileHandler(
    load->medium()->mediumNumber(),
    load->path(),
    static_cast< File::RawFile >( loadHeaderFile ) );
}

void MediaSetExporterImpl::createBatchFile(
  const Media::ConstFilePtr &file ) const
{
  // up-cast batch file
  auto batch{ std::dynamic_pointer_cast< const Media::Batch>( file ) };

  if ( !batch )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Cannot cast file to batch" } );
  }

  File::BatchFile batchFile{ arinc665Version };
  batchFile.partNumber( batch->partNumber() );
  batchFile.comment( batch->comment() );

  for ( const auto &[targetHwId,loads] : batch->targets() )
  {
    File::BatchLoadsInfo batchLoadsInfo{};

    for ( const auto &load : loads )
    {
      auto loadPtr{ load.lock() };
      batchLoadsInfo.emplace_back(
        loadPtr->name(),
        loadPtr->partNumber() );
    }

    batchFile.targetHardware(
      File::BatchTargetInfo{ targetHwId, batchLoadsInfo } );
  }

  writeFileHandler(
    batch->medium()->mediumNumber(),
    batch->path(),
    static_cast< File::RawFile >( batchFile ) );
}

}

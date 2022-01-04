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
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/files/LoadListFile.hpp>
#include <arinc665/files/BatchListFile.hpp>
#include <arinc665/files/FileListFile.hpp>
#include <arinc665/files/LoadHeaderFile.hpp>
#include <arinc665/files/BatchFile.hpp>

#include <arinc665/Arinc665Crc.hpp>

#include <utility>

namespace Arinc665::Utils {

MediaSetExporterImpl::MediaSetExporterImpl():
  arinc665VersionV{ SupportedArinc665Version::Supplement2 },
  createBatchFilesV{ FileCreationPolicy::None },
  createLoadHeaderFilesV{ FileCreationPolicy::None }
{
}

MediaSetExporter& MediaSetExporterImpl::mediaSet(
  Media::ConstMediaSetPtr mediaSet )
{
  mediaSetV = std::move( mediaSet );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createMediumHandler(
  CreateMediumHandler createMediumHandler )
{
  createMediumHandlerV = std::move( createMediumHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createDirectoryHandler(
  CreateDirectoryHandler createDirectoryHandler )
{
  createDirectoryHandlerV = std::move( createDirectoryHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::checkFileExistenceHandler(
  CheckFileExistenceHandler checkFileExistenceHandler )
{
  checkFileExistenceHandlerV = std::move( checkFileExistenceHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createFileHandler(
  CreateFileHandler createFileHandler )
{
  createFileHandlerV = std::move( createFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::writeFileHandler(
  WriteFileHandler writeFileHandler )
{
  writeFileHandlerV = std::move( writeFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::readFileHandler(
  ReadFileHandler readFileHandler )
{
  readFileHandlerV = std::move( readFileHandler );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::arinc665Version(
  SupportedArinc665Version arinc665Version )
{
  arinc665VersionV = std::move( arinc665Version );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createBatchFiles(
  FileCreationPolicy createBatchFiles )
{
  createBatchFilesV = std::move( createBatchFiles );
  return *this;
}

MediaSetExporter& MediaSetExporterImpl::createLoadHeaderFiles(
  FileCreationPolicy createLoadHeaderFiles )
{
  createLoadHeaderFilesV = std::move( createLoadHeaderFiles );
  return *this;
}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Media Set " << mediaSetV->partNumber();

  for ( const auto &[mediumNumber, medium] : mediaSetV->media() )
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
  createMediumHandlerV( medium );

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

  Arinc665::Files::LoadListFile loadListFile{ arinc665VersionV };

  loadListFile.mediaSequenceNumber( medium->mediumNumber() );
  loadListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  loadListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  /* add all loads to "list of loads" file  */
  for ( const auto &load : medium->mediaSet()->loads() )
  {
    loadListFile.load( Files::LoadInfo{
      std::string{ load->partNumber() },
      std::string{ load->name() },
      load->medium()->mediumNumber(),
      load->targetHardwareIds() } );
  }

  loadListFile.userDefinedData( mediaSetV->loadsUserDefinedData() );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfLoadsName },
    static_cast< Files::RawFile >( loadListFile ) );

  // export "list of batches" file (if present)
  if ( medium->mediaSet()->numberOfBatches() != 0U )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "Export List of Batches";

    Arinc665::Files::BatchListFile batchListFile{ arinc665VersionV };
    batchListFile.mediaSequenceNumber( medium->mediumNumber() );
    batchListFile.mediaSetPn( medium->mediaSet()->partNumber() );
    batchListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

    /* add all batches to batches list */
    for ( const auto &batch : medium->mediaSet()->batches() )
    {
      batchListFile.batch( Files::BatchInfo{
        std::string{ batch->partNumber() },
        std::string{ batch->name() },
        batch->medium()->mediumNumber() } );
    }

    batchListFile.userDefinedData( mediaSetV->batchesUserDefinedData() );

    writeFileHandlerV(
      medium->mediumNumber(),
      "/" + std::string{ ListOfBatchesName },
      static_cast< Files::RawFile >( batchListFile ) );
  }

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
    << "Export List of Files";

  Arinc665::Files::FileListFile fileListFile{ arinc665VersionV };
  fileListFile.mediaSequenceNumber( medium->mediumNumber() );
  fileListFile.mediaSetPn( medium->mediaSet()->partNumber() );
  fileListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia() );

  // add list of loads
  const auto rawListOfLoadsFile{
    readFileHandlerV(
      medium->mediumNumber(),
      "/" + std::string{ ListOfLoadsName } ) };
  const uint16_t listOfLoadsFileCrc{
    Files::Arinc665File::calculateChecksum( rawListOfLoadsFile, 0 ) };

  fileListFile.file({
    std::string{ ListOfLoadsName },
    Files::Arinc665File::encodePath( "/" ),
    medium->mediumNumber(),
    listOfLoadsFileCrc,
    {} } );

  // add list of batches - if present
  if ( medium->mediaSet()->numberOfBatches() != 0 )
  {
    const auto rawListOfBatchesFile{
      readFileHandlerV(
        medium->mediumNumber(),
        "/" + std::string{ ListOfBatchesName } ) };
    const uint16_t listOfBatchesFileCrc{
      Files::Arinc665File::calculateChecksum( rawListOfBatchesFile, 0 ) };

    fileListFile.file( {
      std::string{ ListOfBatchesName },
      Files::Arinc665File::encodePath( "/" ),
      medium->mediumNumber(),
      listOfBatchesFileCrc,
      {} } );
  }

  /* add all files, load header files, and batch files to file list */
  for ( const auto &file : medium->mediaSet()->files() )
  {
    const auto rawFile{ readFileHandlerV( medium->mediumNumber(), file->path() ) };
    const uint16_t crc{ Files::Arinc665File::calculateChecksum( rawFile, 0 ) };

    fileListFile.file( {
      std::string{ file->name() },
      Files::Arinc665File::encodePath( file->path().parent_path() ),
      file->medium()->mediumNumber(),
      crc,
      {} } );
  }

  fileListFile.userDefinedData( mediaSetV->filesUserDefinedData() );

  writeFileHandlerV(
    medium->mediumNumber(),
    "/" + std::string{ ListOfFilesName },
    static_cast< Files::RawFile >( fileListFile ) );
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

  createDirectoryHandlerV( directory );

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
    case Media::RegularFile::FileType::RegularFile:
      createFileHandlerV( file );
      break;

    case Media::RegularFile::FileType::LoadFile:
      switch ( createLoadHeaderFilesV )
      {
        case FileCreationPolicy::None:
          createFileHandlerV( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if (checkFileExistenceHandlerV( file ) )
          {
            createFileHandlerV( file );
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

    case Media::RegularFile::FileType::BatchFile:
      switch ( createBatchFilesV )
      {
        case FileCreationPolicy::None:
          createFileHandlerV( file );
          break;

        case FileCreationPolicy::NoneExisting:
          if ( checkFileExistenceHandlerV( file ) )
          {
            createFileHandlerV( file );
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

  Files::LoadHeaderFile loadHeaderFile{ arinc665VersionV };
  loadHeaderFile.partFlags( load->partFlags() );
  loadHeaderFile.partNumber( load->partNumber() );
  loadHeaderFile.targetHardwareIdPositions( load->targetHardwareIdPositions() );
  loadHeaderFile.loadType( load->loadType() );

  // calculate data files CRC and set data.
  for ( const auto &[ file,partNumber ] : load->dataFiles() )
  {
    auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };
    uint16_t fileCrc{
      Files::Arinc665File::calculateChecksum( rawDataFile, 0 ) };

    loadHeaderFile.dataFile( Files::LoadFileInfo{
      std::string{ file->name() },
      partNumber,
      rawDataFile.size(),
      fileCrc,
      {} } );
  }

  // calculate support files CRC and set data.
  for ( const auto &[file,partNumber] : load->supportFiles() )
  {
    auto rawSupportFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };
    uint16_t supportFileCrc{
      Files::Arinc665File::calculateChecksum( rawSupportFile, 0 ) };

    loadHeaderFile.supportFile( Files::LoadFileInfo{
      std::string{ file->name() },
      partNumber,
      rawSupportFile.size(),
      supportFileCrc,
      {} } );
  }

  // User Defined Data
  loadHeaderFile.userDefinedData( load->userDefinedData() );

  // calculate load CRC
  Arinc665Crc32 loadCrc;

  // load header load CRC calculation
  {
    Files::RawFile rawLoadHeader{ loadHeaderFile };

    loadCrc.process_bytes(
      &( *rawLoadHeader.begin() ),
      rawLoadHeader.size() - sizeof( uint32_t ) );
  }

  // load data files for load CRC.
  for ( const auto &[file,partNumber] : load->dataFiles() )
  {
    auto rawDataFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes( &(*rawDataFile.begin()), rawDataFile.size() );
  }

  // load support files for load CRC.
  for ( const auto &[file,partNumber] : load->supportFiles() )
  {
    auto rawSupportFile{ readFileHandlerV(
      file->medium()->mediumNumber(),
      file->path() ) };

    loadCrc.process_bytes( &(*rawSupportFile.begin()), rawSupportFile.size() );
  }

  // set load CRC
  loadHeaderFile.loadCrc( loadCrc.checksum() );

  writeFileHandlerV(
    load->medium()->mediumNumber(),
    load->path(),
    static_cast< Files::RawFile >( loadHeaderFile ) );
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

  Files::BatchFile batchFile{ arinc665VersionV };
  batchFile.partNumber( batch->partNumber() );
  batchFile.comment( batch->comment() );

  for ( const auto &[targetHwId,loads] : batch->targets() )
  {
    Files::BatchLoadsInfo batchLoadsInfo{};

    for ( const auto &load : loads )
    {
      auto loadPtr{ load.lock() };
      batchLoadsInfo.emplace_back( Files::BatchLoadInfo{
        std::string{ loadPtr->name() },
        std::string{ loadPtr->partNumber() } } );
    }

    batchFile.targetHardware(
      Files::BatchTargetInfo{ targetHwId, batchLoadsInfo } );
  }

  writeFileHandlerV(
    batch->medium()->mediumNumber(),
    batch->path(),
    static_cast< Files::RawFile >( batchFile ) );
}

}

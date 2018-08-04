/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Utils::MediaSetExporterImpl.
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

namespace Arinc665::Utils {

MediaSetExporterImpl::MediaSetExporterImpl(
  Media::ConstMediaSetPtr mediaSet,
  Arinc665Utils::CreateMediumHandler createMediumHandler,
  Arinc665Utils::CreateDirectoryHandler createDirectoryHandler,
  Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler,
  Arinc665Utils::CreateFileHandler createFileHandler,
  Arinc665Utils::WriteFileHandler writeFileHandler,
  Arinc665Utils::ReadFileHandler readFileHandler,
  const Arinc665Version arinc665Version,
  const FileCreationPolicy createBatchFiles,
  const FileCreationPolicy createLoadHeaderFiles):
  arinc665Version( arinc665Version),
  createBatchFiles( createBatchFiles),
  createLoadHeaderFiles( createLoadHeaderFiles),
  mediaSet( mediaSet),
  createMediumHandler( createMediumHandler),
  createDirectoryHandler( createDirectoryHandler),
  checkFileExistenceHandler( checkFileExistenceHandler),
  createFileHandler( createFileHandler),
  writeFileHandler( writeFileHandler),
  readFileHandler( readFileHandler)
{

}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Media set " << " - " << mediaSet->partNumber();

  for (auto medium : mediaSet->media())
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Medium " << (unsigned int)medium.first;

    exportMedium( medium.second);
  }
}

void MediaSetExporterImpl::exportMedium( Media::ConstMediumPtr medium)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export medium " << (unsigned int)medium->mediumNumber();

  // create the medium (i.g. create directory)
  createMediumHandler( medium);

  // export sub-directories
  for ( auto directory : medium->subDirectories())
  {
    exportDirectory( directory);
  }

  // export files
  for ( auto file : medium->files())
  {
    exportFile( file);
  }

  // export list of loads
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export list of loads";

  Arinc665::File::LoadListFile loadListFile{ arinc665Version};
  loadListFile.mediaSequenceNumber( medium->mediumNumber());
  loadListFile.mediaSetPn( medium->partNumber());
  loadListFile.numberOfMediaSetMembers(  medium->mediaSet()->numberOfMedia());

  /* add all load to loads list */
  for ( auto &load : medium->mediaSet()->loads())
  {
    loadListFile.loadInfo({
      load->partNumber(),
      load->name(),
      load->medium()->mediumNumber(),
      load->targetHardwareIds()});
  }

  writeFileHandler( medium->mediumNumber(), "/" + ListOfLoadsName, loadListFile);

  // export list of batches (if present)
  if (medium->mediaSet()->numberOfBatches() != 0)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Export list of batches";

    Arinc665::File::BatchListFile batchListFile( arinc665Version);
    batchListFile.mediaSequenceNumber( medium->mediumNumber());
    batchListFile.mediaSetPn( medium->partNumber());
    batchListFile.numberOfMediaSetMembers(  medium->mediaSet()->numberOfMedia());

    /* add all batches to batches list */
    for ( auto &batch : medium->mediaSet()->batches())
    {
      batchListFile.batchInfo({
        batch->partNumber(),
        batch->name(),
        batch->medium()->mediumNumber()});
    }

    writeFileHandler( medium->mediumNumber(), "/" + ListOfBatchesName, batchListFile);
  }

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export list of files";

  Arinc665::File::FileListFile fileListFile{ arinc665Version};
  fileListFile.mediaSequenceNumber( medium->mediumNumber());
  fileListFile.mediaSetPn( medium->partNumber());
  fileListFile.numberOfMediaSetMembers( medium->mediaSet()->numberOfMedia());

  /* add all files, load header files, and batch files to file list */
  for ( auto &file : medium->mediaSet()->files())
  {
    auto rawFile( readFileHandler( medium->mediumNumber(), file->path()));
    uint16_t crc( File::Arinc665File::calculateChecksum( rawFile, 0));

    fileListFile.fileInfo({
      file->name(),
      File::Arinc665File::encodePath( file->path().parent_path()),
      file->medium()->mediumNumber(),
      crc});
  }

  // add list of loads
  auto rawListOfLoadsFile(
    readFileHandler( medium->mediumNumber(), "/" + ListOfLoadsName));
  uint16_t listOfLoadsFileCrc(
    File::Arinc665File::calculateChecksum( rawListOfLoadsFile, 0));

  fileListFile.fileInfo({
    ListOfLoadsName,
    File::Arinc665File::encodePath( "/"),
    medium->mediumNumber(),
    listOfLoadsFileCrc});

  // add list of batches - if present
  if (medium->mediaSet()->numberOfBatches() != 0)
  {
    auto rawListOfBatchesFile(
      readFileHandler( medium->mediumNumber(), "/" + ListOfBatchesName));
    uint16_t listOfBatchesFileCrc(
      File::Arinc665File::calculateChecksum( rawListOfBatchesFile, 0));

    fileListFile.fileInfo({
      ListOfBatchesName,
      File::Arinc665File::encodePath( "/"),
      medium->mediumNumber(),
      listOfBatchesFileCrc});
  }

  writeFileHandler( medium->mediumNumber(), "/" + ListOfFilesName, fileListFile);
}

void MediaSetExporterImpl::exportDirectory( Media::ConstDirectoryPtr directory)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export directory to [" <<
    (unsigned int)directory->medium()->mediumNumber() <<
    "]:" <<
    directory->path();

  createDirectoryHandler( directory);

  // export sub-directories
  for ( auto directory : directory->subDirectories())
  {
    exportDirectory( directory);
  }

  // export files
  for ( auto file : directory->files())
  {
    exportFile( file);
  }
}

void MediaSetExporterImpl::exportFile( Media::ConstFilePtr file)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export file to [" <<
    (unsigned int)file->medium()->mediumNumber() <<
    "]:" <<
    file->path();

  switch (file->fileType())
  {
    case Media::File::FileType::RegularFile:
      createFileHandler( file);
      break;

    case Media::File::FileType::LoadFile:
      switch (createLoadHeaderFiles)
      {
        case FileCreationPolicy::None:
          createFileHandler( file);
          break;

        case FileCreationPolicy::NoneExisting:
          if (checkFileExistenceHandler( file))
          {
            createFileHandler( file);
          }
          else
          {
            createLoadHeaderFile( file);
          }
          break;

        case FileCreationPolicy::All:
          createLoadHeaderFile( file);
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception() <<
            AdditionalInfo( "Invalid value of createLoadHeaderFiles"));
      }
      break;

    case Media::File::FileType::BatchFile:
      switch (createBatchFiles)
      {
        case FileCreationPolicy::None:
          createFileHandler( file);
          break;

        case FileCreationPolicy::NoneExisting:
          if (checkFileExistenceHandler( file))
          {
            createFileHandler( file);
          }
          else
          {
            createBatchFile( file);
          }
          break;

        case FileCreationPolicy::All:
          createBatchFile( file);
          break;

        default:
          BOOST_THROW_EXCEPTION( Arinc665Exception() <<
            AdditionalInfo( "Invalid value of createBatchFiles"));
      }
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Invalid file type"));
        /* no break: because THROW */
  }
}

void MediaSetExporterImpl::createLoadHeaderFile( Media::ConstFilePtr file)
{
  auto load{ std::dynamic_pointer_cast< const Media::Load>( file)};

  if (!load)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Cannot cast file to load"));
  }

  File::LoadHeaderFile loadHeaderFile( Arinc665Version::ARINC_665_2);
  loadHeaderFile.partNumber( load->partNumber());
  loadHeaderFile.targetHardwareIds( load->targetHardwareIds());

  // calculate data files CRC and set data.
  for ( auto dataFile : load->dataFiles())
  {
    auto dataFilePtr{ dataFile.lock()};
    auto rawDataFile{ readFileHandler(
      dataFilePtr->medium()->mediumNumber(),
      dataFilePtr->path())};
    uint16_t dataFileCrc{
      File::Arinc665File::calculateChecksum( rawDataFile, 0)};

    loadHeaderFile.dataFile( {
      dataFilePtr->name(),
      dataFilePtr->partNumber(),
      static_cast< uint32_t>( rawDataFile.size() / 2),
      dataFileCrc});
  }

  // calculate support files CRC and set data.
  for ( auto supportFile : load->supportFiles())
  {
    auto supportFilePtr{ supportFile.lock()};
    auto rawSupportFile{ readFileHandler(
      supportFilePtr->medium()->mediumNumber(),
      supportFilePtr->path())};
    uint16_t supportFileCrc{
      File::Arinc665File::calculateChecksum( rawSupportFile, 0)};

    loadHeaderFile.supportFile( {
      supportFilePtr->name(),
      supportFilePtr->partNumber(),
      static_cast< uint32_t>( rawSupportFile.size() / 2),
      supportFileCrc});
  }

  // calculate load CRC
  Arinc665Crc32 loadCrc;

  // load header load CRC calculation
  {
    File::RawFile rawLoadHeader{ loadHeaderFile};

    loadCrc.process_bytes(
      &(*rawLoadHeader.begin()),
      rawLoadHeader.size() - sizeof( uint32_t));
  }

  // load data files for load CRC.
  for ( auto dataFile : load->dataFiles())
  {
    auto dataFilePtr{ dataFile.lock()};
    auto rawDataFile{ readFileHandler(
      dataFilePtr->medium()->mediumNumber(),
      dataFilePtr->path())};

    loadCrc.process_bytes( &(*rawDataFile.begin()), rawDataFile.size());
  }

  // load support files for load CRC.
  for ( auto supportFile : load->supportFiles())
  {
    auto supportFilePtr{ supportFile.lock()};
    auto rawSupportFile{ readFileHandler(
      supportFilePtr->medium()->mediumNumber(),
      supportFilePtr->path())};

    loadCrc.process_bytes( &(*rawSupportFile.begin()), rawSupportFile.size());
  }

  // set load CRC
  loadHeaderFile.loadCrc( loadCrc.checksum());

  writeFileHandler(
    load->medium()->mediumNumber(),
    load->path(),
    loadHeaderFile);
}

void MediaSetExporterImpl::createBatchFile( Media::ConstFilePtr file)
{
  auto batch{ std::dynamic_pointer_cast< const Media::Batch>( file)};

  if (!batch)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Cannot cast file to batch"));
  }

  File::BatchFile batchFile{ Arinc665Version::ARINC_665_2};
  batchFile.partNumber( batch->partNumber());
  batchFile.comment( batch->comment());

  for ( auto target : batch->targets())
  {
    File::BatchLoadsInfo batchLoadsInfo;
    for (auto load : target.second)
    {
      auto loadPtr{ load.lock()};
      batchLoadsInfo.emplace_back(
        loadPtr->name(),
        loadPtr->partNumber());
    }

    batchFile.targetHardware(
      File::BatchTargetInfo{ target.first, batchLoadsInfo});
  }

  writeFileHandler(
    batch->medium()->mediumNumber(), batch->path(), batchFile);
}

}

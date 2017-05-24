/*
 * $Date$
 * $Revision$
 */
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

namespace Arinc665 {
namespace Utils {

MediaSetExporterImpl::MediaSetExporterImpl(
  Media::ConstMediaSetPtr mediaSet,
  Arinc665Utils::CreateMediumHandler createMediumHandler,
  Arinc665Utils::CreateDirectoryHandler createDirectoryHandler,
  Arinc665Utils::CreateFileHandler createFileHandler,
  Arinc665Utils::WriteFileHandler writeFileHandler,
  Arinc665Utils::ReadFileHandler readFileHandler,
  const Arinc665Version arinc665Version,
  const bool createBatchFiles,
  const bool createLoadHeaderFiles):
  arinc665Version( arinc665Version),
  createBatchFiles( createBatchFiles),
  createLoadHeaderFiles( createLoadHeaderFiles),
  mediaSet( mediaSet),
  createMediumHandler( createMediumHandler),
  createDirectoryHandler( createDirectoryHandler),
  createFileHandler( createFileHandler),
  writeFileHandler( writeFileHandler),
  readFileHandler( readFileHandler)
{

}

void MediaSetExporterImpl::operator()()
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) << "Media set " <<
    mediaSet->getName() << " - " << mediaSet->getPartNumber();

  for (auto medium : mediaSet->getMedia())
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) << "Medium " <<
      (unsigned int)medium->getMediumNumber();

    exportMedium( medium);
  }
}

void MediaSetExporterImpl::exportMedium( Media::ConstMediumPtr medium)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export medium " << (unsigned int)medium->getMediumNumber();

  createMediumHandler( medium);

  // export sub-directories
  for ( auto directory : medium->getSubDirectories())
  {
    exportDirectory( directory);
  }

  // export files
  for ( auto file : medium->getFiles())
  {
    exportFile( file);
  }

  // export list of loads
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export list of loads";

  Arinc665::File::LoadListFile loadListFile( arinc665Version);
  loadListFile.setMediaSequenceNumber( medium->getMediumNumber());
  loadListFile.setMediaSetPn( medium->getPartNumber());
  loadListFile.setNumberOfMediaSetMembers(  medium->getMediaSet()->getNumberOfMedia());
  /* add all load to loads list */
  for ( auto &load : medium->getMediaSet()->getLoads())
  {
    loadListFile.addLoadInfo({
      load->getPartNumber(),
      load->getName(),
      load->getMedium()->getMediumNumber(),
      load->getTargetHardwareIdList()});
  }

  loadListFile.calculateCrc();
  writeFileHandler( medium->getMediumNumber(), "/" + ListOfLoadsName, loadListFile);

  // export list of batches (if present)
  if (medium->getMediaSet()->getNumberOfBatches() != 0)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Export list of batches";

    Arinc665::File::BatchListFile batchListFile( arinc665Version);
    batchListFile.setMediaSequenceNumber( medium->getMediumNumber());
    batchListFile.setMediaSetPn( medium->getPartNumber());
    batchListFile.setNumberOfMediaSetMembers(  medium->getMediaSet()->getNumberOfMedia());
    /* add all batches to batches list */
    for ( auto &batch : medium->getMediaSet()->getBatches())
    {
      batchListFile.addBatchInfo({
        batch->getPartNumber(),
        batch->getName(),
        batch->getMedium()->getMediumNumber()});
    }

    batchListFile.calculateCrc();
    writeFileHandler( medium->getMediumNumber(), "/" + ListOfBatchesName, batchListFile);
  }

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export list of files";

  Arinc665::File::FileListFile fileListFile( arinc665Version);
  fileListFile.setMediaSequenceNumber( medium->getMediumNumber());
  fileListFile.setMediaSetPn( medium->getPartNumber());
  fileListFile.setNumberOfMediaSetMembers( medium->getMediaSet()->getNumberOfMedia());
  /* add all files, load header files, and batch files to file list */
  for ( auto &file : medium->getMediaSet()->getFiles())
  {
    auto rawFile( readFileHandler( medium->getMediumNumber(), file->getPath()));
    uint16_t crc( File::Arinc665File::calculateChecksum( rawFile, 0));

    fileListFile.addFileInfo({
      file->getName(),
      File::Arinc665File::encodePath( file->getPath().parent_path()),
      file->getMedium()->getMediumNumber(),
      crc});
  }

  // add list of loads
  auto rawListOfLoadsFile(
    readFileHandler( medium->getMediumNumber(), "/" + ListOfLoadsName));
  uint16_t listOfLoadsFileCrc(
    File::Arinc665File::calculateChecksum( rawListOfLoadsFile, 0));

  fileListFile.addFileInfo({
    ListOfLoadsName,
    File::Arinc665File::encodePath( "/"),
    medium->getMediumNumber(),
    listOfLoadsFileCrc});

  // add list of batches - if present
  if (medium->getMediaSet()->getNumberOfBatches() != 0)
  {
    auto rawListOfBatchesFile(
      readFileHandler( medium->getMediumNumber(), "/" + ListOfBatchesName));
    uint16_t listOfBatchesFileCrc(
      File::Arinc665File::calculateChecksum( rawListOfBatchesFile, 0));

    fileListFile.addFileInfo({
      ListOfBatchesName,
      File::Arinc665File::encodePath( "/"),
      medium->getMediumNumber(),
      listOfBatchesFileCrc});
  }

  fileListFile.calculateCrc();
  writeFileHandler( medium->getMediumNumber(), "/" + ListOfFilesName, fileListFile);
}

void MediaSetExporterImpl::exportDirectory( Media::ConstDirectoryPtr directory)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export directory to [" <<
    (unsigned int)directory->getMedium()->getMediumNumber() <<
    "]:" <<
    directory->getPath();

  createDirectoryHandler( directory);

  // export sub-directories
  for ( auto directory : directory->getSubDirectories())
  {
    exportDirectory( directory);
  }

  // export files
  for ( auto file : directory->getFiles())
  {
    exportFile( file);
  }
}

void MediaSetExporterImpl::exportFile( Media::ConstFilePtr file)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export file to [" <<
    (unsigned int)file->getMedium()->getMediumNumber() <<
    "]:" <<
    file->getPath();

  switch (file->getFileType())
  {
    case Media::File::FileType::RegularFile:
      createFileHandler( file);
      break;

    case Media::File::FileType::LoadFile:
      if (createLoadHeaderFiles)
      {
        auto load( std::dynamic_pointer_cast< const Media::Load>( file));
        if (!load)
        {
          BOOST_THROW_EXCEPTION( Arinc665Exception() <<
            AdditionalInfo( "Cannot cast file to load"));
        }

        File::LoadHeaderFile loadHeaderFile( Arinc665Version::ARINC_665_2);
        loadHeaderFile.setPartNumber( load->getPartNumber());
        loadHeaderFile.setTargetHardwareIdList( load->getTargetHardwareIdList());

        Arinc665Crc32 loadCrc;

        for ( auto dataFile : load->getDataFiles())
        {
          auto dataFilePtr( dataFile.lock());
          auto rawDataFile( readFileHandler(
            dataFilePtr->getMedium()->getMediumNumber(),
            dataFilePtr->getPath()));
          uint16_t dataFileCrc(
            File::Arinc665File::calculateChecksum( rawDataFile, 0));

          loadHeaderFile.addDataFile( {
            dataFilePtr->getName(),
            dataFilePtr->getPartNumber(),
            static_cast< uint32_t>( rawDataFile.size() / 2),
            dataFileCrc});

          loadCrc.process_block(
            &(*rawDataFile.begin()),
            &(*rawDataFile.begin()) + rawDataFile.size());
        }

        for ( auto supportFile : load->getSupportFiles())
        {
          auto supportFilePtr( supportFile.lock());
          auto rawSupportFile( readFileHandler(
            supportFilePtr->getMedium()->getMediumNumber(),
            supportFilePtr->getPath()));
          uint16_t supportFileCrc(
            File::Arinc665File::calculateChecksum( rawSupportFile, 0));

          loadHeaderFile.addDataFile( {
            supportFilePtr->getName(),
            supportFilePtr->getPartNumber(),
            static_cast< uint32_t>( rawSupportFile.size() / 2),
            supportFileCrc});

          loadCrc.process_block(
            &(*rawSupportFile.begin()),
            &(*rawSupportFile.begin()) + rawSupportFile.size());
        }

        loadHeaderFile.calculateCrc();

        File::RawFile rawLoadHeader( loadHeaderFile);

        loadCrc.process_block(
          &(*rawLoadHeader.begin()),
          &(*rawLoadHeader.begin()) + rawLoadHeader.size() - 4U);

        loadHeaderFile.setLoadCrc( loadCrc.checksum());
        writeFileHandler(
          load->getMedium()->getMediumNumber(), load->getPath(), loadHeaderFile);
      }
      else
      {
        createFileHandler( file);
      }
      break;

    case Media::File::FileType::BatchFile:
      if (createBatchFiles)
      {
        auto batch( std::dynamic_pointer_cast< const Media::Batch>( file));
        if (!batch)
        {
          BOOST_THROW_EXCEPTION( Arinc665Exception() <<
            AdditionalInfo( "Cannot cast file to batch"));
        }

        File::BatchFile batchFile( Arinc665Version::ARINC_665_2);
        batchFile.setPartNumber( batch->getPartNumber());
        batchFile.setComment( batch->getComment());

        for ( auto target : batch->getTargets())
        {
          File::BatchLoadsInfo batchLoadsInfo;
          for (auto load : target.second)
          {
            auto loadPtr( load.lock());
            batchLoadsInfo.emplace_back(
              loadPtr->getName(),
              loadPtr->getPartNumber());
          }

          batchFile.addTargetHardware(
            File::BatchTargetInfo{target.first, batchLoadsInfo});
        }

        batchFile.calculateCrc();
        writeFileHandler(
          batch->getMedium()->getMediumNumber(), batch->getPath(), batchFile);
      }
      else
      {
        createFileHandler( file);
      }
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Invalid file type"));
        /* no break: because THROW */
  }
}

}
}

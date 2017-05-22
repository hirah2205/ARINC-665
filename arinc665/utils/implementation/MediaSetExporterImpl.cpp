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

#include <arinc665/file/list/LoadListFile.hpp>
#include <arinc665/file/list/BatchListFile.hpp>
#include <arinc665/file/list/FileListFile.hpp>

namespace Arinc665 {
namespace Utils {

MediaSetExporterImpl::MediaSetExporterImpl(
  Media::ConstMediaSetPtr mediaSet,
  Arinc665Utils::CreateFileHandler createFileHandler,
  Arinc665Utils::WriteFileHandler writeFileHandler,
  Arinc665Utils::ReadFileHandler readFileHandler,
  const Arinc665Version arinc665Version,
  bool createBatchFiles,
  bool createLoadHeaderFiles):
  arinc665Version( arinc665Version),
  createBatchFiles( createBatchFiles),
  createLoadHeaderFiles( createLoadHeaderFiles),
  mediaSet( mediaSet),
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

  }

  // export medium info
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export list of files";

  Arinc665::File::FileListFile fileListFile( arinc665Version);
  fileListFile.setMediaSequenceNumber( medium->getMediumNumber());
  fileListFile.setMediaSetPn( medium->getPartNumber());
  fileListFile.setNumberOfMediaSetMembers(  medium->getMediaSet()->getNumberOfMedia());
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
  /* add list of loads */
  //! @todo

  /* add list of batches - if present */
  //! @todo

  fileListFile.calculateCrc();
  writeFileHandler( medium->getMediumNumber(), "/" + ListOfFilesName, fileListFile);
}

void MediaSetExporterImpl::exportDirectory( Media::ConstDirectoryPtr directory)
{
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export directory to " << directory->getPath();

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
  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export file to " << file->getPath();

  switch (file->getFileType())
  {
    case Media::File::FileType::RegularFile:
      createFileHandler( file);
      break;

    case Media::File::FileType::LoadFile:
      if (createLoadHeaderFiles)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "Not implemented"));
      }
      createFileHandler( file);
      break;

    case Media::File::FileType::BatchFile:
      if (createBatchFiles)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "Not implemented"));
      }
      createFileHandler( file);
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Invalid file type"));
        /* no break: because THROW */
  }
}

}
}

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
  bool createBatchFiles,
  bool createLoadHeaderFiles):
  mediaSet( mediaSet),
  createFileHandler( createFileHandler),
  writeFileHandler( writeFileHandler),
  createBatchFiles( createBatchFiles),
  createLoadHeaderFiles( createLoadHeaderFiles)
{

}

void MediaSetExporterImpl::operator()()
{
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
#if 0
  Arinc665::File::LoadListFile loadListFile;
  loadListFile.setMediaSequenceNumber( medium->getMediumNumber());
  loadListFile.setMediaSetPn( medium->getPartNumber());
  loadListFile.setNumberOfMediaSetMembers(  medium->getMediaSet()->getNumberOfMedia());
#endif
  // export list of batches

  // export medium info

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

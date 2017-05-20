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

namespace Arinc665 {
namespace Utils {

MediaSetExporterImpl::MediaSetExporterImpl(
  Media::ConstMediaSetPtr mediaSet,
  Arinc665Utils::GetMediumPathHandler getMediumPathHandler,
  Arinc665Utils::CopyFileHandler copyFileHandler,
  bool createBatchFiles,
  bool createLoadHeaderFiles):
  mediaSet( mediaSet),
  getMediumPathHandler( getMediumPathHandler),
  copyFileHandler( copyFileHandler),
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
  auto mediumPath( getMediumPathHandler( medium->getMediumNumber()));

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export Medium to" << mediumPath;

  if (boost::filesystem::exists( mediumPath))
  {
   BOOST_THROW_EXCEPTION( Arinc665Exception() <<
     AdditionalInfo( "Medium directory must no exist"));
  }

  if (!boost::filesystem::create_directories( mediumPath))
  {
   BOOST_THROW_EXCEPTION( Arinc665Exception() <<
     AdditionalInfo( "Cannot create directory"));
  }

  // export sub-directories
  for ( auto directory : medium->getSubDirectories())
  {
    exportDirectory( mediumPath, directory);
  }

  // export files
  for ( auto file : medium->getFiles())
  {
    exportFile( mediumPath, file);
  }

  // export list of loads

  // export list of batches

  // export medium info

}

void MediaSetExporterImpl::exportDirectory(
  const path &base,
  Media::ConstDirectoryPtr directory)
{
  auto directoryPath( base / directory->getName());

  if (!boost::filesystem::create_directory( directoryPath))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Cannot create directory"));
  }

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export directory to" << directoryPath;

  // export sub-directories
  for ( auto directory : directory->getSubDirectories())
  {
    exportDirectory( directoryPath, directory);
  }

  // export files
  for ( auto file : directory->getFiles())
  {
    exportFile( directoryPath, file);
  }
}

void MediaSetExporterImpl::exportFile(
  const path &base,
  Media::ConstFilePtr file)
{
  auto filePath( base / file->getName());

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Export file to" << filePath;

  switch (file->getFileType())
  {
    case Media::File::FileType::RegularFile:
      copyFileHandler( file, filePath);
      break;

    case Media::File::FileType::LoadFile:
      if (createLoadHeaderFiles)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "Not implemented"));
      }
      copyFileHandler( file, filePath);
      break;

    case Media::File::FileType::BatchFile:
      if (createBatchFiles)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "Not implemented"));
      }
      copyFileHandler( file, filePath);
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Invalid file type"));
        /* no break: because THROW */
  }
}

}
}

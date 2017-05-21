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
 * @brief Definition of class Arinc665::File::FileFactory.
 **/

#include "FileFactory.hpp"

#include <arinc665/file/Arinc665File.hpp>

namespace Arinc665 {
namespace File {

Arinc665::FileClassType FileFactory::getArincFileType( const RawFile &file)
{
  const uint16_t formatVersion = Arinc665File::getFormatVersion( file);

  switch (formatVersion)
  {
    case static_cast< uint16_t>( LoadFileFormatVersion::Version2):
    case static_cast< uint16_t>( LoadFileFormatVersion::Version34):
      return FileClassType::LoadFile;

    case static_cast< uint16_t>( BatchFileFormatVersion::Version2):
    case static_cast< uint16_t>( BatchFileFormatVersion::Version34):
      return FileClassType::BatchFile;

    case static_cast< uint16_t>( MediaFileFormatVersion::Version2):
    case static_cast< uint16_t>( MediaFileFormatVersion::Version34):
      return FileClassType::MediaFile;

    default:
      return FileClassType::Invalid;
  }
}

Arinc665::LoadFileFormatVersion FileFactory::getLoadFileFormatVersion(
  const RawFile &file)
{
  const uint16_t formatVersion = Arinc665File::getFormatVersion( file);

  switch (formatVersion)
  {
    case static_cast< uint16_t>( LoadFileFormatVersion::Version2):
    case static_cast< uint16_t>( LoadFileFormatVersion::Version34):
      break;

    default:
      return LoadFileFormatVersion::Invalid;
  }

  return static_cast< LoadFileFormatVersion>( formatVersion);
}

Arinc665::BatchFileFormatVersion FileFactory::getBatchFileFormatVersion(
  const RawFile &file)
{
  const uint16_t formatVersion = Arinc665File::getFormatVersion( file);

  switch (formatVersion)
  {
    case static_cast< uint16_t>( BatchFileFormatVersion::Version2):
    case static_cast< uint16_t>( BatchFileFormatVersion::Version34):
      break;

    default:
      return BatchFileFormatVersion::Invalid;
  }

  return static_cast< BatchFileFormatVersion>( formatVersion);
}

Arinc665::MediaFileFormatVersion FileFactory::getMediaFileFormatVersion(
  const RawFile &file)
{
  const uint16_t formatVersion = Arinc665File::getFormatVersion( file);

  switch (formatVersion)
  {
    case static_cast< uint16_t>( MediaFileFormatVersion::Version2):
    case static_cast< uint16_t>( MediaFileFormatVersion::Version34):
      break;

    default:
      return MediaFileFormatVersion::Invalid;
  }

  return static_cast< MediaFileFormatVersion>( formatVersion);
}

Arinc665::FileType FileFactory::getFileType(
  const boost::filesystem::path &filename)
{
  std::string filenameN = filename.filename().string();

  if ( filenameN == ListOfLoadsName)
  {
    return FileType::LoadList;
  }

  if ( filenameN == ListOfBatchesName)
  {
    return FileType::BatchList;
  }

  if ( filenameN == ListOfFilesName)
  {
    return FileType::FileList;
  }

  std::string extension = filename.extension().string();

  if ( extension == LoadUploadHeaderExtension)
  {
    return FileType::LoadUploadHeader;
  }

  if ( extension == BatchFileExtension)
  {
    return FileType::BatchFile;
  }

  return FileType::Invalid;
}

FileListFile FileFactory::getFileListFile( const RawFile &file)
{
  return FileListFile( file);
}

LoadListFile FileFactory::getLoadListFile( const RawFile &file)
{
  return LoadListFile( file);
}

#if 0
BatchListFile FileFactory::getBatchListFile(
  const RawFile &file)
{
  return BatchListFile( file);
}
#endif

LoadHeaderFile FileFactory::getLoadHeaderFile( const RawFile &file)
{
  return LoadHeaderFile( file);
}

#if 0
BatchFile FileFactory::getBatchFile( const RawFile &file)
{
  return BatchFile( file);
}
#endif

}
}

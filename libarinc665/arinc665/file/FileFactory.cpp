/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class FileFactory
 **/

#include "FileFactory.hpp"

#include <arinc665/file/Arinc665File.hpp>

namespace Arinc665 {
namespace File {

Arinc665::FileClassType FileFactory::getArincFileType( const RawFile &file)
{
	uint16_t formatVersion = Arinc665File::getFormatVersion( file);

	switch (formatVersion)
	{
		case static_cast< uint16_t>( LoadFileFormatVersion::ARINC_665_LOAD_FILE_VERSION_2):
		case static_cast< uint16_t>( LoadFileFormatVersion::ARINC_665_LOAD_FILE_VERSION_3):
			return FileClassType::ARINC_665_FILE_CLASS_LOAD_FILE;

		case static_cast< uint16_t>( BatchFileFormatVersion::ARINC_665_BATCH_FILE_VERSION_2):
		case static_cast< uint16_t>( BatchFileFormatVersion::ARINC_665_BATCH_FILE_VERSION_3):
			return FileClassType::ARINC_665_FILE_CLASS_BATCH_FILE;

		case static_cast< uint16_t>( MediaFileFormatVersion::ARINC_665_MEDIA_FILE_VERSION_2):
		case static_cast< uint16_t>( MediaFileFormatVersion::ARINC_665_MEDIA_FILE_VERSION_3):
			return FileClassType::ARINC_665_FILE_CLASS_MEDIA_FILE;

		default:
			return FileClassType::ARINC_665_FILE_CLASS_INVALID;
	}
}

Arinc665::LoadFileFormatVersion FileFactory::getLoadFileFormatVersion(
	const RawFile &file)
{
	uint16_t formatVersion = Arinc665File::getFormatVersion( file);

	switch (formatVersion)
	{
		case static_cast< uint16_t>( LoadFileFormatVersion::ARINC_665_LOAD_FILE_VERSION_2):
		case static_cast< uint16_t>( LoadFileFormatVersion::ARINC_665_LOAD_FILE_VERSION_3):
			break;

		default:
			return LoadFileFormatVersion::ARINC_665_LOAD_FILE_INVALID;
	}

	return static_cast< LoadFileFormatVersion>( formatVersion);
}

Arinc665::BatchFileFormatVersion FileFactory::getBatchFileFormatVersion(
	const RawFile &file)
{
	uint16_t formatVersion = Arinc665File::getFormatVersion( file);

	switch (formatVersion)
	{
		case static_cast< uint16_t>( BatchFileFormatVersion::ARINC_665_BATCH_FILE_VERSION_2):
		case static_cast< uint16_t>( BatchFileFormatVersion::ARINC_665_BATCH_FILE_VERSION_3):
			break;

		default:
			return BatchFileFormatVersion::ARINC_665_BATCH_FILE_INVALID;
	}

	return static_cast< BatchFileFormatVersion>( formatVersion);
}

Arinc665::MediaFileFormatVersion FileFactory::getMediaFileFormatVersion(
	const RawFile &file)
{
	uint16_t formatVersion = Arinc665File::getFormatVersion( file);

	switch (formatVersion)
	{
		case static_cast< uint16_t>( MediaFileFormatVersion::ARINC_665_MEDIA_FILE_VERSION_2):
		case static_cast< uint16_t>( MediaFileFormatVersion::ARINC_665_MEDIA_FILE_VERSION_3):
			break;

		default:
			return MediaFileFormatVersion::ARINC_665_MEDIA_FILE_INVALID;
	}

	return static_cast< MediaFileFormatVersion>( formatVersion);
}

Arinc665::FileType FileFactory::getFileType(
  const boost::filesystem::path &filename)
{
  std::string filenameN = filename.filename().string();

  if ( filenameN == ListOfLoadsName)
  {
    return FileType::ARINC_665_FILE_TYPE_LOAD_LIST;
  }

  if ( filenameN == ListOfBatchesName)
  {
    return FileType::ARINC_665_FILE_TYPE_BATCH_LIST;
  }

  if ( filenameN == ListOfFilesName)
  {
    return FileType::ARINC_665_FILE_TYPE_FILE_LIST;
  }

  std::string extension = filename.extension().string();

  if ( extension == LoadUploadHeaderExtension)
  {
    return FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER;
  }

  if ( extension == BatchFileExtension)
  {
    return FileType::ARINC_665_FILE_TYPE_BATCH_FILE;
  }

  return FileType::ARINC_665_FILE_TYPE_INVALID;
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

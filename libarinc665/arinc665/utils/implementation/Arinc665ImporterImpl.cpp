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
 * @brief Definition of class Arinc665::Importer::Arinc665ImporterImpl.
 **/

#include "Arinc665ImporterImpl.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/file/FileFactory.hpp>

#include <helper/Logger.hpp>

#include <fstream>

namespace Arinc665 {
namespace Utils {

Arinc665ImporterImpl::MediaSetPtr Arinc665ImporterImpl::getMediaSet( void)
{
	return mediaSet;
}

void Arinc665ImporterImpl::import( GetMediumHandler getMediumHandler)
{
	path mediumPath = getMediumHandler( 1);

	if (!boost::filesystem::is_directory( mediumPath))
	{
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "Medium path does not exists"));
	}

	// Load list of files file
	FileListFile fileListFile( loadFile( mediumPath / Arinc665::ListOfFilesName));

	// create Media set
	mediaSet = std::make_shared< Media::MediaSet>(
		fileListFile.getMediaSetPn(),
		fileListFile.getNumberOfMediaSetMembers());

	// Add content of medium to media set
	addMedium( 1, mediumPath);

	// add additional media
	for (unsigned int mediaIndex = 2; mediaIndex < mediaSet->getNumberOfMedia(); ++mediaIndex)
	{
		mediumPath = getMediumHandler( mediaIndex);

	  if (!boost::filesystem::is_directory( mediumPath))
	  {
	    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
	      AdditionalInfo( "Medium path does not exists"));
	  }

	  addMedium( mediaIndex, mediumPath);
	}
}

void Arinc665ImporterImpl::addMedium( const unsigned int mediaIndex, const path &mediumPath)
{
	// Load list of files file
	//FileListFile fileListFile( loadFileListFile( mediaIndex, mediumPath / Arinc665::ListOfFilesName));

#if 0
	const FileListFile::FileListType &files = fileListFile.getFiles();
	FileListFile::ListType dataFiles;
	FileListFile::ListType loadFiles;
	FileListFile::ListType batchFiles;
	FileInfo listOfLoadsFile;
	FileInfo listOfBatchesFile;

	// iterate over all files
	for ( auto &file : files)
	{
	  using Arinc665::File::FileFactory;

		switch (FileFactory::getFileType( file.getFilename()))
		{
			case FileType::ARINC_665_FILE_TYPE_BATCH_FILE:
				batchFiles.push_back( file);
				break;

			case FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER:
				loadFiles.push_back( file);
				break;

			case FileType::ARINC_665_FILE_TYPE_LOAD_LIST:
				listOfLoadsFile = file;
				break;

			case FileType::ARINC_665_FILE_TYPE_BATCH_LIST:
				listOfBatchesFile = file;
				break;

			case FileType::ARINC_665_FILE_TYPE_FILE_LIST:
				BOOST_THROW_EXCEPTION( Arinc665Exception() <<
					AdditionalInfo( "FILES.LUM not expected in FILES.LUM"));
				break;

			default:
				// This is a data/ support file
				dataFiles.push_back( file);
				break;
		}
	}


	// iterate over load files
	for ( auto &loadHeaderFileIt : loadFiles)
	{
		path loadHeaderFilePath = mediumPath / loadHeaderFileIt.getFilename();

		//! @todo handle path name
		LoadHeaderFile loadHeaderFile( loadFile( loadHeaderFilePath));

		//! compare checksums (within header file and files.lum)
		if ( loadHeaderFile.getCrc() != loadHeaderFileIt.getCrc())
		{
			BOOST_THROW_EXCEPTION( Arinc665Exception() <<
				AdditionalInfo( "CRC of load header files invalid"));
		}

		//! add load
		Media::LoadPtr load = mediaSet->addLoad(
			loadHeaderFileIt.getMemberSequenceNumber(),
			loadHeaderFile.getPartNumber(),
			loadHeaderFileIt.getFilename(),
			loadHeaderFileIt.getPathName());

		load->setTargetHardwareIdList( loadHeaderFile.getTargetHardwareIdList());

		//! iterate over data files
		for ( auto &dataFile : loadHeaderFile.getDataFileList())
		{
			//! get file from global file list
			Media::FilePtr file = mediaSet->getFile( dataFile.getName());
			if (!file)
			{
				BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
					AdditionalInfo( "Data file not in file list"));
			}

			//! compare checksums
			if (file->getCrc() != dataFile.getCrc())
			{
				BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
					AdditionalInfo( "CRC in FILES.LUM and load header differs"));
			}

			//! @todo check file lenghts

			//! update part number of file
			file->setPartNumber( dataFile.getPartNumber());
			//! add file to load
			load->addDataFile( file);
		}

		//! iterate over support files
		for ( auto &supportFile : loadHeaderFile.getSupportFileList())
		{
			//! get file from global file list
			Media::FilePtr file = mediaSet->getFile( supportFile.getName());
			if (!file)
			{
				BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
					AdditionalInfo( "Support file not in file list"));
			}

			//! compare checksums
			if (file->getCrc() != supportFile.getCrc())
			{
				BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
					AdditionalInfo( "CRC in FILES.LUM and load header differs"));
			}

			//! @todo check file lengths

			//! update part number of file
			file->setPartNumber( supportFile.getPartNumber());
			//! add file to load
			load->addSupportFile( file);
		}

		//! add user defined data part
		load->setUserDefinedData( loadHeaderFile.getUserDefinedData());
	}
#endif
}

void Arinc665ImporterImpl::loadFileListFile( const unsigned int mediaIndex, const path &mediumPath)
{
  // Load list of files file
  FileListFile fileListFile( loadFile( mediumPath / Arinc665::ListOfFilesName));

  if (!this->fileListFile)
  {
    this->fileListFile = fileListFile;
  }

  // check for consistency
  if ((mediaSet->getPartNumber() != fileListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != fileListFile.getNumberOfMediaSetMembers()) ||
      (fileListFile.getMediaSequenceNumber() != mediaIndex))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Medium is not consistent to media set"));
  }

  if (this->fileListFile.get().belongsToSameMediaSet( fileListFile))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Medium is not consistent to media set"));
  }

  // iterate over data files
  for ( auto &file : fileListFile.getFiles())
  {
    if (file.getMemberSequenceNumber() != mediaIndex)
    {
      continue;
    }

    path dataFilePath = mediumPath / file.getPathName() / file.getFilename();

    RawFile rawFile( loadFile( dataFilePath));

    uint16_t crc = File::Arinc665File::calculateChecksum( rawFile, 0);

    // compare checksums
    if (crc != file.getCrc())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "CRC of files invalid"));
    }
  }
}

void Arinc665ImporterImpl::loadLoadListFile( const unsigned int mediaIndex, const path &mediumPath)
{

}

void Arinc665ImporterImpl::loadBatchListFile( const unsigned int mediaIndex, const path &mediumPath)
{
  if (!boost::filesystem::is_regular( mediumPath / Arinc665::ListOfBatchesName))
  {
    if (mediaIndex == 1)
    {
      return;
    }

    if (batchListFile)
    {
      return;
    }
    // List Batches Files was present on first medium --> if no missing, error
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "Medium is not consistent to media set"));
  }


}

Arinc665::File::RawFile Arinc665ImporterImpl::loadFile( const path &filePath)
{
  if (!boost::filesystem::is_regular( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665Exception() << AdditionalInfo( filePath.string() + " not found"));
  }

  RawFile data( boost::filesystem::file_size( filePath));

	std::ifstream file(
		filePath.string().c_str(),
		std::ifstream::binary | std::ifstream::in);

	if (!file.is_open())
	{
    //! @throw Arinc665Exception
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "Error opening files"));
	}

	// read the data to the buffer
	file.read( (char*)&data.at(0), data.size());

	// return the buffer
	return data;
}

}
}

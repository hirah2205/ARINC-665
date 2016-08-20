/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#include "Arinc665Importer.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/file/FileFactory.hpp>

#include <fstream>

using namespace Arinc665::Importer;

using Arinc665::File::FileFactory;
using Arinc665::File::FileListFile;
using Arinc665::File::LoadListFile;
using Arinc665::File::BatchListFile;
using Arinc665::File::LoadHeaderFile;
using Arinc665::File::FileInfo;

MediaSetPtr Arinc665Importer::getMediaSet( void)
{
	return mediaSet;
}

void Arinc665Importer::import( GetMediumHandler getMediumHandler)
{
	path mediumPath = getMediumHandler( 1);

	if (!boost::filesystem::is_directory( mediumPath))
	{
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "Medium path does not exists"));
	}

	// Load list of files file
	path listOfFilesPath = mediumPath / Arinc665::ListOfFilesName;

	// check for existence of list of loads file
	if (!boost::filesystem::is_regular( listOfFilesPath))
	{
		BOOST_THROW_EXCEPTION( InvalidArinc665File() <<
			AdditionalInfo( listOfFilesPath.string() + std::string( " not found")));
	}

	FileListFile fileListFile( loadFile( listOfFilesPath));

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

		addMedium( mediaIndex, mediumPath);
	}
}

void Arinc665Importer::addMedium( const unsigned int mediaIndex, const path &mediumPath)
{
	if (!boost::filesystem::is_directory( mediumPath))
	{
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "Medium path does not exists"));
	}

	// Load list of files file
	path listOfFilesPath = mediumPath / Arinc665::ListOfFilesName;

	// check for existence of list of loads file
	if (!boost::filesystem::is_regular( listOfFilesPath))
	{
		BOOST_THROW_EXCEPTION( InvalidArinc665File() <<
			AdditionalInfo( Arinc665::ListOfFilesName + std::string( " not found")));
	}

	FileListFile fileListFile( loadFile( listOfFilesPath));

	// check for consistency
	if ((mediaSet->getPartNumber() != fileListFile.getMediaSetPn()) ||
			(mediaSet->getNumberOfMedia() != fileListFile.getNumberOfMediaSetMembers()) ||
			(fileListFile.getMediaSequenceNumber() != mediaIndex))
	{
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "Medium is not consistent to media set"));
	}

	const FileListFile::ListType &files = fileListFile.getFiles();
	FileListFile::ListType dataFiles;
	FileListFile::ListType loadFiles;
	FileListFile::ListType batchFiles;
	FileInfo listOfLoadsFile;
	FileInfo listOfBatchesFile;

	// iterate over all files
	for ( auto &file : files)
	{
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

	// iterate over data files
	for ( auto &dataFile : dataFiles)
	{
		path dataFilePath = mediumPath / dataFile.getFilename();

		RawFile rawFile( loadFile( dataFilePath));

		uint16_t crc = Arinc665File::calculateChecksum( rawFile, 0);

		// compare checksums
		if (crc != dataFile.getCrc())
		{
			BOOST_THROW_EXCEPTION( Arinc665Exception() <<
				AdditionalInfo( "CRC of files invalid"));
		}

		// add data file
		mediaSet->getMedium( dataFile.getMemberSequenceNumber())->addFile(
			dataFile.getFilename(),
			dataFile.getCrc());
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
}

Arinc665::File::RawFile Arinc665Importer::loadFile( const path &filePath)
{
	RawFile data( boost::filesystem::file_size( filePath));

	std::ifstream file(
		filePath.string().c_str(),
		std::ifstream::binary | std::ifstream::in);

	if (!file.is_open())
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "Error opening files"));
	}

	//! read the data to the buffer
	file.read( (char*)&data.at(0), data.size());

	//! return the buffer
	return data;
}

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
#include <arinc665/media/Directory.hpp>

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
	      AdditionalInfo( mediumPath.string() + " is not a valid medium path"));
	  }

	  addMedium( mediaIndex, mediumPath);
	}
}

void Arinc665ImporterImpl::addMedium( const unsigned int mediaIndex, const path &mediumPath)
{
  loadFileListFile( mediaIndex, mediumPath);
  loadLoadListFile( mediaIndex, mediumPath);
  loadBatchListFile( mediaIndex, mediumPath);
  loadLoadHeaderFiles( mediaIndex, mediumPath);
  loadBatchFiles( mediaIndex, mediumPath);
  addFiles();

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
    files = fileListFile.getFileMap();
  }

  // check for consistency
  if ((mediaSet->getPartNumber() != fileListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != fileListFile.getNumberOfMediaSetMembers()) ||
      (mediaIndex != fileListFile.getMediaSequenceNumber()))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfFilesName).string() +
        " is not consistent to media set"));
  }

  if (!this->fileListFile.get().belongsToSameMediaSet( fileListFile))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfFilesName).string() +
        " is not consistent to other file list"));
  }

  // iterate over files
  for ( auto &file : fileListFile.getFiles())
  {
    if (file.getMemberSequenceNumber() != mediaIndex)
    {
      continue;
    }

    path dataFilePath( mediumPath / file.getPath());

    RawFile rawFile( loadFile( dataFilePath));

    uint16_t crc = File::Arinc665File::calculateChecksum( rawFile, 0);

    // compare checksums
    if (crc != file.getCrc())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          (dataFilePath).string() +
            ": CRC of file invalid"));
    }
  }
}

void Arinc665ImporterImpl::loadLoadListFile( const unsigned int mediaIndex, const path &mediumPath)
{
  // Load list of loads file
  LoadListFile loadListFile( loadFile( mediumPath / Arinc665::ListOfLoadsName));

  if (!this->loadListFile)
  {
    this->loadListFile = loadListFile;
    loads = loadListFile.getLoadMap();
  }

  // check for consistency
  if ((mediaSet->getPartNumber() != loadListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != loadListFile.getNumberOfMediaSetMembers()) ||
      (loadListFile.getMediaSequenceNumber() != mediaIndex))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfLoadsName).string() +
        " is not consistent to media set"));
  }

  if (!this->loadListFile.get().belongsToSameMediaSet( loadListFile))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfLoadsName).string() +
        " is not consistent to other loads list"));
  }

  if (1 == mediaIndex)
  {
    // iterate over all loads
    for ( auto &load : loadListFile.getLoads())
    {
      // check existence of load header files.

      FileListFile::FileMap::iterator file =
        files.find( std::make_pair(
          load.getMemberSequenceNumber(),
          load.getHeaderFilename()));

      if (file == files.end())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( load.getHeaderFilename() + ": load header file not found"));
      }
    }
  }
}

void Arinc665ImporterImpl::loadBatchListFile( const unsigned int mediaIndex, const path &mediumPath)
{
  // Check for optional batch list file
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
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfBatchesName).string() +
        " expected but not existing"));
  }

  // Load list of batches file
  BatchListFile batchListFile( loadFile( mediumPath / Arinc665::ListOfBatchesName));

  if (!this->batchListFile)
  {
    this->batchListFile = batchListFile;
  }

  // check for consistency
  if ((mediaSet->getPartNumber() != batchListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != batchListFile.getNumberOfMediaSetMembers()) ||
      (batchListFile.getMediaSequenceNumber() != mediaIndex))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
       AdditionalInfo(
         (mediumPath / Arinc665::ListOfBatchesName).string() +
         " is not consistent to media set"));
  }

  if (!this->batchListFile.get().belongsToSameMediaSet( batchListFile))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfBatchesName).string() +
        " is not consistent to other batches list"));
  }

  if (1 == mediaIndex)
  {
    // iterate over all batches
    for ( auto &batch : batchListFile.getBatches())
    {
      FileListFile::FileMap::iterator file =
        files.find( std::make_pair(
          batch.getMemberSequenceNumber(),
          batch.getFilename()));

      if (file == files.end())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( batch.getFilename() + ": batch file not found"));
      }
    }
  }
}

void Arinc665ImporterImpl::loadLoadHeaderFiles( const unsigned int mediaIndex, const path &mediumPath)
{
  for ( const auto &load : loads)
  {
    if ( load.first.first != mediaIndex)
    {
      continue;
    }

    FileListFile::FileMap::const_iterator loadHeaderFileIt(
      files.find( load.first));

    if (files.end() == loadHeaderFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    LoadHeaderFile loadHeaderFile(
      loadFile(
        mediumPath /
        loadHeaderFileIt->second.getPath()));

    if (loadHeaderFile.getPartNumber() != load.second.getPartNumber())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    loadHeaderFiles.insert(
      std::make_pair( loadHeaderFileIt->second.getFilename(), loadHeaderFile));
  }
}

void Arinc665ImporterImpl::loadBatchFiles( const unsigned int mediaIndex, const path &mediumPath)
{
  for ( const auto &batch : batches)
  {
    if ( batch.first.first != mediaIndex)
    {
      continue;
    }

    FileListFile::FileMap::const_iterator batchFileIt(
      files.find( batch.first));

    if (files.end() == batchFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    BatchFile batchFile(
      loadFile(
        mediumPath /
        batchFileIt->second.getPathName() /
        batchFileIt->second.getFilename()));

    if (batchFile.getPartNumber() != batch.second.getPartNumber())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    batchFiles.insert(
      std::make_pair( batchFileIt->second.getFilename(), batchFile));
  }
}

void Arinc665ImporterImpl::addFiles( void)
{
  FileListFile::FileMap loadHeaders;
  FileListFile::FileMap batches;

  // add not load headers and batch files
  for ( const auto &file : files)
  {
    Arinc665::FileType fileType( Arinc665::File::FileFactory::getFileType( file.first.second));

    switch ( fileType)
    {
      case Arinc665::FileType::ARINC_665_FILE_TYPE_FILE_LIST:
      case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_LIST:
      case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_LIST:
        continue;

      case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER:
        loadHeaders.insert( file);
        continue;

      case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_FILE:
        batches.insert( file);
        continue;

      default:
        break;
    }

    ContainerEntityPtr container(
      checkCreateDirectory( file.first.first, file.second.getPath()));

    container->addFile(
      file.second.getFilename(),
      file.second.getCrc(),
      0);
  }

  // loads
  for ( const auto &loadHeader : loadHeaders)
  {
    LoadListFile::LoadMap::const_iterator load = loads.find( loadHeader.first);
    LoadHeaderFiles::const_iterator loadHeaderFile = loadHeaderFiles.find( loadHeader.first.second);

    ContainerEntityPtr container(
      checkCreateDirectory( loadHeader.first.first, loadHeader.second.getPath()));

    Arinc665::Media::LoadPtr loadPtr =
      container->addLoad( loadHeader.second.getFilename(), load->second.getPartNumber());

    // iterate over data files
    for ( const auto &dataFile : loadHeaderFile->second.getDataFileList())
    {
      Arinc665::Media::FilePtr dataFilePtr = mediaSet->getFile( dataFile.getName());

      dataFilePtr->setPartNumber( dataFile.getPartNumber());

      loadPtr->addDataFile( dataFilePtr);
    }

    // iterate over data files
    for ( const auto &supportFile : loadHeaderFile->second.getSupportFileList())
    {
      Arinc665::Media::FilePtr supportFilePtr = mediaSet->getFile( supportFile.getName());

      loadPtr->addSupportFile( supportFilePtr);
    }
  }

  // Batches
  for ( const auto &batch : batches)
  {
    BatchListFile::BatchMap::const_iterator batchInfo = this->batches.find( batch.first);
    BatchFiles::const_iterator batchFile = batchFiles.find( batch.first.second);

    ContainerEntityPtr container(
      checkCreateDirectory( batch.first.first, batch.second.getPath()));

    Arinc665::Media::LoadPtr loadPtr =
      container->addLoad( batch.second.getFilename(), batchInfo->second.getPartNumber());

    // iterate over loads
    for ( const auto &load : batchFile->second.getTargetHardwareLoadList())
    {
    }
  }

}

Arinc665ImporterImpl::ContainerEntityPtr Arinc665ImporterImpl::checkCreateDirectory( const unsigned int mediaIndex, const path &mediumPath)
{
  path dirPath( mediumPath.parent_path());

  Arinc665::Media::MediumPtr medium( mediaSet->getMedium( mediaIndex));

  if ( dirPath == "/")
  {
    return medium;
  }

  ContainerEntityPtr dir = medium;

  for ( path::iterator it = ++dirPath.begin(); it != dirPath.end(); ++it)
  {
    dir = dir->getSubDirectory( it->string());
  }

  return dir;
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

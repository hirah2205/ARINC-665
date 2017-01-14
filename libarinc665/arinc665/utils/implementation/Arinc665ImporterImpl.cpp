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
 * @brief Definition of class Arinc665::Utils::Arinc665ImporterImpl.
 **/

#include "Arinc665ImporterImpl.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/file/FileFactory.hpp>
#include <arinc665/media/Directory.hpp>

#include <helper/Logger.hpp>

#include <fstream>

namespace Arinc665 {
namespace Utils {

Arinc665ImporterImpl::Arinc665ImporterImpl(
  Arinc665Utils::GetMediumPathHandler getMediumPathHandler):
  getMediumPathHandler( getMediumPathHandler)
{
}

Arinc665ImporterImpl::MediaSetPtr Arinc665ImporterImpl::operator ()()
{
  path mediumPath = getMediumPathHandler( 1);

  if ( !boost::filesystem::is_directory( mediumPath))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception() << AdditionalInfo( "Medium path does not exists"));
  }

  // Load list of files file
  FileListFile fileListFile( loadFile( mediumPath / Arinc665::ListOfFilesName));

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>( fileListFile.getMediaSetPn());
  mediaSet->setNumberOfMedia( fileListFile.getNumberOfMediaSetMembers());

  // Add content of medium to media set
  addMedium( 1, mediumPath);

  // add additional media
  for ( unsigned int mediaIndex = 2; mediaIndex <= mediaSet->getNumberOfMedia();
    ++mediaIndex)
  {
    mediumPath = getMediumPathHandler( mediaIndex);

    if ( !boost::filesystem::is_directory( mediumPath))
    {
      BOOST_THROW_EXCEPTION(
        Arinc665Exception()
          << AdditionalInfo(
            mediumPath.string() + " is not a valid medium path"));
    }

    addMedium( mediaIndex, mediumPath);
  }

  return mediaSet;
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

void Arinc665ImporterImpl::loadFileListFile(
  const unsigned int mediaIndex,
  const path &mediumPath)
{
  // Load list of files file
  FileListFile fileListFile( loadFile( mediumPath / Arinc665::ListOfFilesName));

  // store first list of files for further tests
  if (!this->fileListFile)
  {
    this->fileListFile = fileListFile;
    fileInfos = fileListFile.getFileInfosAsMap();
  }

  // check for consistency of current file list file
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
  for ( auto &fileInfo : fileInfos)
  {
    // skip files, which are not part of the current medium
    if (fileInfo.second.getMemberSequenceNumber() != mediaIndex)
    {
      continue;
    }

    path dataFilePath( mediumPath / fileInfo.second.getPath());

    RawFile rawFile( loadFile( dataFilePath));

    uint16_t crc = File::Arinc665File::calculateChecksum( rawFile, 0);

    // compare checksums
    if (crc != fileInfo.second.getCrc())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          (dataFilePath).string() +
            ": CRC of file invalid"));
    }
  }
}

void Arinc665ImporterImpl::loadLoadListFile(
  const unsigned int mediaIndex,
  const path &mediumPath)
{
  // Load list of loads file
  LoadListFile loadListFile( loadFile( mediumPath / Arinc665::ListOfLoadsName));

  // store first list of loads for further tests
  if (!this->loadListFile)
  {
    this->loadListFile = loadListFile;
    loadInfos = loadListFile.getLoadInfosAsMap();
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

  // only the load list of the first media must be checked in detail
  if (1 == mediaIndex)
  {
    // iterate over all loads
    for ( auto &loadInfo : loadInfos)
    {
      // check existence of load header files.

      FileListFile::FileInfoMap::iterator file =
        fileInfos.find( loadInfo.first);

      if (file == fileInfos.end())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( loadInfo.first.second + ": load header file not found"));
      }

      if (loadInfo.second != file->second)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "data inconsistency"));
      }
    }
  }
}

void Arinc665ImporterImpl::loadBatchListFile(
  const unsigned int mediaIndex,
  const path &mediumPath)
{
  // Check for optional batch list file
  if (!boost::filesystem::is_regular( mediumPath / Arinc665::ListOfBatchesName))
  {
    // if not existent and we are the first media
    if (mediaIndex == 1)
    {
      return;
    }

    if (!batchListFile)
    {
      // batch file must be present on EVERY media if found on first one.
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
    this->batchInfos = batchListFile.getBatchInfosAsMap();
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

  // only the batch list of the first media must be checked in detail
  if (1 == mediaIndex)
  {
    // iterate over all batches
    for ( auto &batchInfo : batchInfos)
    {
      FileListFile::FileInfoMap::iterator file = fileInfos.find( batchInfo.first);

      if (file == fileInfos.end())
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( batchInfo.first.second + ": batch file not found"));
      }

      if (batchInfo.second != file->second)
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "File inconsistency"));
      }
    }
  }
}

void Arinc665ImporterImpl::loadLoadHeaderFiles(
  const unsigned int mediaIndex,
  const path &mediumPath)
{
  for ( const auto &loadInfo : loadInfos)
  {
    // skip files, which are not present on current media
    if ( loadInfo.first.first != mediaIndex)
    {
      continue;
    }

    FileListFile::FileInfoMap::const_iterator loadHeaderFileIt(
      fileInfos.find( loadInfo.first));

    if (fileInfos.end() == loadHeaderFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    LoadHeaderFile loadHeaderFile(
      loadFile(
        mediumPath /
        loadHeaderFileIt->second.getPath()));

    if (loadHeaderFile.getPartNumber() != loadInfo.second.getPartNumber())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    if (loadHeaderFile.getTargetHardwareIdList() != loadInfo.second.getTargetHardwareIdList())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    loadHeaderFiles.insert(
      std::make_pair( loadHeaderFileIt->second.getFilename(), loadHeaderFile));
  }
}

void Arinc665ImporterImpl::loadBatchFiles(
  const unsigned int mediaIndex,
  const path &mediumPath)
{
  for ( const auto &batchInfo : batchInfos)
  {
    // Skip batch files not located on this medium
    if ( batchInfo.first.first != mediaIndex)
    {
      continue;
    }

    FileListFile::FileInfoMap::const_iterator batchFileIt(
      fileInfos.find( batchInfo.first));

    if (fileInfos.end() == batchFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    BatchFile batchFile(
      loadFile(
        mediumPath /
        batchFileIt->second.getPathName() /
        batchFileIt->second.getFilename()));

    if (batchFile.getPartNumber() != batchInfo.second.getPartNumber())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    batchFiles.insert(
      std::make_pair( batchFileIt->second.getFilename(), batchFile));
  }
}

void Arinc665ImporterImpl::addFiles()
{
  FileListFile::FileInfoMap loadHeaders;
  FileListFile::FileInfoMap batches;

  // skip list files and handle load headers and batch files separate
  for ( const auto &fileInfo : fileInfos)
  {
    // get file type
    Arinc665::FileType fileType(
      Arinc665::File::FileFactory::getFileType( fileInfo.first.second));

    switch ( fileType)
    {
      case Arinc665::FileType::ARINC_665_FILE_TYPE_FILE_LIST:
      case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_LIST:
      case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_LIST:
        // List files are ignored
        continue;

      case Arinc665::FileType::ARINC_665_FILE_TYPE_LOAD_UPLOAD_HEADER:
        // load header files are handled later - all regular files must be loaded first
        loadHeaders.insert( fileInfo);
        continue;

      case Arinc665::FileType::ARINC_665_FILE_TYPE_BATCH_FILE:
        // batch files are handled later - all load header files must be loaded first
        batches.insert( fileInfo);
        continue;

      default:
        break;
    }

    // get directory, where file will be placed into.
    ContainerEntityPtr container(
      checkCreateDirectory( fileInfo.first.first, fileInfo.second.getPath()));

    // place file
    Arinc665::Media::FilePtr filePtr = container->addFile(
      fileInfo.second.getFilename());
  }

  // loads
  addLoads( loadHeaders);

  // Batches
  addBatches( batches);
}

void Arinc665ImporterImpl::addLoads( FileListFile::FileInfoMap &loadHeaders)
{
  for ( const auto &loadHeader : loadHeaders)
  {
    LoadListFile::LoadInfoMap::const_iterator load =
      loadInfos.find( loadHeader.first);
    LoadHeaderFiles::const_iterator loadHeaderFile =
      loadHeaderFiles.find( loadHeader.first.second);

    ContainerEntityPtr container(
      checkCreateDirectory( loadHeader.first.first, loadHeader.second.getPath()));

    Arinc665::Media::LoadPtr loadPtr(
      container->addLoad( loadHeader.second.getFilename()));

    loadPtr->setPartNumber( load->second.getPartNumber());
    loadPtr->setTargetHardwareIdList( load->second.getTargetHardwareIdList());

    // iterate over data files
    for ( const auto &dataFile : loadHeaderFile->second.getDataFileList())
    {
      Arinc665::Media::FilePtr dataFilePtr = mediaSet->getFile( dataFile.getName());

      dataFilePtr->setPartNumber( dataFile.getPartNumber());

      loadPtr->addDataFile( dataFilePtr);
    }

    // iterate over support files
    for ( const auto &supportFile : loadHeaderFile->second.getSupportFileList())
    {
      Arinc665::Media::FilePtr supportFilePtr = mediaSet->getFile( supportFile.getName());

      loadPtr->addSupportFile( supportFilePtr);
    }
  }
}

void Arinc665ImporterImpl::addBatches( FileListFile::FileInfoMap &batches)
{
  for ( const auto &batch : batches)
  {
    // get batch info for batch file
    BatchListFile::BatchInfoMap::const_iterator batchInfo =
      this->batchInfos.find( batch.first);

    // get the batch file
    BatchFiles::const_iterator batchFile = batchFiles.find( batch.first.second);

    ContainerEntityPtr container(
      checkCreateDirectory( batch.first.first, batch.second.getPath()));

    Arinc665::Media::BatchPtr batchPtr( container->addBatch( batch.second.getFilename()));

    batchPtr->setPartNumber( batchInfo->second.getPartNumber());

    // iterate over loads
#if 0
    for ( const auto &load : batchFile->second.getTargetHardwareLoadList())
    {
    }
#endif
  }
}

Arinc665ImporterImpl::ContainerEntityPtr
Arinc665ImporterImpl::checkCreateDirectory(
  const unsigned int mediaIndex,
  const path &filePath)
{
  path dirPath( filePath.parent_path());

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
      Arinc665Exception() <<
        boost::errinfo_file_name( filePath.c_str()) <<
        AdditionalInfo( "File not found"));
  }

  RawFile data( boost::filesystem::file_size( filePath));

  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Error opening files"));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

}
}

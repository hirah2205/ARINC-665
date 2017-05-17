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
 * @brief Definition of class Arinc665::Utils::MediaSetImporterImpl.
 **/

#include "MediaSetImporterImpl.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/file/FileFactory.hpp>

#include <arinc665/media/Directory.hpp>

#include <fstream>

namespace Arinc665 {
namespace Utils {

MediaSetImporterImpl::MediaSetImporterImpl(
  Arinc665Utils::GetMediumPathHandler getMediumPathHandler):
  getMediumPathHandler( getMediumPathHandler)
{
}

Media::MediaSetPtr MediaSetImporterImpl::operator()( const string &mediaSetName)
{
  BOOST_LOG_FUNCTION();

  // path for first medium
  path mediumPath( getMediumPathHandler( 1));

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Medium 1: " << mediumPath;


  // check medium path
  if ( !boost::filesystem::is_directory( mediumPath))
  {
    //! @throw Arinc665Exception if medium path does not exist
    BOOST_THROW_EXCEPTION(
      Arinc665Exception() << AdditionalInfo( "Medium path does not exists"));
  }

  // Load list of files file
  FileListFile fileListFile( loadFile( mediumPath / Arinc665::ListOfFilesName));

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>( mediaSetName);
  mediaSet->setPartNumber( fileListFile.getMediaSetPn());
  mediaSet->setNumberOfMedia( fileListFile.getNumberOfMediaSetMembers());

  // Add content of medium to media set
  addMedium( 1, mediumPath);

  // iterate over additional media
  for ( unsigned int mediaIndex = 2; mediaIndex <= mediaSet->getNumberOfMedia();
    ++mediaIndex)
  {
    // get medium path
    mediumPath = getMediumPathHandler( mediaIndex);

    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Medium " << mediaIndex << ": " << mediumPath;

    // check medium path
    if ( !boost::filesystem::is_directory( mediumPath))
    {
      //! @throw Arinc665Exception if medium path does not exist
      BOOST_THROW_EXCEPTION( Arinc665Exception() << AdditionalInfo(
        mediumPath.string() + " is not a valid medium path"));
    }

    addMedium( mediaIndex, mediumPath);
  }

  return mediaSet;
}

void MediaSetImporterImpl::addMedium(
  const uint8_t mediumIndex,
  const path &mediumPath)
{
  loadFileListFile( mediumIndex, mediumPath);
  loadLoadListFile( mediumIndex, mediumPath);
  loadBatchListFile( mediumIndex, mediumPath);
  loadLoadHeaderFiles( mediumIndex, mediumPath);
  loadBatchFiles( mediumIndex, mediumPath);
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

void MediaSetImporterImpl::loadFileListFile(
  const uint8_t mediumIndex,
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
  else
  {
    // otherwise compare current list of files to first one
    if (!this->fileListFile.get().belongsToSameMediaSet( fileListFile))
    {
      //! @throw Arinc665Exception When FILES.LUM is inconsistent to other media
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          (mediumPath / Arinc665::ListOfFilesName).string() +
          " is not consistent to other file list"));
    }
  }

  // check for consistency of current file list file to media set
  if ((mediaSet->getPartNumber() != fileListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != fileListFile.getNumberOfMediaSetMembers()) ||
      (mediumIndex != fileListFile.getMediaSequenceNumber()))
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
     "media set: " <<
       mediaSet->getPartNumber() << " " <<
       (int)mediaSet->getNumberOfMedia() << " " <<
       (int)mediumIndex << " // " <<
       fileListFile.getMediaSetPn() << " " <<
       (int)fileListFile.getNumberOfMediaSetMembers() << " " <<
       (int)fileListFile.getMediaSequenceNumber();

    //! @throw Arinc665Exception When FILES.LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfFilesName).string() +
        " is not consistent to media set"));
  }

  // iterate over files
  for ( auto &fileInfo : fileInfos)
  {
    // skip files, which are not part of the current medium
    if (fileInfo.second.getMemberSequenceNumber() != mediumIndex)
    {
      continue;
    }

    path dataFilePath( mediumPath / fileInfo.second.getPath());

    RawFile rawFile( loadFile( dataFilePath));

    uint16_t crc = File::Arinc665File::calculateChecksum( rawFile, 0);

    // compare checksums
    if (crc != fileInfo.second.getCrc())
    {
      //! @throw Arinc665Exception When file CRCs does not match
      BOOST_THROW_EXCEPTION( Arinc665Exception() << AdditionalInfo(
        (dataFilePath).string() + ": CRC of file invalid"));
    }

    // add file path mapping

  }
}

void MediaSetImporterImpl::loadLoadListFile(
  const uint8_t mediumIndex,
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
  else
  {
    // otherwise check against stored version
    if (!this->loadListFile.get().belongsToSameMediaSet( loadListFile))
    {
      //! @throw Arinc665Exception When LOADS.LUM is inconsistent to other media
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          (mediumPath / Arinc665::ListOfLoadsName).string() +
          " is not consistent to other loads list"));
    }
  }

  // check for consistency of current file list file to media set
  if ((mediaSet->getPartNumber() != loadListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != loadListFile.getNumberOfMediaSetMembers()) ||
      (loadListFile.getMediaSequenceNumber() != mediumIndex))
  {
    //! @throw Arinc665Exception When LOADS:LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        (mediumPath / Arinc665::ListOfLoadsName).string() +
        " is not consistent to media set"));
  }

  // only the load list of the first media must be checked in detail
  if (1U == mediumIndex)
  {
    // iterate over all loads
    for ( auto &loadInfo : loadInfos)
    {
      // check existence of load header files.

      FileListFile::FileInfoMap::iterator file(
        fileInfos.find( loadInfo.first));

      if (file == fileInfos.end())
      {
        //! @throw Arinc665Exception When load header is not found.
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( loadInfo.first.second + ": load header file not found"));
      }

      // checks that the load list and file list entry maps to the same file entry
      if (loadInfo.second != file->second)
      {
        //! @throw Arinc665Exception When load file and list file reference does not match
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "data inconsistency"));
      }
    }
  }
}

void MediaSetImporterImpl::loadBatchListFile(
  const uint8_t mediumIndex,
  const path &mediumPath)
{
  // Check for optional batch list file
  if (!boost::filesystem::is_regular( mediumPath / Arinc665::ListOfBatchesName))
  {
    // if not existent and we are the first media
    if (mediumIndex == 1)
    {
      return;
    }

    if (!batchListFile)
    {
      // batch file must be present on EVERY media if found on first one.
      return;
    }

    //! @throw Arinc665Exception When List Batches Files was present on first medium but not on following
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
  else
  {
    // otherwise check against stored version
    if (!this->batchListFile.get().belongsToSameMediaSet( batchListFile))
    {
      //! @throw Arinc665Exception When BATCHES.LUM is inconsistent to other media
     BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          (mediumPath / Arinc665::ListOfBatchesName).string() +
          " is not consistent to other batches list"));
    }
  }

  // check for consistency of current batch list file to media set
  if ((mediaSet->getPartNumber() != batchListFile.getMediaSetPn()) ||
      (mediaSet->getNumberOfMedia() != batchListFile.getNumberOfMediaSetMembers()) ||
      (batchListFile.getMediaSequenceNumber() != mediumIndex))
  {
    //! @throw Arinc665Exception When FILES.LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
       AdditionalInfo(
         (mediumPath / Arinc665::ListOfBatchesName).string() +
         " is not consistent to media set"));
  }


  // only the batch list of the first media must be checked in detail
  if (1 == mediumIndex)
  {
    // iterate over all batches
    for ( auto &batchInfo : batchInfos)
    {
      FileListFile::FileInfoMap::iterator file = fileInfos.find( batchInfo.first);

      if (file == fileInfos.end())
      {
        //! @throw Arinc665Exception When batch file is not found.
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( batchInfo.first.second + ": batch file not found"));
      }

      // checks that the batch list and file list entry maps to the same file entry
      if (batchInfo.second != file->second)
      {
        //! @throw Arinc665Exception When batch list file and list file reference does not match
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "File inconsistency"));
      }
    }
  }
}

void MediaSetImporterImpl::loadLoadHeaderFiles(
  const uint8_t mediumIndex,
  const path &mediumPath)
{
  for ( const auto &loadInfo : loadInfos)
  {
    // skip load headers, which are not present on current medium
    if ( loadInfo.first.first != mediumIndex)
    {
      continue;
    }

    // find load header in file list
    FileListFile::FileInfoMap::const_iterator loadHeaderFileIt(
      fileInfos.find( loadInfo.first));

    // Check existence
    if (fileInfos.end() == loadHeaderFileIt)
    {
      //! @throw Arinc665Exception When load header has not been found in file list
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Load header file not found in file list"));
    }

    // decode load header
    LoadHeaderFile loadHeaderFile(
      loadFile(
        mediumPath /
        loadHeaderFileIt->second.getPath()));

    // validate load header part number info with the stored information in load list
    if (loadHeaderFile.getPartNumber() != loadInfo.second.getPartNumber())
    {
      //! @throw Arinc665Exception When load header PN info is inconsistent to list of loads information
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    // validate load header target HW list info with the stored information in load list
    if (loadHeaderFile.getTargetHardwareIdList() != loadInfo.second.getTargetHardwareIdList())
    {
      //! @throw Arinc665Exception When load header THW info is inconsistent to list of loads information
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    // add load header to global information
    loadHeaderFiles.insert(
      std::make_pair( loadHeaderFileIt->second.getFilename(), loadHeaderFile));
  }
}

void MediaSetImporterImpl::loadBatchFiles(
  const uint8_t mediumIndex,
  const path &mediumPath)
{
  for ( const auto &batchInfo : batchInfos)
  {
    // Skip batch files not located on this medium
    if ( batchInfo.first.first != mediumIndex)
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

void MediaSetImporterImpl::addFiles()
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
      case Arinc665::FileType::FileList:
      case Arinc665::FileType::LoadList:
      case Arinc665::FileType::BatchList:
        // List files are ignored
        continue;

      case Arinc665::FileType::LoadUploadHeader:
        // load header files are handled later - all regular files must be loaded first
        loadHeaders.insert( fileInfo);
        continue;

      case Arinc665::FileType::BatchFile:
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

  // Loads
  addLoads( loadHeaders);

  // Batches
  addBatches( batches);
}

void MediaSetImporterImpl::addLoads( FileListFile::FileInfoMap &loadHeaders)
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

void MediaSetImporterImpl::addBatches( FileListFile::FileInfoMap &batches)
{
  for ( const auto &batch : batches)
  {
    // get batch info for batch file
    BatchListFile::BatchInfoMap::const_iterator batchInfo =
      this->batchInfos.find( batch.first);

    // get the batch file
    BatchFiles::const_iterator batchFile( batchFiles.find( batch.first.second));

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

MediaSetImporterImpl::ContainerEntityPtr
MediaSetImporterImpl::checkCreateDirectory(
  const uint8_t mediumIndex,
  const path &filePath)
{
  path dirPath( filePath.parent_path());

  auto medium( mediaSet->getMedium( mediumIndex));

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

Arinc665::File::RawFile MediaSetImporterImpl::loadFile( const path &filePath)
{
  if (!boost::filesystem::is_regular( filePath))
  {
    //! @throw Arinc665Exception
    BOOST_THROW_EXCEPTION(
      Arinc665Exception() <<
        boost::errinfo_file_name( filePath.string()) <<
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

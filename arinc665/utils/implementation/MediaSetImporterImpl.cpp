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
#include <arinc665/Arinc665Crc.hpp>

#include <arinc665/media/Directory.hpp>

namespace Arinc665::Utils {

MediaSetImporterImpl::MediaSetImporterImpl(
  Arinc665Utils::ReadFileHandler readFileHandler):
  readFileHandler( readFileHandler)
{
}

Media::MediaSetPtr MediaSetImporterImpl::operator()()
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Medium 1: ";

  // Load list of files file
  File::FileListFile fileListFile(
    readFileHandler( 1, Arinc665::ListOfFilesName));

  // create Media set
  mediaSet = std::make_shared< Media::MediaSet>();
  mediaSet->partNumber( fileListFile.mediaSetPn());
  mediaSet->numberOfMedia( fileListFile.numberOfMediaSetMembers());

  // Add content of medium to media set
  addMedium( 1);

  // iterate over additional media
  for ( unsigned int mediaIndex = 2; mediaIndex <= mediaSet->numberOfMedia();
    ++mediaIndex)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Medium " << mediaIndex;

    addMedium( mediaIndex);
  }

  // finally add all files (regular, load headers, batches) to the media set
  addFiles();

  return mediaSet;
}

void MediaSetImporterImpl::addMedium( const uint8_t mediumIndex)
{
  loadFileListFile( mediumIndex);
  loadLoadListFile( mediumIndex);
  loadBatchListFile( mediumIndex);
  loadLoadHeaderFiles( mediumIndex);
  loadBatchFiles( mediumIndex);
}

void MediaSetImporterImpl::loadFileListFile( const uint8_t mediumIndex)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Load File List File " << Arinc665::ListOfFilesName;

  // Load list of files file
  File::FileListFile fileListFile(
    readFileHandler( mediumIndex, Arinc665::ListOfFilesName));

  // store first list of files for further tests
  if (!this->fileListFile)
  {
    this->fileListFile = fileListFile;
    fileInfos = fileListFile.filesAsMap();
  }
  else
  {
    // otherwise compare current list of files to first one
    if (!this->fileListFile.get().belongsToSameMediaSet( fileListFile))
    {
      //! @throw Arinc665Exception When FILES.LUM is inconsistent to other media
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          Arinc665::ListOfFilesName +
          " is not consistent to other file list"));
    }
  }

  // check for consistency of current file list file to media set
  if ((mediaSet->partNumber() != fileListFile.mediaSetPn()) ||
      (mediaSet->numberOfMedia() != fileListFile.numberOfMediaSetMembers()) ||
      (mediumIndex != fileListFile.mediaSequenceNumber()))
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::error) <<
     "media set: " <<
       mediaSet->partNumber() << " " <<
       (unsigned int)mediaSet->numberOfMedia() << " " <<
       (unsigned int)mediumIndex << " // " <<
       fileListFile.mediaSetPn() << " " <<
       (unsigned int)fileListFile.numberOfMediaSetMembers() << " " <<
       (unsigned int)fileListFile.mediaSequenceNumber();

    //! @throw Arinc665Exception When FILES.LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        Arinc665::ListOfFilesName + " is not consistent to media set"));
  }

  // iterate over files
  for ( auto &fileInfo : fileInfos)
  {
    // skip files, which are not part of the current medium
    if (fileInfo.second.memberSequenceNumber() != mediumIndex)
    {
      continue;
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Check file " << fileInfo.second.path();

    auto rawFile( readFileHandler( mediumIndex, fileInfo.second.path()));

    uint16_t crc = File::Arinc665File::calculateChecksum( rawFile, 0);

    // compare checksums
    if (crc != fileInfo.second.crc())
    {
      //! @throw Arinc665Exception When file CRCs does not match
      BOOST_THROW_EXCEPTION( Arinc665Exception() << AdditionalInfo(
        fileInfo.second.path().string() + ": CRC of file invalid"));
    }
  }
}

void MediaSetImporterImpl::loadLoadListFile( const uint8_t mediumIndex)
{
  BOOST_LOG_FUNCTION();

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Load Load List File " << Arinc665::ListOfLoadsName;

  // Load list of loads file
  File::LoadListFile loadListFile(
    readFileHandler( mediumIndex, Arinc665::ListOfLoadsName));

  // store first list of loads for further tests
  if (!this->loadListFile)
  {
    this->loadListFile = loadListFile;
    loadInfos = loadListFile.loadsAsMap();
  }
  else
  {
    // otherwise check against stored version
    if (!this->loadListFile.get().belongsToSameMediaSet( loadListFile))
    {
      //! @throw Arinc665Exception When LOADS.LUM is inconsistent to other media
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          Arinc665::ListOfLoadsName + " is not consistent to other loads list"));
    }
  }

  // check for consistency of current file list file to media set
  if ((mediaSet->partNumber() != loadListFile.mediaSetPn()) ||
      (mediaSet->numberOfMedia() != loadListFile.numberOfMediaSetMembers()) ||
      (loadListFile.mediaSequenceNumber() != mediumIndex))
  {
    //! @throw Arinc665Exception When LOADS:LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo(
        Arinc665::ListOfLoadsName + " is not consistent to media set"));
  }

  // only the load list of the first media must be checked in detail
  if (1U == mediumIndex)
  {
    // iterate over all loads
    for ( auto &loadInfo : loadInfos)
    {
      // check existence of load header files.

      auto file( fileInfos.find( loadInfo.first));

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

void MediaSetImporterImpl::loadBatchListFile( const uint8_t mediumIndex)
{
  // Check for optional batch list file
  if (fileInfos.find( {mediumIndex, ListOfBatchesName}) == fileInfos.end())
  {
    return;
  }

  BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
    "Load Batch List File " << Arinc665::ListOfBatchesName;

  // Load list of batches file
  File::BatchListFile batchListFile(
    readFileHandler( mediumIndex, Arinc665::ListOfBatchesName));

  if (!this->batchListFile)
  {
    this->batchListFile = batchListFile;
    this->batchInfos = batchListFile.batchesAsMap();
  }
  else
  {
    // otherwise check against stored version
    if (!this->batchListFile.get().belongsToSameMediaSet( batchListFile))
    {
      //! @throw Arinc665Exception When BATCHES.LUM is inconsistent to other media
     BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo(
          Arinc665::ListOfBatchesName + " is not consistent to other batches list"));
    }
  }

  // check for consistency of current batch list file to media set
  if ((mediaSet->partNumber() != batchListFile.mediaSetPn()) ||
      (mediaSet->numberOfMedia() != batchListFile.numberOfMediaSetMembers()) ||
      (batchListFile.mediaSequenceNumber() != mediumIndex))
  {
    //! @throw Arinc665Exception When FILES.LUM is inconsistent to media set
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
       AdditionalInfo(
         Arinc665::ListOfBatchesName + " is not consistent to media set"));
  }


  // only the batch list of the first media must be checked in detail
  if (1 == mediumIndex)
  {
    // iterate over all batches
    for ( auto &batchInfo : batchInfos)
    {
      auto file( fileInfos.find( batchInfo.first));

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

void MediaSetImporterImpl::loadLoadHeaderFiles( const uint8_t mediumIndex)
{
  // iterate over all load informations
  for ( const auto &loadInfo : loadInfos)
  {
    // skip load headers, which are not present on current medium
    if ( loadInfo.first.first != mediumIndex)
    {
      continue;
    }

    // find load header in file list
    auto loadHeaderFileIt( fileInfos.find( loadInfo.first));

    // Check existence
    if (fileInfos.end() == loadHeaderFileIt)
    {
      //! @throw Arinc665Exception When load header has not been found in file list
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Load header file not found in file list"));
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Load Header File " << loadHeaderFileIt->second.path();

    // decode load header
    File::LoadHeaderFile loadHeaderFile(
      readFileHandler( mediumIndex, loadHeaderFileIt->second.path()));

    // validate load header part number info with the stored information in load list
    if (loadHeaderFile.partNumber() != loadInfo.second.partNumber())
    {
      //! @throw Arinc665Exception When load header PN info is inconsistent to list of loads information
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Load header is not consistent to load list"));
    }

    // validate load header target HW list info with the stored information in load list
    if (loadHeaderFile.targetHardwareIds() != loadInfo.second.targetHardwareIds())
    {
      //! @throw Arinc665Exception When load header THW info is inconsistent to list of loads information
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Load header is not consistent to load list"));
    }

    //! @todo implement load CRC check.

    // add load header to global information
    loadHeaderFiles.insert(
      std::make_pair( loadHeaderFileIt->second.filename(), loadHeaderFile));
  }
}

void MediaSetImporterImpl::loadBatchFiles( const uint8_t mediumIndex)
{
  for ( const auto &batchInfo : batchInfos)
  {
    // Skip batch files not located on this medium
    if ( batchInfo.first.first != mediumIndex)
    {
      continue;
    }

    auto batchFileIt( fileInfos.find( batchInfo.first));

    if (fileInfos.end() == batchFileIt)
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::info) <<
      "Load Batch File " << batchFileIt->second.path();

    File::BatchFile batchFile(
      readFileHandler( mediumIndex, batchFileIt->second.path()));

    if (batchFile.partNumber() != batchInfo.second.partNumber())
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception() <<
        AdditionalInfo( "Medium is not consistent to media set"));
    }

    // add batch file to to batch file list
    batchFiles.insert(
      std::make_pair( batchFileIt->second.filename(), batchFile));
  }
}

void MediaSetImporterImpl::addFiles()
{
  File::FileListFile::FileInfoMap loadHeaders;
  File::FileListFile::FileInfoMap batches;

  // iterate over all files
  for ( const auto &fileInfo : fileInfos)
  {
    // get file type
    auto fileType(
      Arinc665::File::Arinc665File::fileType( fileInfo.first.second));

    // skip list files and handle load headers and batch files separate
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
    ContainerEntityPtr container( checkCreateDirectory(
      fileInfo.first.first,
      fileInfo.second.path().parent_path()));

    // place file
    auto filePtr{ container->addFile( fileInfo.second.filename())};
  }

  // Loads
  addLoads( loadHeaders);

  // Batches
  addBatches( batches);
}

void MediaSetImporterImpl::addLoads(
  File::FileListFile::FileInfoMap &loadHeaders)
{
  // iterate over load headers
  for ( const auto &loadHeader : loadHeaders)
  {
    auto load{ loadInfos.find( loadHeader.first)};
    auto loadHeaderFile{ loadHeaderFiles.find( loadHeader.first.second)};

    // obtain container (directory, medium), which will contain the load.
    ContainerEntityPtr container( checkCreateDirectory(
      loadHeader.first.first,
      loadHeader.second.path().parent_path()));

    // create load
    auto loadPtr( container->addLoad( loadHeader.second.filename()));

    loadPtr->partNumber( loadHeaderFile->second.partNumber());
    loadPtr->targetHardwareIds( loadHeaderFile->second.targetHardwareIds());

    // The load CRC
    Arinc665Crc32 loadCrc;

    {
      // load header file for load checksum calculation
      const auto file{ readFileHandler(
        loadPtr->medium()->mediumNumber(),
        loadPtr->path())};

      // calculate CRC
      loadCrc.process_bytes( &(*file.begin()), file.size() - sizeof(uint32_t));
    }

    // iterate over data files
    for ( const auto &dataFile : loadHeaderFile->second.dataFiles())
    {
      auto dataFilePtr( mediaSet->file( dataFile.filename()));

      dataFilePtr->partNumber( dataFile.partNumber());

      loadPtr->addDataFile( dataFilePtr);

      // load file for load checksum calculation
      const auto file{ readFileHandler(
        dataFilePtr->medium()->mediumNumber(),
        dataFilePtr->path())};

      loadCrc.process_block( &(*file.begin()), &(*file.begin()) + file.size());
    }

    // iterate over support files
    for ( const auto &supportFile : loadHeaderFile->second.supportFiles())
    {
      auto supportFilePtr( mediaSet->file( supportFile.filename()));

      supportFilePtr->partNumber( supportFile.partNumber());

      loadPtr->addSupportFile( supportFilePtr);

      // load file for load checksum calculation
      const auto file{ readFileHandler(
        supportFilePtr->medium()->mediumNumber(),
        supportFilePtr->path())};

      loadCrc.process_block( &(*file.begin()), &(*file.begin()) + file.size());
    }

    if (loadCrc.checksum() != loadHeaderFile->second.loadCrc())
    {
      BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::error) <<
        "Load CRC differs " << std::hex << loadCrc.checksum() << " : " <<
        loadHeaderFile->second.loadCrc();
    }
  }
}

void MediaSetImporterImpl::addBatches( File::FileListFile::FileInfoMap &batches)
{
  BOOST_LOG_FUNCTION();

  // iterate over batches
  for ( const auto &batch : batches)
  {
    // get batch info for batch file
    auto batchInfo( batchInfos.find( batch.first));

    // get the batch file
    auto batchFile( batchFiles.find( batch.first.second));

    ContainerEntityPtr container( checkCreateDirectory(
      batch.first.first,
      batch.second.path().parent_path()));

    // create batch
    auto batchPtr( container->addBatch( batch.second.filename()));

    batchPtr->partNumber( batchFile->second.partNumber());
    batchPtr->comment( batchFile->second.comment());

    // iterate over target hardware
    for ( const auto &targetHardware : batchFile->second.targetHardwares())
    {
      Media::WeakLoads loads;

      // iterate over loads
      for ( const auto& load : targetHardware.loads())
      {
        auto loadPtr{ mediaSet->load( load.headerFilename())};
        assert( loadPtr);

        loads.push_back( loadPtr);
      }

      // add Target Hardware/ Position
      batchPtr->target( targetHardware.targetHardwareIdPosition(), loads);
    }
  }
}

MediaSetImporterImpl::ContainerEntityPtr
MediaSetImporterImpl::checkCreateDirectory(
  const uint8_t mediumIndex,
  const std::filesystem::path &directoryPath)
{
  // make path relative (remove leading slash)
  auto dirPath{ directoryPath.relative_path()};

  auto medium( mediaSet->medium( mediumIndex));

  // we are in root-directory
  if ( dirPath.empty())
  {
    return medium;
  }

  ContainerEntityPtr dir{ medium};

  // iterate over path elements
  for ( auto &subPath : dirPath)
  {
    auto subDir{ dir->subDirectory( subPath.string())};

    // if sub-directory does not exists - create it
    if (!subDir)
    {
      subDir = dir->addSubDirectory( subPath.string());

      if (!subDir)
      {
        //! @throw Arinc665Exception When sub-directory cannot be created
        BOOST_THROW_EXCEPTION( Arinc665Exception() <<
          AdditionalInfo( "Cannot create sub-directory"));
      }
    }

    dir = subDir;
  }

  return dir;
}

}

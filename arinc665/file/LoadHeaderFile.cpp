/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::LoadHeaderFile.
 **/

#include "LoadHeaderFile.hpp"

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

LoadHeaderFile::LoadHeaderFile( Arinc665Version version) :
  Arinc665File( FileType::LoadUploadHeader, version, FileCrcOffset),
  loadCrcValue( 0)
{
}

LoadHeaderFile::LoadHeaderFile( const RawFile &rawFile):
  Arinc665File( FileType::LoadUploadHeader, rawFile, FileCrcOffset)
{
  decodeBody( rawFile);
}

LoadHeaderFile& LoadHeaderFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

std::string LoadHeaderFile::partNumber() const
{
  return partNumberValue;
}

void LoadHeaderFile::partNumber( const std::string &partNumber)
{
  partNumberValue = partNumber;
}

void LoadHeaderFile::partNumber( std::string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

const LoadHeaderFile::TargetHardwareIds&
LoadHeaderFile::targetHardwareIds() const
{
  return targetHardwareIdsValue;
}

LoadHeaderFile::TargetHardwareIds& LoadHeaderFile::targetHardwareIds()
{
  return targetHardwareIdsValue;
}

void LoadHeaderFile::targetHardwareIds(
  const TargetHardwareIds &targetHardwareIds)
{
  targetHardwareIdsValue = targetHardwareIds;
}

void LoadHeaderFile::targetHardwareIds(
  TargetHardwareIds &&targetHardwareIds)
{
  targetHardwareIdsValue = std::move( targetHardwareIds);
}

void LoadHeaderFile::targetHardwareId( const std::string &targetHardwareId)
{
  targetHardwareIdsValue.push_back( targetHardwareId);
}

void LoadHeaderFile::targetHardwareId( std::string &&targetHardwareId)
{
  targetHardwareIdsValue.push_back( std::move( targetHardwareId));
}

const LoadFilesInfo& LoadHeaderFile::dataFiles() const
{
  return dataFilesValue;
}

LoadFilesInfo& LoadHeaderFile::dataFiles()
{
  return dataFilesValue;
}

void LoadHeaderFile::dataFile( const LoadFileInfo &dataFileInfo)
{
  dataFilesValue.push_back( dataFileInfo);
}

void LoadHeaderFile::dataFile( LoadFileInfo &&dataFileInfo)
{
  dataFilesValue.push_back( std::move( dataFileInfo));
}

const LoadFilesInfo& LoadHeaderFile::supportFiles() const
{
  return supportFilesValue;
}

LoadFilesInfo& LoadHeaderFile::supportFiles()
{
  return supportFilesValue;
}

void LoadHeaderFile::supportFile( const LoadFileInfo &supportFileInfo)
{
  supportFilesValue.push_back( supportFileInfo);
}

void LoadHeaderFile::supportFile( LoadFileInfo &&supportFileInfo)
{
  supportFilesValue.push_back( std::move( supportFileInfo));
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::userDefinedData() const
{
  return userDefinedDataValue;
}

void LoadHeaderFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

void LoadHeaderFile::userDefinedData( UserDefinedData &&userDefinedData)
{
  userDefinedDataValue = std::move( userDefinedData);
}

uint32_t LoadHeaderFile::loadCrc() const
{
  return loadCrcValue;
}

void LoadHeaderFile::loadCrc( const uint32_t loadCrc)
{
  loadCrcValue = loadCrc;
}

RawFile LoadHeaderFile::encode() const
{
  RawFile rawFile( LoadHeaderSizeV2);

  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ LoadHeaderSizeV2};


  // Load Part Number
  auto rawLoadPn{ encodeString( partNumber())};
  assert( rawLoadPn.size() % 2 == 0);

  uint32_t loadPartNumberPtr = nextFreeOffset / 2;
  setInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffset,
    loadPartNumberPtr);
  nextFreeOffset += rawLoadPn.size();

  rawFile.insert( rawFile.end(), rawLoadPn.begin(), rawLoadPn.end());


  // THW ID list
  auto rawThwIdsList{ encodeStringList( targetHardwareIds())};
  assert( rawThwIdsList.size() % 2 == 0);

  uint32_t targetHardwareIdListPtr = nextFreeOffset / 2;
  setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    targetHardwareIdListPtr);
  nextFreeOffset += rawThwIdsList.size();

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end());


  // data files list pointer
  auto rawDataFiles{ encodeFileList( dataFiles())};
  assert( rawDataFiles.size() % 2 == 0);

  uint32_t dataFileListPtr = nextFreeOffset / 2;
  setInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffset,
    dataFileListPtr);
  nextFreeOffset += rawDataFiles.size();

  rawFile.insert( rawFile.end(), rawDataFiles.begin(), rawDataFiles.end());


  // support files (only if support files are present)
  auto rawSupportFiles{ encodeFileList( supportFiles())};
  assert( rawSupportFiles.size() % 2 == 0);

  uint32_t supportFileListPtr = 0;

  if (!supportFiles().empty())
  {
    supportFileListPtr = nextFreeOffset / 2;
    nextFreeOffset += rawSupportFiles.size();

    rawFile.insert(
      rawFile.end(),
      rawSupportFiles.begin(),
      rawSupportFiles.end());
  }

  setInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffset,
    supportFileListPtr);


  // user defined data pointer
  assert( userDefinedDataValue.size() % 2 == 0);
  uint32_t userDefinedDataPtr = 0;

  if (!userDefinedDataValue.empty())
  {
    userDefinedDataPtr = nextFreeOffset / 2;
    // nextFreeOffset += userDefinedDataValue.size();

    rawFile.insert(
      rawFile.end(),
      userDefinedDataValue.begin(),
      userDefinedDataValue.end());
  }

  setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);


  //! @todo add Load Check Values.

  // Resize to final size ( File CRC + Load CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t) + sizeof( uint32_t));

  // set header and crc
  insertHeader( rawFile);

  // load CRC

  setInt< uint32_t>(
    rawFile.begin() + (rawFile.size() - LoadCrcOffset),
    loadCrcValue);

  return rawFile;
}

void LoadHeaderFile::decodeBody( const RawFile &rawFile)
{
  uint32_t loadPartNumberPtr;
  getInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffset,
    loadPartNumberPtr);

  uint32_t targetHardwareIdListPtr;
  getInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    targetHardwareIdListPtr);

  uint32_t dataFileListPtr;
  getInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffset,
    dataFileListPtr);

  uint32_t supportFileListPtr;
  getInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffset,
    supportFileListPtr);

  uint32_t userDefinedDataPtr;
  getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);

  // ToDO: add Load Type Description Field (ARINC 665-3)
  // ToDO: add THW IDs with Positions Field (ARINC 665-3)
  // ToDO: add Load Check Value Field (ARINC 665-3)


  // load part number
  decodeString( rawFile.begin() + loadPartNumberPtr * 2, partNumberValue);


  // target hardware id list
  decodeStringList(
    rawFile.begin() + targetHardwareIdListPtr * 2,
    targetHardwareIdsValue);


  // data file list
  dataFilesValue = decodeFileList( rawFile, dataFileListPtr * 2);


  // support file list
  if ( 0 != supportFileListPtr)
  {
    supportFilesValue = decodeFileList( rawFile, supportFileListPtr * 2);
  }


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    userDefinedDataValue.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.end() - 6U);
    //! @todo update End of user defined data calculation in conjunction with ARINC 665-3
  }

  // file crc decoded and checked within base class

  // load crc
  getInt< uint32_t>( rawFile.end() - LoadCrcOffset, loadCrcValue);
}

RawFile LoadHeaderFile::encodeFileList( const LoadFilesInfo &loadFilesInfo) const
{
  RawFile rawFileList( sizeof( uint16_t));

  // number of loads
  setInt< uint16_t>(
    rawFileList.begin(),
    safeCast< uint16_t>( loadFilesInfo.size()));

  // iterate over files
  uint16_t fileCounter( 0);
  for (auto const &fileInfo : loadFilesInfo)
  {
    ++fileCounter;
    auto const rawFilename( encodeString( fileInfo.filename()));
    assert( rawFilename.size() % 2 == 0);
    auto const rawPartNumber( encodeString( fileInfo.partNumber()));
    assert( rawPartNumber.size() % 2 == 0);

    RawFile rawFileInfo(
      sizeof( uint16_t) + // next load pointer
      rawFilename.size() +
      rawPartNumber.size() +
      sizeof( uint32_t) + // file length
      sizeof( uint16_t)); // crc

    auto fileInfoIt( rawFileInfo.begin());

    // next load pointer (is set to 0 for last load)
    fileInfoIt = setInt< uint16_t>(
      fileInfoIt,
      (fileCounter == loadFilesInfo.size()) ?
        (0U) :
        safeCast< uint16_t>( rawFileInfo.size() / 2));

    // filename
    fileInfoIt = std::copy( rawFilename.begin(), rawFilename.end(), fileInfoIt);

    // part number
    fileInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), fileInfoIt);

    // file length
    fileInfoIt = setInt< uint32_t>( fileInfoIt, fileInfo.length());

    // CRC
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.crc());

    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFileList;
}

LoadFilesInfo LoadHeaderFile::decodeFileList(
  const RawFile &rawFile,
  const std::size_t offset)
{
  RawFile::const_iterator it( rawFile.begin() + offset);

  LoadFilesInfo files;

  // number of data files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  // iterate over file index
  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt( it);

    // next file pointer
    uint16_t filePointer;
    listIt = getInt< uint16_t>( listIt, filePointer);

    // filename
    std::string name;
    listIt = decodeString( listIt, name);

    // part number
    std::string partNumber;
    listIt = decodeString( listIt, partNumber);

    // file length
    //! @todo Attention ! data and support files differs in interpretation!
    //! data files -> number of 16bit words vs. support files -> number of 8bit words.
    uint32_t length;
    listIt = getInt< uint32_t>( listIt, length);

    // CRC
    uint16_t crc;
    listIt = getInt< uint16_t>( listIt, crc);

    // set it to begin of next file
    it += filePointer * 2;

    // file info
    files.emplace_back( name, partNumber, length, crc);
  }

  return files;
}

}

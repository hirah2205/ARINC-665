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
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

LoadHeaderFile::LoadHeaderFile( Arinc665Version version) :
  Arinc665File( FileType::LoadUploadHeader, version, 6U),
  loadCrc( 0)
{
}

LoadHeaderFile::LoadHeaderFile( const RawFile &rawFile):
  Arinc665File( FileType::LoadUploadHeader, rawFile, 6U)
{
  decodeBody( rawFile);
}

LoadHeaderFile& LoadHeaderFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

LoadHeaderFile::string LoadHeaderFile::getPartNumber() const
{
  return partNumber;
}

void LoadHeaderFile::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

const LoadHeaderFile::TargetHardwareIds&
LoadHeaderFile::getTargetHardwareIds() const
{
  return targetHardwareIds;
}

LoadHeaderFile::TargetHardwareIds& LoadHeaderFile::getTargetHardwareIds()
{
  return targetHardwareIds;
}

void LoadHeaderFile::setTargetHardwareIds(
  const TargetHardwareIds &targetHardwareIds)
{
  this->targetHardwareIds = targetHardwareIds;
}

void LoadHeaderFile::setTargetHardwareIds(
  TargetHardwareIds &&targetHardwareIds)
{
  this->targetHardwareIds = targetHardwareIds;
}

void LoadHeaderFile::addTargetHardwareId(
  const LoadHeaderFile::string &targetHardwareId)
{
  targetHardwareIds.push_back( targetHardwareId);
}

void LoadHeaderFile::addTargetHardwareId( string targetHardwareId)
{
  targetHardwareIds.push_back( targetHardwareId);
}

const LoadFilesInfo& LoadHeaderFile::getDataFiles() const
{
  return dataFilesInfo;
}

LoadFilesInfo& LoadHeaderFile::getDataFiles()
{
  return dataFilesInfo;
}

void LoadHeaderFile::addDataFile( const LoadFileInfo &dataFileInfo)
{
  dataFilesInfo.push_back( dataFileInfo);
}

void LoadHeaderFile::addDataFile( LoadFileInfo &&dataFileInfo)
{
  dataFilesInfo.push_back( dataFileInfo);
}

const LoadFilesInfo& LoadHeaderFile::getSupportFiles() const
{
  return supportFilesInfo;
}

LoadFilesInfo& LoadHeaderFile::getSupportFiles()
{
  return supportFilesInfo;
}

void LoadHeaderFile::addSupportFile( const LoadFileInfo &supportFileInfo)
{
  supportFilesInfo.push_back( supportFileInfo);
}

void LoadHeaderFile::addSupportFile( LoadFileInfo &&supportFileInfo)
{
  supportFilesInfo.push_back( supportFileInfo);
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::getUserDefinedData() const
{
  return userDefinedData;
}

void LoadHeaderFile::setUserDefinedData( const UserDefinedData &userDefinedData)
{
  this->userDefinedData = userDefinedData;
}

uint32_t LoadHeaderFile::getLoadCrc() const
{
  return loadCrc;
}

void LoadHeaderFile::setLoadCrc( const uint32_t loadCrc)
{
  this->loadCrc = loadCrc;
}

RawFile LoadHeaderFile::encode() const
{
  RawFile rawFile(
    BaseHeaderOffset +
    5 * sizeof( uint32_t) + // loadPnPtr, thwIdListPtr, dataFileListPtr, supportFileListPtr, userDefinedDataPtr
    sizeof( uint16_t) +     // CRC
    sizeof( uint32_t));     // load CRC


  auto rawLoadPn( encodeString( getPartNumber()));
  assert( rawLoadPn.size() % 2 == 0);

  auto rawThwIdsList( encodeStringList( getTargetHardwareIds()));
  assert( rawThwIdsList.size() % 2 == 0);

  auto rawDataFiles( encodeFileList( getDataFiles()));
  assert( rawDataFiles.size() % 2 == 0);

  auto rawSupportFiles( encodeFileList( getSupportFiles()));
  assert( rawSupportFiles.size() % 2 == 0);

  auto it( rawFile.begin() + BaseHeaderOffset);

  // load part number information pointer
  uint32_t loadPartNumberPtr =
    (BaseHeaderOffset + (5 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, loadPartNumberPtr);

  // THW ID list pointer
  uint32_t targetHardwareIdListPtr =
    loadPartNumberPtr + (rawLoadPn.size() / 2);
  it = setInt< uint32_t>( it, targetHardwareIdListPtr);

  // data files list pointer
  uint32_t dataFileListPtr =
    targetHardwareIdListPtr + (rawThwIdsList.size() / 2);
  it = setInt< uint32_t>( it, dataFileListPtr);

  // support files list pointer (only if support files are present)
  uint32_t supportFileListPtr =
    getSupportFiles().empty() ?
      (0) :
      (dataFileListPtr + (rawDataFiles.size() / 2));
  it = setInt< uint32_t>( it, supportFileListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedData.empty() ? 0 : supportFileListPtr + (rawSupportFiles.size() / 2);
  it = setInt< uint32_t>( it, userDefinedDataPtr);
  //! @todo if support files is omitted completely (ARINC 6653-3) - pointer to
  //! user defined data calculation must be corrected.


  // load part number
  it = rawFile.insert( it, rawLoadPn.begin(), rawLoadPn.end());
  it += rawLoadPn.size();

  // THW ID list
  it = rawFile.insert( it, rawThwIdsList.begin(), rawThwIdsList.end());
  it += rawThwIdsList.size();

  // Data file list
  it = rawFile.insert( it, rawDataFiles.begin(), rawDataFiles.end());
  it += rawDataFiles.size();

  // Support file list (if empty, only count (0) is written)
  //! @todo in ARINC 665-3 the number of support files field is also omitted
  it = rawFile.insert( it, rawSupportFiles.begin(), rawSupportFiles.end());
  it += rawSupportFiles.size();

  if (!userDefinedData.empty())
  {
    assert( userDefinedData.size() % 2 == 0);
    rawFile.insert( it, userDefinedData.begin(), userDefinedData.end());
  }

  // set header and crc
  insertHeader( rawFile);

  // load CRC
  setInt< uint32_t>( rawFile.end() - 4U, loadCrc);

  return rawFile;
}

void LoadHeaderFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  RawFile::const_iterator it = rawFile.begin() + BaseHeaderOffset;

  uint32_t loadPartNumberPtr;
  it = getInt< uint32_t>( it, loadPartNumberPtr);

  uint32_t targetHardwareIdListPtr;
  it = getInt< uint32_t>( it, targetHardwareIdListPtr);

  uint32_t dataFileListPtr;
  it = getInt< uint32_t>( it, dataFileListPtr);

  uint32_t supportFileListPtr;
  it = getInt< uint32_t>( it, supportFileListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // load part number
  it = rawFile.begin() + loadPartNumberPtr * 2;
  it = decodeString( it, partNumber);

  // target hardware id list
  it = rawFile.begin() + targetHardwareIdListPtr * 2;
  it = decodeStringList( it, targetHardwareIds);

  // data file list
  dataFilesInfo = decodeFileList( rawFile, dataFileListPtr * 2);

  // support file list
  if ( 0 != supportFileListPtr)
  {
    supportFilesInfo = decodeFileList( rawFile, supportFileListPtr * 2);
  }

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, rawFile.end() - 6U);
  }

  // file crc decoded and checked within base class

  // load crc
  getInt< uint32_t>( rawFile.end() - 4U, loadCrc);
}

RawFile LoadHeaderFile::encodeFileList( const LoadFilesInfo &loadFilesInfo) const
{
  RawFile rawFileList( sizeof( uint16_t));

  // number of loads
  setInt< uint16_t>( rawFileList.begin(), loadFilesInfo.size());

  // iterate over files
  uint16_t fileCounter( 0);
  for (auto const &fileInfo : loadFilesInfo)
  {
    ++fileCounter;
    auto const rawFilename( encodeString( fileInfo.getFilename()));
    assert( rawFilename.size() % 2 == 0);
    auto const rawPartNumber( encodeString( fileInfo.getPartNumber()));
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
        (rawFileInfo.size() / 2));

    // filename
    fileInfoIt = std::copy( rawFilename.begin(), rawFilename.end(), fileInfoIt);

    // part number
    fileInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), fileInfoIt);

    // file length
    fileInfoIt = setInt< uint32_t>( fileInfoIt, fileInfo.getLength());

    // CRC
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.getCrc());

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
    string name;
    listIt = decodeString( listIt, name);

    // part number
    string partNumber;
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
}

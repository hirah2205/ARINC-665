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
 * @brief Definition of class Arinc665::File::FileListFile.
 **/

#include "FileListFile.hpp"

#include <arinc665/file/FileFactory.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

FileListFile::FileListFile( Arinc665Version version):
  ListFile( FileType::FileList, version),
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
{
}

FileListFile::FileListFile( const RawFile &rawFile):
  ListFile( FileType::FileList, rawFile)
{
  decodeBody( rawFile);
}

FileListFile& FileListFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

FileListFile::string FileListFile::getMediaSetPn() const
{
  return mediaSetPn;
}

void FileListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t FileListFile::getMediaSequenceNumber() const
{
  return mediaSequenceNumber;
}

void FileListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t FileListFile::getNumberOfMediaSetMembers() const
{
  return numberOfMediaSetMembers;
}

void FileListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  this->numberOfMediaSetMembers = numberOfMediaSetMembers;
}

size_t FileListFile::getNumberOfFiles() const
{
  return filesInfo.size();
}

const FilesInfo& FileListFile::getFilesInfo() const
{
  return filesInfo;
}

FilesInfo& FileListFile::getFilesInfo()
{
  return filesInfo;
}

FileListFile::FileInfoMap FileListFile::getFileInfosAsMap() const
{
  FileInfoMap fileMap;

  for ( const auto &fileInfo : filesInfo)
  {
    fileMap.insert(
      std::make_pair(
        std::make_pair(
          fileInfo.getMemberSequenceNumber(),
          fileInfo.getFilename()),
        fileInfo));
  }

  return fileMap;
}

void FileListFile::addFileInfo( const FileInfo &fileInfo)
{
  filesInfo.push_back( fileInfo);
}

void FileListFile::addFileInfo( FileInfo &&fileInfo)
{
  filesInfo.push_back( fileInfo);
}

FileListFile::FileInfoPathMap FileListFile::getFilesInfoAsPathMap() const
{
  FileInfoPathMap fileMap;

  for ( const auto &fileInfo : filesInfo)
  {
    fileMap.insert(
      std::make_pair(
        std::make_pair(
          fileInfo.getMemberSequenceNumber(),
          path( fileInfo.getPathName()) / fileInfo.getFilename()),
        fileInfo));
  }

  return fileMap;
}

const FileListFile::UserDefinedData& FileListFile::getUserDefinedData() const
{
  return userDefinedData;
}

void FileListFile::setUserDefinedData(
  const UserDefinedData &userDefinedData)
{
  this->userDefinedData = userDefinedData;
}

bool FileListFile::belongsToSameMediaSet( const FileListFile &other) const
{
  if (
    (mediaSetPn != other.getMediaSetPn()) ||
    (numberOfMediaSetMembers != other.getNumberOfMediaSetMembers()) ||
    (userDefinedData != other.getUserDefinedData()))
  {
    return false;
  }

  FilesInfo otherFileList( other.getFilesInfo());

  if (filesInfo.size() != otherFileList.size())
  {
    return false;
  }

  for ( unsigned int i = 0; i < filesInfo.size(); ++i)
  {
    if (
      (filesInfo[i].getFilename() != otherFileList[i].getFilename()) ||
      (filesInfo[i].getPathName() != otherFileList[i].getPathName()))
    {
      return false;
    }

    switch ( FileFactory::getFileType( filesInfo[i].getFilename()))
    {
      case FileType::LoadList:
      case FileType::BatchList:
        // skip test of CRC and Member Sequence Number
        break;

      default:
        if (
          (filesInfo[i].getCrc() != otherFileList[i].getCrc()) ||
          (filesInfo[i].getMemberSequenceNumber() != otherFileList[i].getMemberSequenceNumber()))
        {
          return false;
        }

        break;
    }
  }

  return true;
}

RawFile FileListFile::encode() const
{
  RawFile rawFile(
    BaseHeaderOffset +
    3 * sizeof( uint32_t) + // mediaInformationPtr, fileListPtr, userDefinedDataPtr
    2 * sizeof( uint8_t)  + // media sequence number, number of media set members
    sizeof( uint16_t));     // crc

  auto rawMediaSetPn( encodeString( getMediaSetPn()));
  assert( rawMediaSetPn.size() % 2 == 0);
  auto rawFilesInfo( encodeFileInfo());
  assert( rawFilesInfo.size() % 2 == 0);

  auto it( rawFile.begin() + BaseHeaderOffset);

  // media information pointer
  uint32_t mediaInformationPtr =
    (BaseHeaderOffset + (3 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, mediaInformationPtr);

  // file list pointer
  uint32_t fileListPtr =
    mediaInformationPtr + (2 * sizeof( uint8_t)) / 2 + rawMediaSetPn.size() / 2;
  it = setInt< uint32_t>( it, fileListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedData.empty() ? 0 : fileListPtr + rawFilesInfo.size() / 2;
  it = setInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.insert( it, rawMediaSetPn.begin(), rawMediaSetPn.end());
  it += rawMediaSetPn.size();

  // media sequence number
  it = setInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = setInt< uint8_t>( it, numberOfMediaSetMembers);

  // file list
  it = rawFile.insert( it, rawFilesInfo.begin(), rawFilesInfo.end());
  it += rawFile.size();

  if (!userDefinedData.empty())
  {
    assert( userDefinedData.size() % 2 == 0);
    rawFile.insert( it, userDefinedData.begin(), userDefinedData.end());
  }

  //! @todo calculate CRC

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void FileListFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  RawFile::const_iterator it = rawFile.begin() + BaseHeaderOffset;

  // media information pointer
  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  // file list pointer
  uint32_t fileListPtr;
  it = getInt< uint32_t>( it, fileListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.begin() + mediaInformationPtr * 2;
  it = decodeString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // file list
  filesInfo = decodeFilesInfo( rawFile, 2 * fileListPtr);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, rawFile.end() - 2);
  }

  // file crc decoded and checked within base class
}

RawFile FileListFile::encodeFileInfo() const
{
  RawFile rawFilesInfo( sizeof( uint16_t));

  // number of files
  setInt< uint16_t>( rawFilesInfo.begin(), getNumberOfFiles());

  // iterate over files
  for (auto const &fileInfo : getFilesInfo())
  {
    auto const rawFilename( encodeString( fileInfo.getFilename()));
    assert( rawFilename.size() % 2 == 0);
    auto const rawPathname( encodeString( fileInfo.getPathName()));
    assert( rawPathname.size() % 2 == 0);

    RawFile rawFileInfo(
      sizeof( uint16_t) + // next pointer
      rawFilename.size() +
      rawPathname.size() +
      sizeof( uint16_t) + // member sequence number
      sizeof( uint16_t)); // crc

    auto fileInfoIt( rawFileInfo.begin());

    // next file pointer
    fileInfoIt = setInt< uint16_t>( fileInfoIt, rawFileInfo.size() / 2);

    // filename
    fileInfoIt = std::copy( rawFilename.begin(), rawFilename.end(), fileInfoIt);

    // path name
    fileInfoIt = std::copy( rawPathname.begin(), rawPathname.end(), fileInfoIt);

    // member sequence number
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.getMemberSequenceNumber());

    // crc
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.getCrc());

    // add file info to files info
    rawFilesInfo.insert( rawFilesInfo.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFilesInfo;
}

FilesInfo FileListFile::decodeFilesInfo(
  const RawFile &rawFile,
  const std::size_t offset)
{
  auto it( rawFile.begin() + offset);

  FilesInfo fileList;

  // number of files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt( it);

    // next file pointer
    uint16_t filePointer;
    listIt = getInt< uint16_t>( listIt, filePointer);

    // filename
    string filename;
    listIt = decodeString( listIt, filename);

    // path name
    string pathName;
    listIt = decodeString( listIt, pathName);

    // member sequence number
    uint16_t memberSequenceNumber;
    listIt = getInt< uint16_t>( listIt, memberSequenceNumber);

    // crc
    uint16_t crc;
    listIt = getInt< uint16_t>( listIt, crc);

    // set it to begin of next file
    it += filePointer * 2;

    fileList.emplace_back( filename, pathName, memberSequenceNumber, crc);
  }

  return fileList;
}

}
}

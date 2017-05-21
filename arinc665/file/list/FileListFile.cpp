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

#include <arinc665/file/StringHelper.hpp>
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

FileListFile::FileListFile( const RawFile &file):
  ListFile( FileType::FileList, file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  decodeBody( file);
}

FileListFile& FileListFile::operator=( const RawFile &file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  decodeBody( file);

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
  return fileInfos.size();
}

const FileInfoList& FileListFile::getFileInfos() const
{
  return fileInfos;
}

FileInfoList& FileListFile::getFileInfos()
{
  return fileInfos;
}

FileListFile::FileInfoMap FileListFile::getFileInfosAsMap() const
{
  FileInfoMap fileMap;

  for ( const auto &fileInfo : fileInfos)
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

FileListFile::FileInfoPathMap FileListFile::getFileInfosAsPathMap() const
{
  FileInfoPathMap fileMap;

  for ( const auto &fileInfo : fileInfos)
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

  FileInfoList otherFileList( other.getFileInfos());

  if (fileInfos.size() != otherFileList.size())
  {
    return false;
  }

  for ( unsigned int i = 0; i < fileInfos.size(); ++i)
  {
    if (
      (fileInfos[i].getFilename() != otherFileList[i].getFilename()) ||
      (fileInfos[i].getPathName() != otherFileList[i].getPathName()))
    {
      return false;
    }

    switch ( FileFactory::getFileType( fileInfos[i].getFilename()))
    {
      case FileType::LoadList:
      case FileType::BatchList:
        // skip test of CRC and Member Sequence Number
        break;

      default:
        if (
          (fileInfos[i].getCrc() != otherFileList[i].getCrc()) ||
          (fileInfos[i].getMemberSequenceNumber() != otherFileList[i].getMemberSequenceNumber()))
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
  RawFile file( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // set header and crc
  insertHeader( file);

  return file;
}

void FileListFile::decodeBody( const RawFile &file)
{
  // set processing start to position after spare
  RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t fileListPtr;
  it = getInt< uint32_t>( it, fileListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = file.begin() + mediaInformationPtr * 2;
  it = getString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // file list
  fileInfos = decodeFileInfo( file, 2 * fileListPtr);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 2);
  }

  // file crc decoded and checked within base class
}

FileInfoList FileListFile::decodeFileInfo(
  const RawFile &file,
  const std::size_t offset)
{
  auto it( file.begin() + offset);

  FileInfoList fileList;

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
    listIt = getString( listIt, filename);

    // path name
    string pathName;
    listIt = getString( listIt, pathName);

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

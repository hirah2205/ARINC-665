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

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

FileListFile::FileListFile( Arinc665Version version):
  ListFile( FileType::FileList, version),
  mediaSequenceNumberValue( 0),
  numberOfMediaSetMembersValue( 0)
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

std::string FileListFile::mediaSetPn() const
{
  return mediaSetPnValue;
}

void FileListFile::mediaSetPn( const std::string &mediaSetPn)
{
  mediaSetPnValue = mediaSetPn;
}

void FileListFile::mediaSetPn( std::string &&mediaSetPn)
{
  mediaSetPnValue = std::move( mediaSetPn);
}

uint8_t FileListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberValue;
}

void FileListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  mediaSequenceNumberValue = mediaSequenceNumber;
}

uint8_t FileListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersValue;
}

void FileListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  numberOfMediaSetMembersValue = numberOfMediaSetMembers;
}

size_t FileListFile::numberOfFiles() const
{
  return filesInfoValue.size();
}

const FilesInfo& FileListFile::filesInfo() const
{
  return filesInfoValue;
}

FilesInfo& FileListFile::filesInfo()
{
  return filesInfoValue;
}

FileListFile::FileInfoMap FileListFile::filesInfoAsMap() const
{
  FileInfoMap fileMap;

  for ( const auto &fileInfo : filesInfoValue)
  {
    fileMap.insert(
      std::make_pair(
        std::make_pair(
          fileInfo.memberSequenceNumber(),
          fileInfo.filename()),
        fileInfo));
  }

  return fileMap;
}

FileListFile::FileInfoPathMap FileListFile::filesInfoAsPathMap() const
{
  FileInfoPathMap fileMap;

  for ( const auto &fileInfo : filesInfoValue)
  {
    fileMap.insert(
      std::make_pair(
        std::make_pair(
          fileInfo.memberSequenceNumber(),
          fileInfo.path()),
        fileInfo));
  }

  return fileMap;
}

void FileListFile::fileInfo( const FileInfo &fileInfo)
{
  filesInfoValue.push_back( fileInfo);
}

void FileListFile::fileInfo( FileInfo &&fileInfo)
{
  filesInfoValue.push_back( std::move( fileInfo));
}


const FileListFile::UserDefinedData& FileListFile::userDefinedData() const
{
  return userDefinedDataValue;
}

void FileListFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

bool FileListFile::belongsToSameMediaSet( const FileListFile &other) const
{
  if (
    (mediaSetPnValue != other.mediaSetPn()) ||
    (numberOfMediaSetMembersValue != other.numberOfMediaSetMembers()) ||
    (userDefinedDataValue != other.userDefinedData()))
  {
    return false;
  }

  const auto &otherFileList( other.filesInfo());

  if (filesInfoValue.size() != otherFileList.size())
  {
    return false;
  }

  for ( unsigned int i = 0; i < filesInfoValue.size(); ++i)
  {
    if (
      (filesInfoValue[i].filename() != otherFileList[i].filename()) ||
      (filesInfoValue[i].pathName() != otherFileList[i].pathName()))
    {
      return false;
    }

    switch ( fileType( filesInfoValue[i].filename()))
    {
      case FileType::LoadList:
      case FileType::BatchList:
        // skip test of CRC and Member Sequence Number
        break;

      default:
        if (
          (filesInfoValue[i].crc() != otherFileList[i].crc()) ||
          (filesInfoValue[i].memberSequenceNumber() != otherFileList[i].memberSequenceNumber()))
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
    3 * sizeof( uint32_t) + // mediaInformationPtr, filesListPtr, userDefinedDataPtr
    2 * sizeof( uint8_t)  + // media sequence number, number of media set members
    sizeof( uint16_t));     // crc

  auto rawMediaSetPn( encodeString( mediaSetPn()));
  assert( rawMediaSetPn.size() % 2 == 0);
  auto rawFilesInfo( encodeFilesInfo());
  assert( rawFilesInfo.size() % 2 == 0);

  auto it( rawFile.begin() + BaseHeaderOffset);

  // media information pointer
  uint32_t mediaInformationPtr =
    (BaseHeaderOffset + (3 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, mediaInformationPtr);

  // files list pointer
  uint32_t fileListPtr =
    mediaInformationPtr + (2 * sizeof( uint8_t)) / 2 + rawMediaSetPn.size() / 2;
  it = setInt< uint32_t>( it, fileListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedDataValue.empty() ? 0 : fileListPtr + rawFilesInfo.size() / 2;
  it = setInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.insert( it, rawMediaSetPn.begin(), rawMediaSetPn.end());
  it += rawMediaSetPn.size();

  // media sequence number
  it = setInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  it = setInt< uint8_t>( it, numberOfMediaSetMembersValue);

  // file list
  it = rawFile.insert( it, rawFilesInfo.begin(), rawFilesInfo.end());
  it += rawFilesInfo.size();

  if (!userDefinedDataValue.empty())
  {
    assert( userDefinedDataValue.size() % 2 == 0);
    rawFile.insert(
      it,
      userDefinedDataValue.begin(),
      userDefinedDataValue.end());
  }

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void FileListFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  auto it{ rawFile.begin() + BaseHeaderOffset};

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
  it = decodeString( it, mediaSetPnValue);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembersValue);

  // file list
  decodeFilesInfo( rawFile, 2 * fileListPtr);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedDataValue.assign( it, rawFile.end() - 2);
  }

  // file crc decoded and checked within base class
}

RawFile FileListFile::encodeFilesInfo() const
{
  RawFile rawFilesInfo( sizeof( uint16_t));

  // number of files
  setInt< uint16_t>( rawFilesInfo.begin(), numberOfFiles());

  // iterate over files
  uint16_t fileCounter( 0);
  for (auto const &fileInfo : filesInfo())
  {
    ++fileCounter;
    auto const rawFilename( encodeString( fileInfo.filename()));
    assert( rawFilename.size() % 2 == 0);
    auto const rawPathname( encodeString( fileInfo.pathName()));
    assert( rawPathname.size() % 2 == 0);

    RawFile rawFileInfo(
      sizeof( uint16_t) + // next pointer
      rawFilename.size() +
      rawPathname.size() +
      sizeof( uint16_t) + // member sequence number
      sizeof( uint16_t)); // crc

    auto fileInfoIt( rawFileInfo.begin());

    // next file pointer (is set to 0 for last file)
    fileInfoIt = setInt< uint16_t>(
      fileInfoIt,
      (fileCounter == numberOfFiles()) ?
        (0U) :
        (rawFileInfo.size() / 2));

    // filename
    fileInfoIt = std::copy( rawFilename.begin(), rawFilename.end(), fileInfoIt);

    // path name
    fileInfoIt = std::copy( rawPathname.begin(), rawPathname.end(), fileInfoIt);

    // member sequence number
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.memberSequenceNumber());

    // crc
    fileInfoIt = setInt< uint16_t>( fileInfoIt, fileInfo.crc());

    // add file info to files info
    rawFilesInfo.insert( rawFilesInfo.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFilesInfo;
}

void FileListFile::decodeFilesInfo(
  const RawFile &rawFile,
  const std::size_t offset)
{
  auto it( rawFile.begin() + offset);

  // clear potentially data
  filesInfoValue.clear();

  // number of files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  // iterate over index
  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt( it);

    // next file pointer
    uint16_t filePointer;
    listIt = getInt< uint16_t>( listIt, filePointer);

    //! @todo check pointer for != 0 (all except last ==> OK, last ==> error)

    // filename
    std::string filename;
    listIt = decodeString( listIt, filename);

    // path name
    std::string pathName;
    listIt = decodeString( listIt, pathName);

    // member sequence number
    uint16_t memberSequenceNumber;
    listIt = getInt< uint16_t>( listIt, memberSequenceNumber);

    // crc
    uint16_t crc;
    listIt = getInt< uint16_t>( listIt, crc);

    // set it to begin of next file
    it += filePointer * 2;

    filesInfoValue.emplace_back( filename, pathName, memberSequenceNumber, crc);
  }
}

}

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

#include <arinc665/Arinc665Exception.hpp>

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
  RawFile rawFile( FileHeaderSizeV2);  // TODO

  // spare field
  setInt< uint32_t>( rawFile.begin() + SpareFieldOffset, 0U);

  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ rawFile.size()};


  // media set part number
  const auto rawMediaSetPn{ encodeString( mediaSetPn())};
  assert( rawMediaSetPn.size() % 2 == 0);

  rawFile.insert( rawFile.end(), rawMediaSetPn.begin(), rawMediaSetPn.end());

  rawFile.resize( rawFile.size() + 2 * sizeof( uint8_t));

  // media sequence number
  setInt< uint8_t>(
    rawFile.begin() + nextFreeOffset + rawMediaSetPn.size(),
    mediaSequenceNumberValue);

  // number of media set members
  setInt< uint8_t>(
    rawFile.begin() + nextFreeOffset + rawMediaSetPn.size() + sizeof( uint8_t),
    numberOfMediaSetMembersValue);

  setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawMediaSetPn.size() + 2 * sizeof( uint8_t);


  // media set files list
  const auto rawFilesInfo{ encodeFilesInfo()};
  assert( rawFilesInfo.size() % 2 == 0);

  setInt< uint32_t>(
    rawFile.begin() + MediaSetFilesPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawFilesInfo.size();

  rawFile.insert( rawFile.end(), rawFilesInfo.begin(), rawFilesInfo.end());


  // user defined data
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


  // Resize to final size ( File CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t));

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void FileListFile::decodeBody( const RawFile &rawFile)
{
  uint32_t spare;
  getInt< uint32_t>( rawFile.begin() + SpareFieldOffset, spare);

  if (0U != spare)
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << AdditionalInfo( "Spare is not 0"));
  }


  // media information pointer
  uint32_t mediaInformationPtr;
  getInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffset,
    mediaInformationPtr);

  // file list pointer
  uint32_t fileListPtr;
  getInt< uint32_t>(
    rawFile.begin() + MediaSetFilesPointerFieldOffset,
    fileListPtr);


  // user defined data pointer
  uint32_t userDefinedDataPtr;
  getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);


  // media set part number
  auto it = decodeString(
    rawFile.begin() + mediaInformationPtr * 2,
    mediaSetPnValue);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  getInt< uint8_t>( it, numberOfMediaSetMembersValue);


  // file list
  decodeFilesInfo( rawFile, 2 * fileListPtr);


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    userDefinedDataValue.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + rawFile.size() - DefaultChecksumPosition);
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

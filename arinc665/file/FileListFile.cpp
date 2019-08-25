/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::FileListFile.
 **/

#include "FileListFile.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/file/CheckValueUtils.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

FileListFile::FileListFile( SupportedArinc665Version version):
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
  return filesValue.size();
}

const FilesInfo& FileListFile::files() const
{
  return filesValue;
}

FilesInfo& FileListFile::files()
{
  return filesValue;
}

FileListFile::FileInfoMap FileListFile::filesAsMap() const
{
  FileInfoMap fileMap;

  for ( const auto &fileInfo : filesValue)
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

FileListFile::FileInfoPathMap FileListFile::filesAsPathMap() const
{
  FileInfoPathMap fileMap;

  for ( const auto &fileInfo : filesValue)
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

void FileListFile::file( const FileInfo &file)
{
  filesValue.push_back( file);
}

void FileListFile::file( FileInfo &&file)
{
  filesValue.push_back( std::move( file));
}


const FileListFile::UserDefinedData& FileListFile::userDefinedData() const
{
  return userDefinedDataValue;
}

void FileListFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

void FileListFile::userDefinedData( UserDefinedData &&userDefinedData)
{
  userDefinedDataValue = std::move( userDefinedData);
}

const std::optional< CheckValue>& FileListFile::checkValue() const
{
  return checkValueValue;
}

void FileListFile::checkValue( const std::optional< CheckValue> &value)
{
  checkValueValue = value;
}

void FileListFile::checkValue( std::optional< CheckValue> &&value)
{
  checkValueValue = std::move( value);
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

  const auto &otherFileList( other.files());

  if (filesValue.size() != otherFileList.size())
  {
    return false;
  }

  for ( unsigned int i = 0; i < filesValue.size(); ++i)
  {
    if (
      (filesValue[i].filename() != otherFileList[i].filename()) ||
      (filesValue[i].pathName() != otherFileList[i].pathName()))
    {
      return false;
    }

    switch ( fileType( filesValue[i].filename()))
    {
      case FileType::LoadList:
      case FileType::BatchList:
        // skip test of CRC and Member Sequence Number
        break;

      default:
        if (
          (filesValue[i].crc() != otherFileList[i].crc()) ||
          (filesValue[i].checkValue() != otherFileList[i].checkValue()) ||
          (filesValue[i].memberSequenceNumber() != otherFileList[i].memberSequenceNumber()))
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
  bool encodeV3Data{ false};
  std::size_t baseSize{ 0};

  switch ( arincVersion())
  {
    case SupportedArinc665Version::Supplement2:
      baseSize = FileHeaderSizeV2;
      break;

    case SupportedArinc665Version::Supplement34:
      encodeV3Data = true;
      baseSize = FileHeaderSizeV3;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));
  }

  RawFile rawFile( baseSize);


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
  const auto rawFilesInfo{ encodeFilesInfo( encodeV3Data)};
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
    nextFreeOffset += userDefinedDataValue.size();

    rawFile.insert(
      rawFile.end(),
      userDefinedDataValue.begin(),
      userDefinedDataValue.end());
  }

  setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);


  // Load Check Value (only in V3 mode)
  if (encodeV3Data)
  {
    // Alternative implementation set File Check Pointer to zero

    const auto rawCheckValue{ CheckValueUtils_encode( checkValueValue)};
    assert( rawCheckValue.size() % 2 == 0);

    setInt< uint32_t>(
      rawFile.begin() + FileCheckValuePointerFieldOffset,
      nextFreeOffset / 2);

    rawFile.insert(
      rawFile.end(),
      rawCheckValue.begin(),
      rawCheckValue.end());
  }


  // Resize to final size ( File CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t));

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void FileListFile::decodeBody( const RawFile &rawFile)
{
  bool decodeV3Data{ false};

  switch ( arincVersion())
  {
    case SupportedArinc665Version::Supplement2:
      break;

    case SupportedArinc665Version::Supplement34:
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));
  }


  // Spare Field
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

  uint32_t fileCheckValuePtr = 0;

  // only decode this pointers in V3 mode
  if (decodeV3Data)
  {
    getInt< uint32_t>(
      rawFile.begin() + FileCheckValuePointerFieldOffset,
      fileCheckValuePtr);
  }

  // media set part number
  auto it = decodeString(
    rawFile.begin() + mediaInformationPtr * 2,
    mediaSetPnValue);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  getInt< uint8_t>( it, numberOfMediaSetMembersValue);


  // file list
  decodeFilesInfo( rawFile, 2 * fileListPtr, decodeV3Data);


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    std::size_t endOfUserDefinedData{ rawFile.size() - DefaultChecksumPosition};

    if (fileCheckValuePtr != 0)
    {
      if (fileCheckValuePtr <= userDefinedDataPtr)
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << AdditionalInfo( "Invalid Pointers"));
      }

      endOfUserDefinedData = fileCheckValuePtr * 2;
    }

    userDefinedDataValue.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + endOfUserDefinedData);
  }


  // File Check Value Field (ARINC 665-3)
  checkValueValue.reset();
  if (decodeV3Data && (0U!=fileCheckValuePtr))
  {
    checkValueValue = CheckValueUtils_decode( rawFile, 2U * fileCheckValuePtr);
  }


  // file crc decoded and checked within base class
}

RawFile FileListFile::encodeFilesInfo( const bool encodeV3Data) const
{
  RawFile rawFilesInfo( sizeof( uint16_t));

  // number of files
  setInt< uint16_t>( rawFilesInfo.begin(), numberOfFiles());

  // iterate over files
  uint16_t fileCounter( 0);
  for (auto const &fileInfo : filesValue)
  {
    ++fileCounter;

    RawFile rawFileInfo( sizeof( uint16_t));

    // filename
    auto const rawFilename{ encodeString( fileInfo.filename())};
    assert( rawFilename.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawFilename.begin(),
      rawFilename.end());

    // path name
    auto const rawPathname{ encodeString( fileInfo.pathName())};
    assert( rawPathname.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawPathname.begin(),
      rawPathname.end());

    rawFileInfo.resize( rawFileInfo.size() + 2 * sizeof( uint16_t));

    // member sequence number
    auto fileInfoIt = setInt< uint16_t>(
      rawFileInfo.begin() + rawFileInfo.size() - ( 2 * sizeof( uint16_t)),
      fileInfo.memberSequenceNumber());

    // crc
    setInt< uint16_t>( fileInfoIt, fileInfo.crc());

    // following fields are available in ARINC 665-3 ff
    if (encodeV3Data)
    {
      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue())};
      assert( rawCheckValue.size() % 2 == 0);
      rawFileInfo.insert(
        rawFileInfo.end(),
        rawCheckValue.begin(),
        rawCheckValue.end());
    }

    // next file pointer (is set to 0 for last file)
    setInt< uint16_t>(
      rawFileInfo.begin(),
      (fileCounter == numberOfFiles()) ?
      (0U) :
      (rawFileInfo.size() / 2));

    // add file info to files info
    rawFilesInfo.insert( rawFilesInfo.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFilesInfo;
}

void FileListFile::decodeFilesInfo(
  const RawFile &rawFile,
  const std::size_t offset,
  const bool decodeV3Data)
{
  auto it{ rawFile.begin() + offset};

  // clear potentially data
  filesValue.clear();

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

    // CheckValue (keep default initialised if not V3 File Info
    std::optional< CheckValue> checkValue;

    // following fields are available in ARINC 665-3 ff
    if (decodeV3Data)
    {
      // check Value
      checkValue = CheckValueUtils_decode(
        rawFile,
        std::distance( rawFile.begin(), listIt));
    }


    // set it to begin of next file
    it += filePointer * 2;

    filesValue.emplace_back(
      filename,
      pathName,
      memberSequenceNumber,
      crc,
      checkValue);
  }
}

}

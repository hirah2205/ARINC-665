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

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

LoadHeaderFile::LoadHeaderFile( Arinc665Version version) :
  Arinc665File( FileType::LoadUploadHeader, version, FileCrcOffset),
  partFlagsValue( 0),
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

uint16_t LoadHeaderFile::partFlags() const
{
  return partFlagsValue;
}

void LoadHeaderFile::partFlags( uint16_t partFlags)
{
  partFlagsValue= partFlags;
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

const LoadHeaderFile::TargetHardwareIdPositions&
LoadHeaderFile::targetHardwareIdPositions() const
{
  return targetHardwareIdPositionsValue;
}

LoadHeaderFile::TargetHardwareIdPositions&
LoadHeaderFile::targetHardwareIdPositions()
{
  return targetHardwareIdPositionsValue;
}

void LoadHeaderFile::targetHardwareIdPositions(
  const TargetHardwareIdPositions &targetHardwareIdPositions)
{
  targetHardwareIdPositionsValue = targetHardwareIdPositions;
}

void LoadHeaderFile::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions)
{
  targetHardwareIdPositionsValue = std::move( targetHardwareIdPositions);
}

LoadHeaderFile::StringList LoadHeaderFile::targetHardwareIds() const
{
  StringList targetHardwareIds;

  for ( const auto &element : targetHardwareIdPositionsValue)
  {
    targetHardwareIds.push_back( element.first);
  }

  return targetHardwareIds;
}

void LoadHeaderFile::targetHardwareIds( const StringList &targetHardwareIds)
{
  for ( const auto& thwId : targetHardwareIds)
  {
    targetHardwareId( thwId);
  }
}

void LoadHeaderFile::targetHardwareId(
  const std::string &targetHardwareId,
  const StringList &positions)
{
  targetHardwareIdPositionsValue.insert(
    std::make_pair( targetHardwareId, positions));
}

void LoadHeaderFile::targetHardwareId(
  std::string &&targetHardwareId,
  StringList &&positions)
{
  targetHardwareIdPositionsValue.emplace(
    std::make_pair( std::move( targetHardwareId), std::move( positions)));
}

const LoadHeaderFile::LoadType& LoadHeaderFile::type() const
{
  return typeValue;
}

void LoadHeaderFile::loadType( const LoadType &type)
{
  typeValue = type;
}

void LoadHeaderFile::loadType( LoadType &&type)
{
  typeValue = std::move( type);
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
  bool encodeV3Data{ false};
  std::size_t baseSize{ 0};

  switch ( arincVersion())
  {
    case Arinc665Version::ARINC_665_1:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));

    case Arinc665Version::ARINC_665_2:
      // Spare
      baseSize = LoadHeaderSizeV2;
      break;

    case Arinc665Version::ARINC_665_3:
    case Arinc665Version::ARINC_665_4:
      // Part Flags
      encodeV3Data = true;
      baseSize = LoadHeaderSizeV3;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));
  }

  RawFile rawFile( baseSize);

  // Part Flags or Spare
  setInt< uint16_t>(
    rawFile.begin() + PartFlagsFieldOffset,
    encodeV3Data ? partFlagsValue : 0U);


  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ LoadHeaderSizeV2};


  // Load Part Number
  auto rawLoadPn{ encodeString( partNumber())};
  assert( rawLoadPn.size() % 2 == 0);

  setInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawLoadPn.size();

  rawFile.insert( rawFile.end(), rawLoadPn.begin(), rawLoadPn.end());

  // Load Type (only in V3 mode)
  if (encodeV3Data)
  {
    uint32_t loadTypePtr = 0;

    // Encode lode type only if set.
    if (typeValue)
    {
      loadTypePtr = nextFreeOffset / 2;

      const auto rawTypeDescription{ encodeString( typeValue->first)};

      // description
      rawFile.insert(
        rawFile.end(),
        rawTypeDescription.begin(),
        rawTypeDescription.end());

      rawFile.resize( rawFile.size() + sizeof( uint16_t));
      setInt< uint16_t>(
        rawFile.begin() + rawTypeDescription.size(),
        typeValue->second);

      nextFreeOffset += rawTypeDescription.size() + sizeof( uint16_t);
    }

    setInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffset,
      loadTypePtr);
  }

  // THW ID list
  auto rawThwIdsList{ encodeStringList( targetHardwareIds())};
  assert( rawThwIdsList.size() % 2 == 0);

  setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawThwIdsList.size();

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end());


  // THW ID + Positions (only in V3 mode)
  if (encodeV3Data)
  {
    // TODO
  }


  // data files list pointer
  auto rawDataFiles{ encodeFileList( dataFiles())};
  assert( rawDataFiles.size() % 2 == 0);

  setInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffset,
    nextFreeOffset / 2);
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


  // Load Check Value (only in V3 mode)
  {
    //! @todo add Load Check Values.
  }


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
  bool decodeV3Data{ false};

  uint32_t partFlags;
  getInt< uint32_t>(
    rawFile.begin() + PartFlagsFieldOffset,
    partFlags);

  switch ( arincVersion())
  {
    case Arinc665Version::ARINC_665_1:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));

    case Arinc665Version::ARINC_665_2:
      // Spare
      if (partFlags != 0U)
      {
        partFlagsValue = 0;
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << AdditionalInfo( "Spare not 0"));
      }
      break;

    case Arinc665Version::ARINC_665_3:
    case Arinc665Version::ARINC_665_4:
      partFlagsValue = partFlags;
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << AdditionalInfo( "Unsupported ARINC 665 Version"));
  }

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

  uint32_t loadTypeDescriptionPtr = 0;
  uint32_t thwIdsPositionPtr = 0;
  uint32_t loadCheckValuePtr = 0;

  // only decode this pointers in V3 mode
  if (decodeV3Data)
  {
    getInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffset,
      loadTypeDescriptionPtr);

    getInt< uint32_t>(
      rawFile.begin() + ThwIdPositionsPointerFieldOffset,
      thwIdsPositionPtr);

    getInt< uint32_t>(
      rawFile.begin() + LoadCheckValuePointerFieldOffset,
      loadCheckValuePtr);
  }


  // load part number
  decodeString( rawFile.begin() + loadPartNumberPtr * 2, partNumberValue);


  // Load Type Description Field (ARINC 665-3)
  if (decodeV3Data && (0!=loadTypeDescriptionPtr))
  {
    std::string loadTypeDescription;
    auto it{ decodeString(
      rawFile.begin() + loadTypeDescriptionPtr * 2,
      loadTypeDescription)};

    uint16_t loadTypeValue;
    getInt< uint16_t>( it, loadTypeValue);

    loadType( {std::make_pair(std::move( loadTypeDescription), loadTypeValue)});
  }


  // target hardware id list
  StringList targetHardwareIdsValue;

  decodeStringList(
    rawFile.begin() + targetHardwareIdListPtr * 2,
    targetHardwareIdsValue);

  targetHardwareIds( targetHardwareIdsValue);

  // THW IDs with Positions Field (ARINC 665-3)
  if (decodeV3Data && (0!=thwIdsPositionPtr))
  {
    uint16_t numberOfThwIdsWithPos;
    auto it = getInt< uint16_t>(
      rawFile.begin() + thwIdsPositionPtr * 2,
      numberOfThwIdsWithPos);

    for ( unsigned int thwIdIndex = 0; thwIdIndex < numberOfThwIdsWithPos; ++thwIdIndex)
    {
      std::string thwId;
      it = decodeString( it, thwId);

      StringList positions;
      it = decodeStringList( it, positions);

      targetHardwareIdPositionsValue.insert_or_assign(
        std::move( thwId),
        std::move( positions));
    }
  }


  // data file list
  dataFilesValue = decodeFileList( rawFile, dataFileListPtr * 2);


  // support file list
  if ( 0U != supportFileListPtr)
  {
    supportFilesValue = decodeFileList( rawFile, supportFileListPtr * 2);
  }


  // user defined data
  if ( 0U != userDefinedDataPtr)
  {
    std::size_t endOfUserDefinedData{ rawFile.size() - FileCrcOffset};

    if (loadCheckValuePtr != 0)
    {
      if (loadCheckValuePtr <= userDefinedDataPtr)
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << AdditionalInfo( "Invalid Pointers"));
      }

      endOfUserDefinedData = loadCheckValuePtr * 2;
    }

    userDefinedDataValue.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + endOfUserDefinedData);
  }

  if (decodeV3Data)
  {
    // TODO: add Load Check Value Field (ARINC 665-3)
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
  auto it{ rawFile.begin() + offset};

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

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::LoadHeaderFile.
 **/

#include "LoadHeaderFile.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/file/CheckValueUtils.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::File {

LoadHeaderFile::LoadHeaderFile( SupportedArinc665Version version) :
  Arinc665File{ version, FileCrcOffset},
  partFlagsV{ 0},
  loadCrcV{ 0}
{
}

LoadHeaderFile::LoadHeaderFile(
  SupportedArinc665Version version,
  std::string_view partNumber,
  const TargetHardwareIdPositions &targetHardwareIdPositions,
  const LoadFilesInfo &dataFilesInfo,
  const LoadFilesInfo &supportFilesInfo,
  const UserDefinedData &userDefinedData,
  uint32_t loadCrc):
  Arinc665File{ version, FileCrcOffset},
  partFlagsV{ 0 }, //! @todo add
  partNumberV{ partNumber },
  targetHardwareIdPositionsV{ targetHardwareIdPositions },
  dataFilesV{ dataFilesInfo },
  supportFilesV{ supportFilesInfo },
  userDefinedDataV{ userDefinedData },
  loadCrcV{ loadCrc }
{
  checkUserDefinedData();
}

LoadHeaderFile::LoadHeaderFile(
  SupportedArinc665Version version,
  std::string &&partNumber,
  TargetHardwareIdPositions &&targetHardwareIdPositions,
  LoadFilesInfo &&dataFilesInfo,
  LoadFilesInfo &&supportFilesInfo,
  UserDefinedData &&userDefinedData,
  uint32_t loadCrc):
  Arinc665File{ version, FileCrcOffset},
  partFlagsV{ 0 }, //! @todo add
  partNumberV{ std::move( partNumber ) },
  targetHardwareIdPositionsV{ std::move( targetHardwareIdPositions ) },
  dataFilesV{ std::move( dataFilesInfo ) },
  supportFilesV{ std::move( supportFilesInfo ) },
  userDefinedDataV{ std::move( userDefinedData ) },
  loadCrcV{ loadCrc }
{
  checkUserDefinedData();
}

LoadHeaderFile::LoadHeaderFile( const ConstRawFileSpan &rawFile):
  Arinc665File{ rawFile, FileType::LoadUploadHeader, FileCrcOffset},
  partFlagsV{ 0 },
  loadCrcV{ 0 }
{
  decodeBody( rawFile);
}

LoadHeaderFile& LoadHeaderFile::operator=( const ConstRawFileSpan &rawFile)
{
  // call inherited operator
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

FileType LoadHeaderFile::fileType() const noexcept
{
  return FileType::LoadUploadHeader;
}

uint16_t LoadHeaderFile::partFlags() const
{
  return partFlagsV;
}

void LoadHeaderFile::partFlags( uint16_t partFlags)
{
  partFlagsV = partFlags;
}

std::string_view LoadHeaderFile::partNumber() const
{
  return partNumberV;
}

void LoadHeaderFile::partNumber( std::string_view partNumber)
{
  partNumberV = partNumber;
}

void LoadHeaderFile::partNumber( std::string &&partNumber)
{
  partNumberV = std::move( partNumber);
}

const LoadHeaderFile::TargetHardwareIdPositions&
LoadHeaderFile::targetHardwareIdPositions() const
{
  return targetHardwareIdPositionsV;
}

LoadHeaderFile::TargetHardwareIdPositions&
LoadHeaderFile::targetHardwareIdPositions()
{
  return targetHardwareIdPositionsV;
}

void LoadHeaderFile::targetHardwareIdPositions(
  const TargetHardwareIdPositions &targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = targetHardwareIdPositions;
}

void LoadHeaderFile::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = std::move( targetHardwareIdPositions);
}

LoadHeaderFile::StringList LoadHeaderFile::targetHardwareIds() const
{
  BOOST_LOG_FUNCTION()

  StringList targetHardwareIds{};

  for ( const auto &element : targetHardwareIdPositionsV )
  {
    targetHardwareIds.push_back( element.first);
  }

  return targetHardwareIds;
}

void LoadHeaderFile::targetHardwareIds( const StringList &targetHardwareIds)
{
  for ( const auto &thwId : targetHardwareIds)
  {
    targetHardwareId( thwId);
  }
}

void LoadHeaderFile::targetHardwareId(
  std::string_view targetHardwareId,
  const StringList &positions)
{
  targetHardwareIdPositionsV.insert(
    std::make_pair( targetHardwareId, positions));
}

void LoadHeaderFile::targetHardwareId(
  std::string &&targetHardwareId,
  StringList &&positions)
{
  targetHardwareIdPositionsV.emplace(
    std::make_pair( std::move( targetHardwareId), std::move( positions)));
}

const LoadHeaderFile::LoadType& LoadHeaderFile::loadType() const
{
  return typeV;
}

void LoadHeaderFile::loadType( const LoadType &type)
{
  typeV = type;
}

void LoadHeaderFile::loadType( LoadType &&type)
{
  typeV = std::move( type);
}

const LoadFilesInfo& LoadHeaderFile::dataFiles() const
{
  return dataFilesV;
}

LoadFilesInfo& LoadHeaderFile::dataFiles()
{
  return dataFilesV;
}

void LoadHeaderFile::dataFile( const LoadFileInfo &dataFileInfo)
{
  dataFilesV.push_back( dataFileInfo);
}

void LoadHeaderFile::dataFile( LoadFileInfo &&dataFileInfo)
{
  dataFilesV.push_back( std::move( dataFileInfo));
}

const LoadFilesInfo& LoadHeaderFile::supportFiles() const
{
  return supportFilesV;
}

LoadFilesInfo& LoadHeaderFile::supportFiles()
{
  return supportFilesV;
}

void LoadHeaderFile::supportFile( const LoadFileInfo &supportFileInfo)
{
  supportFilesV.push_back( supportFileInfo);
}

void LoadHeaderFile::supportFile( LoadFileInfo &&supportFileInfo)
{
  supportFilesV.push_back( std::move( supportFileInfo));
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::userDefinedData() const
{
  return userDefinedDataV;
}

void LoadHeaderFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = userDefinedData;

  checkUserDefinedData();
}

void LoadHeaderFile::userDefinedData( UserDefinedData &&userDefinedData)
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData);

  checkUserDefinedData();
}

uint32_t LoadHeaderFile::loadCrc() const
{
  return loadCrcV;
}

void LoadHeaderFile::loadCrc( const uint32_t loadCrc)
{
  loadCrcV = loadCrc;
}

const std::optional< CheckValue>& LoadHeaderFile::loadCheckValue() const
{
  return loadCheckValueV;
}

void LoadHeaderFile::loadCheckValue( const std::optional< CheckValue> &value)
{
  loadCheckValueV = value;
}

void LoadHeaderFile::loadCheckValue( std::optional< CheckValue> &&value)
{
  loadCheckValueV = std::move( value);
}

RawFile LoadHeaderFile::encode() const
{
  BOOST_LOG_FUNCTION()

  bool encodeV3Data{ false};
  std::size_t baseSize{ 0};

  switch ( arincVersion())
  {
    case SupportedArinc665Version::Supplement2:
      // Spare
      baseSize = LoadHeaderSizeV2;
      break;

    case SupportedArinc665Version::Supplement34:
      // Part Flags
      encodeV3Data = true;
      baseSize = LoadHeaderSizeV3;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo( "Unsupported ARINC 665 Version"));
  }

  RawFile rawFile( baseSize);

  // Part Flags or Spare
  Helper::setInt< uint16_t>(
    rawFile.begin() + PartFlagsFieldOffset,
    encodeV3Data ? partFlagsV : 0U);


  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ rawFile.size()};


  // Load Part Number
  auto rawLoadPn{ encodeString( partNumber())};
  assert( rawLoadPn.size() % 2 == 0);

  Helper::setInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawLoadPn.size();

  rawFile.insert( rawFile.end(), rawLoadPn.begin(), rawLoadPn.end());


  // Load Type (only in V3 mode)
  if (encodeV3Data)
  {
    uint32_t loadTypePtr{ 0};

    // Encode lode type only if set.
    if ( typeV )
    {
      loadTypePtr = nextFreeOffset / 2;

      const auto rawTypeDescription{ encodeString( typeV->first)};
      assert( rawTypeDescription.size() % 2 == 0);

      // description
      rawFile.insert(
        rawFile.end(),
        rawTypeDescription.begin(),
        rawTypeDescription.end());

      rawFile.resize( rawFile.size() + sizeof( uint16_t ) );
      Helper::setInt< uint16_t>(
        rawFile.begin() + nextFreeOffset + rawTypeDescription.size(),
        typeV->second );

      nextFreeOffset += rawTypeDescription.size() + sizeof( uint16_t );
    }

    Helper::setInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffset,
      loadTypePtr);
  }

  // THW ID list
  auto rawThwIdsList{ encodeStringList( targetHardwareIds())};
  assert( rawThwIdsList.size() % 2 == 0);

  Helper::setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawThwIdsList.size();

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end());


  // THW ID + Positions (only in V3 mode)
  if (encodeV3Data)
  {
    unsigned int thwIdPosCount{ 0};
    RawFile rawThwPos( sizeof( uint16_t));

    for ( const auto &thwIdPosition : targetHardwareIdPositionsV )
    {
      if (thwIdPosition.second.empty())
      {
        continue;
      }

      // THW ID
      const auto rawThwId{ encodeString( thwIdPosition.first)};
      assert( rawThwId.size() % 2 == 0);

      rawThwPos.insert(
        rawThwPos.end(),
        rawThwId.begin(),
        rawThwId.end());

      // Positions
      const auto rawPositions{ encodeStringList( thwIdPosition.second)};
      assert( rawThwId.size() % 2 == 0);

      rawThwPos.insert(
        rawThwPos.end(),
        rawPositions.begin(),
        rawPositions.end());

      ++thwIdPosCount;
    }

    Helper::setInt< uint16_t>( rawThwPos.begin(), thwIdPosCount);

    uint32_t thwIdPosPtr{ 0};

    if (0 != thwIdPosCount)
    {
      thwIdPosPtr = nextFreeOffset / 2;
      rawFile.insert( rawFile.end(), rawThwPos.begin(), rawThwPos.end());
      nextFreeOffset += rawThwPos.size();
    }

    Helper::setInt< uint32_t>(
      rawFile.begin() + ThwIdPositionsPointerFieldOffset,
      thwIdPosPtr);
  }


  // data files list pointer
  auto rawDataFiles{ encodeFileList(
    dataFiles(),
    FileListType::Data,
    encodeV3Data)};
  assert( rawDataFiles.size() % 2 == 0);

  Helper::setInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawDataFiles.size();

  rawFile.insert( rawFile.end(), rawDataFiles.begin(), rawDataFiles.end());


  // support files (only if support files are present)
  auto rawSupportFiles{ encodeFileList(
    supportFiles(),
    FileListType::Support,
    encodeV3Data)};
  assert( rawSupportFiles.size() % 2 == 0);

  uint32_t supportFileListPtr{ 0};

  if (!supportFiles().empty())
  {
    supportFileListPtr = nextFreeOffset / 2;
    nextFreeOffset += rawSupportFiles.size();

    rawFile.insert(
      rawFile.end(),
      rawSupportFiles.begin(),
      rawSupportFiles.end());
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffset,
    supportFileListPtr);


  // user defined data pointer
  assert( userDefinedDataV.size() % 2 == 0);
  uint32_t userDefinedDataPtr{ 0};

  if (!userDefinedDataV.empty())
  {
    userDefinedDataPtr = nextFreeOffset / 2;
    nextFreeOffset += userDefinedDataV.size();

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end());
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);


  // Load Check Value (only in V3 mode)
  if ( encodeV3Data)
  {
    // Alternative implementation set Load Check Pointer to zero

    const auto rawCheckValue{ CheckValueUtils_encode( loadCheckValueV ) };
    assert( rawCheckValue.size() % 2 == 0);

    Helper::setInt< uint32_t>(
      rawFile.begin() + LoadCheckValuePointerFieldOffset,
      nextFreeOffset / 2);

    rawFile.insert(
      rawFile.end(),
      rawCheckValue.begin(),
      rawCheckValue.end());
  }


  // Resize to final size ( File CRC + Load CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t) + sizeof( uint32_t));

  // set header and crc
  insertHeader( rawFile);

  // load CRC

  Helper::setInt< uint32_t>(
    rawFile.begin() + (rawFile.size() - LoadCrcOffset),
    loadCrcV );

  return rawFile;
}

void LoadHeaderFile::decodeBody( const ConstRawFileSpan &rawFile)
{
  BOOST_LOG_FUNCTION()

  bool decodeV3Data{ false};

  uint32_t partFlags{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + PartFlagsFieldOffset,
    partFlags);

  switch ( arincVersion())
  {
    case SupportedArinc665Version::Supplement2:
      // Spare
      if (partFlags != 0U)
      {
        partFlagsV = 0;
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Spare not 0"));
      }
      break;

    case SupportedArinc665Version::Supplement34:
      partFlagsV = partFlags;
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo( "Unsupported ARINC 665 Version"));
  }

  uint32_t loadPartNumberPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffset,
    loadPartNumberPtr);

  uint32_t targetHardwareIdListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    targetHardwareIdListPtr);

  uint32_t dataFileListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffset,
    dataFileListPtr);

  uint32_t supportFileListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffset,
    supportFileListPtr);

  uint32_t userDefinedDataPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffset,
    userDefinedDataPtr);

  uint32_t loadTypeDescriptionPtr = 0;
  uint32_t thwIdsPositionPtr = 0;
  uint32_t loadCheckValuePtr = 0;

  // only decode this pointers in V3 mode
  if (decodeV3Data)
  {
    Helper::getInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffset,
      loadTypeDescriptionPtr);

    Helper::getInt< uint32_t>(
      rawFile.begin() + ThwIdPositionsPointerFieldOffset,
      thwIdsPositionPtr);

    Helper::getInt< uint32_t>(
      rawFile.begin() + LoadCheckValuePointerFieldOffset,
      loadCheckValuePtr);
  }


  // load part number
  decodeString( rawFile.begin() + loadPartNumberPtr * 2, partNumberV );


  // Load Type Description Field (ARINC 665-3)
  if (decodeV3Data && (0!=loadTypeDescriptionPtr))
  {
    std::string loadTypeDescription{};
    auto it{ decodeString(
      rawFile.begin() + loadTypeDescriptionPtr * 2,
      loadTypeDescription)};

    uint16_t loadTypeValue{};
    Helper::getInt< uint16_t>( it, loadTypeValue);

    loadType( {std::make_pair(std::move( loadTypeDescription), loadTypeValue)});
  }


  // target hardware id list
  StringList targetHardwareIdsValue{};

  decodeStringList(
    rawFile.begin() + targetHardwareIdListPtr * 2,
    targetHardwareIdsValue);

  targetHardwareIds( targetHardwareIdsValue);

  // THW IDs with Positions Field (ARINC 665-3)
  if (decodeV3Data && (0!=thwIdsPositionPtr))
  {
    uint16_t numberOfThwIdsWithPos{};
    auto it{ Helper::getInt< uint16_t>(
      rawFile.begin() + thwIdsPositionPtr * 2,
      numberOfThwIdsWithPos)};

    for ( unsigned int thwIdIndex = 0; thwIdIndex < numberOfThwIdsWithPos; ++thwIdIndex)
    {
      std::string thwId;
      it = decodeString( it, thwId);

      StringList positions;
      it = decodeStringList( it, positions);

      targetHardwareIdPositionsV.insert_or_assign(
        std::move( thwId),
        std::move( positions));
    }
  }


  // data file list
  dataFilesV = decodeFileList(
    rawFile,
    dataFileListPtr * 2,
    FileListType::Data,
    decodeV3Data);


  // support file list
  if ( 0U != supportFileListPtr)
  {
    supportFilesV = decodeFileList(
      rawFile,
      supportFileListPtr * 2,
      FileListType::Support,
      decodeV3Data);
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
          << Helper::AdditionalInfo( "Invalid Pointers"));
      }

      endOfUserDefinedData = loadCheckValuePtr * 2;
    }

    userDefinedDataV.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + endOfUserDefinedData);
  }

  // Load Check Value Field (ARINC 665-3)
  loadCheckValueV.reset();
  if (decodeV3Data && (0U!=loadCheckValuePtr))
  {
    loadCheckValueV =
      CheckValueUtils_decode( rawFile, 2 * loadCheckValuePtr);
  }


  // file crc decoded and checked within base class

  // load crc
  Helper::getInt< uint32_t>( rawFile.end() - LoadCrcOffset, loadCrcV );
}

RawFile LoadHeaderFile::encodeFileList(
  const LoadFilesInfo &loadFilesInfo,
  const FileListType type,
  const bool encodeV3Data) const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFileList( sizeof( uint16_t));

  // number of loads
  Helper::setInt< uint16_t>(
    rawFileList.begin(),
    Helper::safeCast< uint16_t>( loadFilesInfo.size()));

  // iterate over files
  uint16_t fileCounter{ 0};
  for (auto const &fileInfo : loadFilesInfo)
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

    // part number
    auto const rawPartNumber{ encodeString( fileInfo.partNumber())};
    assert( rawPartNumber.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawPartNumber.begin(),
      rawPartNumber.end());

    rawFileInfo.resize(
      rawFileInfo.size() + sizeof( uint32_t) + sizeof( uint16_t));

    // file length
    uint32_t fileLength{ 0};

    switch (type)
    {
      case FileListType::Data:
        // rounded number of 16-bit words
        fileLength = Helper::safeCast< uint32_t>( (fileInfo.length() + 1) / 2);
        break;

      case FileListType::Support:
        // number of bytes
        fileLength = fileInfo.length();
        break;

      default:
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Invalid List Type"));
    }

    Helper::setInt< uint32_t>(
      rawFileInfo.begin() + rawFileInfo.size() - (sizeof( uint32_t) + sizeof( uint16_t)),
      fileLength);

    // CRC
    Helper::setInt< uint16_t>(
      rawFileInfo.begin() + rawFileInfo.size() - sizeof( uint16_t),
      fileInfo.crc());

    // following fields are available in ARINC 665-3 ff
    if (encodeV3Data)
    {
      // length in bytes (Data File List)
      if (type == FileListType::Data)
      {
        rawFileInfo.resize( rawFileInfo.size() + sizeof( uint64_t));
        Helper::setInt<uint64_t>(
          rawFileInfo.begin() + rawFileInfo.size() - sizeof( uint64_t),
          fileInfo.length());
      }

      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue())};
      assert( rawCheckValue.size() % 2 == 0);
      rawFileInfo.insert(
        rawFileInfo.end(),
        rawCheckValue.begin(),
        rawCheckValue.end());
    }

    // next load pointer (is set to 0 for last load)
    Helper::setInt< uint16_t>(
      rawFileInfo.begin(),
      (fileCounter == loadFilesInfo.size()) ?
      (0U) :
      Helper::safeCast< uint16_t>( rawFileInfo.size() / 2));


    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end());
  }

  return rawFileList;
}

LoadFilesInfo LoadHeaderFile::decodeFileList(
  const ConstRawFileSpan &rawFile,
  const std::size_t offset,
  const FileListType type,
  const bool decodeV3Data)
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + offset};

  LoadFilesInfo files{};

  // number of data files
  uint16_t numberOfFiles{};
  it = Helper::getInt< uint16_t>( it, numberOfFiles);

  // iterate over file index
  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    auto listIt{ it};

    // next file pointer
    uint16_t filePointer{};
    listIt = Helper::getInt< uint16_t>( listIt, filePointer);

    // filename
    std::string name{};
    listIt = decodeString( listIt, name);

    // part number
    std::string partNumber{};
    listIt = decodeString( listIt, partNumber);

    // file length
    uint32_t length{};
    listIt = Helper::getInt< uint32_t>( listIt, length);

    uint64_t realLength{ 0};

    switch (type)
    {
      case FileListType::Data:
        // rounded number of 16-bit words
        realLength = length * 2;
        break;

      case FileListType::Support:
        // number of bytes
        realLength = length;
        break;

      default:
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo( "Invalid List Type"));
    }

    // CRC
    uint16_t crc{};
    listIt = Helper::getInt< uint16_t>( listIt, crc);

    // CheckValue (keep default initialised if not V3 File Info
    std::optional< CheckValue> checkValue;

    // following fields are available in ARINC 665-3 ff
    if (decodeV3Data)
    {
      // length in bytes (Data File List)
      if (type == FileListType::Data)
      {
        uint64_t fileLengthInBytes{};
        listIt = Helper::getInt< uint64_t>( listIt, fileLengthInBytes);
        realLength = fileLengthInBytes;
      }

      // check Value
      checkValue = CheckValueUtils_decode(
        rawFile,
        std::distance( rawFile.begin(), listIt));
    }

    // set it to begin of next file
    it += filePointer * 2U;

    // file info
    files.emplace_back( name, partNumber, realLength, crc, checkValue);
  }

  return files;
}

void LoadHeaderFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning)
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back(0U);
  }
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::LoadHeaderFile.
 **/

#include "LoadHeaderFile.hpp"

#include <arinc665/files/CheckValueUtils.hpp>
#include <arinc665/files/StringUtils.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::Files {

void LoadHeaderFile::encodeLoadCrc( RawFileSpan rawFile, const uint32_t crc )
{
  Helper::setInt< uint32_t>(
    rawFile.begin() + static_cast< ptrdiff_t>( rawFile.size() ) - LoadCrcOffset,
    crc );
}

uint32_t LoadHeaderFile::decodeLoadCrc( ConstRawFileSpan rawFile )
{
  uint32_t crc{ 0U };
  Helper::getInt< uint32_t>( rawFile.end() - LoadCrcOffset, crc );
  return crc;
}

void LoadHeaderFile::encodeLoadCheckValue(
  RawFileSpan rawFile,
  const Arinc645::CheckValue &checkValue )
{
  uint32_t loadCheckValuePtr{ 0U };

  Helper::getInt< uint32_t>(
    rawFile.begin() + LoadCheckValuePointerFieldOffsetV3,
    loadCheckValuePtr );

  if ( 0U == loadCheckValuePtr )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Load Check Value Ptr invalid" } );
  }

  // write load check value to position
  std::ranges::copy(
    CheckValueUtils_encode( checkValue ),
    rawFile.begin() + static_cast< ptrdiff_t >( loadCheckValuePtr ) * 2 );

  // Update File CRC, which is also calculated over Load Check Value
  const auto calculatedCrc{
    calculateChecksum( rawFile.first( rawFile.size() - FileCrcOffset ) ) };

  Helper::setInt< uint16_t >(
    rawFile.begin() + static_cast< ptrdiff_t>( rawFile.size() ) - FileCrcOffset,
    calculatedCrc );
}

Arinc645::CheckValue LoadHeaderFile::decodeLoadCheckValue(
  ConstRawFileSpan rawFile )
{
  uint32_t loadCheckValuePtr{ 0U };

  Helper::getInt< uint32_t>(
    rawFile.begin() + LoadCheckValuePointerFieldOffsetV3,
    loadCheckValuePtr );

  if ( 0U == loadCheckValuePtr )
  {
    return Arinc645::NoCheckValue;
  }

  return CheckValueUtils_decode(
    rawFile.subspan( static_cast< size_t >( loadCheckValuePtr ) * 2U ) );
}

LoadHeaderFile::LoadHeaderFile( const SupportedArinc665Version version ) :
  Arinc665File{ version, FileCrcOffset }
{
}

LoadHeaderFile::LoadHeaderFile( const ConstRawFileSpan &rawFile ):
  Arinc665File{ rawFile, FileType::LoadUploadHeader, FileCrcOffset }
{
  decodeBody( rawFile );
}

LoadHeaderFile& LoadHeaderFile::operator=( const ConstRawFileSpan &rawFile )
{
  // call inherited operator
  Arinc665File::operator =( rawFile );
  decodeBody( rawFile );

  return *this;
}

FileType LoadHeaderFile::fileType() const noexcept
{
  return FileType::LoadUploadHeader;
}

uint16_t LoadHeaderFile::partFlags() const noexcept
{
  return partFlagsV;
}

void LoadHeaderFile::partFlags( uint16_t partFlags ) noexcept
{
  partFlagsV = partFlags;
}

std::string_view LoadHeaderFile::partNumber() const
{
  return partNumberV;
}

void LoadHeaderFile::partNumber( std::string_view partNumber )
{
  partNumberV = partNumber;
}

void LoadHeaderFile::partNumber( std::string &&partNumber )
{
  partNumberV = std::move( partNumber );
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
  const TargetHardwareIdPositions &targetHardwareIdPositions )
{
  targetHardwareIdPositionsV = targetHardwareIdPositions;
}

void LoadHeaderFile::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions )
{
  targetHardwareIdPositionsV = std::move( targetHardwareIdPositions );
}

LoadHeaderFile::TargetHardwareIds LoadHeaderFile::targetHardwareIds() const
{
  BOOST_LOG_FUNCTION()

  TargetHardwareIds targetHardwareIds{};

  for ( const auto &[ targetHardwareId, positions ] : targetHardwareIdPositionsV )
  {
    targetHardwareIds.emplace( targetHardwareId );
  }

  return targetHardwareIds;
}

void LoadHeaderFile::targetHardwareIds(
  const TargetHardwareIds &targetHardwareIds )
{
  for ( const auto &thwId : targetHardwareIds )
  {
    targetHardwareId( thwId );
  }
}

void LoadHeaderFile::targetHardwareId(
  std::string_view targetHardwareId,
  const Positions &positions )
{
  targetHardwareIdPositionsV.insert_or_assign(
    TargetHardwareIdPositions::key_type{ targetHardwareId },
    positions );
}

void LoadHeaderFile::targetHardwareId(
  std::string &&targetHardwareId,
  Positions &&positions )
{
  targetHardwareIdPositionsV.insert_or_assign(
    std::move( targetHardwareId ),
    std::move( positions ) );
}

const LoadHeaderFile::LoadType& LoadHeaderFile::loadType() const
{
  return typeV;
}

void LoadHeaderFile::loadType( const LoadType &type )
{
  typeV = type;
}

void LoadHeaderFile::loadType( LoadType &&type )
{
  typeV = std::move( type);
}

const LoadFilesInfo& LoadHeaderFile::dataFiles() const noexcept
{
  return dataFilesV;
}

LoadFilesInfo& LoadHeaderFile::dataFiles() noexcept
{
  return dataFilesV;
}

void LoadHeaderFile::dataFile( const LoadFileInfo &dataFileInfo )
{
  dataFilesV.push_back( dataFileInfo);
}

void LoadHeaderFile::dataFile( LoadFileInfo &&dataFileInfo )
{
  dataFilesV.push_back( std::move( dataFileInfo ) );
}

const LoadFilesInfo& LoadHeaderFile::supportFiles() const noexcept
{
  return supportFilesV;
}

LoadFilesInfo& LoadHeaderFile::supportFiles() noexcept
{
  return supportFilesV;
}

void LoadHeaderFile::supportFile( const LoadFileInfo &supportFileInfo )
{
  supportFilesV.push_back( supportFileInfo );
}

void LoadHeaderFile::supportFile( LoadFileInfo &&supportFileInfo )
{
  supportFilesV.push_back( std::move( supportFileInfo ) );
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::userDefinedData() const
{
  return userDefinedDataV;
}

void LoadHeaderFile::userDefinedData( const UserDefinedData &userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = userDefinedData;

  checkUserDefinedData();
}

void LoadHeaderFile::userDefinedData( UserDefinedData &&userDefinedData)
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData );

  checkUserDefinedData();
}

Arinc645::CheckValueType LoadHeaderFile::loadCheckValueType() const
{
  return loadCheckValueTypeV;
}

void LoadHeaderFile::loadCheckValueType( const Arinc645::CheckValueType type )
{
  loadCheckValueTypeV = type;
}

RawFile LoadHeaderFile::encode() const
{
  BOOST_LOG_FUNCTION()

  bool encodeV3Data{ false };
  std::size_t baseSize{};

  switch ( arincVersion() )
  {
    case SupportedArinc665Version::Supplement2:
      // Spare
      baseSize = LoadHeaderSizeV2;
      break;

    case SupportedArinc665Version::Supplement345:
      // Part Flags
      encodeV3Data = true;
      baseSize = LoadHeaderSizeV3;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Unsupported ARINC 665 Version" } );
  }

  RawFile rawFile( baseSize );

  // Part Flags or Spare
  Helper::setInt< uint16_t>(
    rawFile.begin() + PartFlagsFieldOffsetV3,
    encodeV3Data ? partFlagsV : 0U );


  // Next free Offset (used for optional pointer calculation)
  ptrdiff_t nextFreeOffset{ static_cast< ptrdiff_t>( rawFile.size() ) };


  // Load Part Number
  auto rawLoadPn{ StringUtils_encodeString( partNumber() ) };
  assert( rawLoadPn.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2 ) );
  nextFreeOffset += static_cast< ptrdiff_t>( rawLoadPn.size() );

  rawFile.insert( rawFile.end(), rawLoadPn.begin(), rawLoadPn.end() );


  // Load Type (only in V3 mode)
  if ( encodeV3Data )
  {
    uint32_t loadTypePtr{ 0 };

    // Encode lode type only if set.
    if ( typeV )
    {
      loadTypePtr = static_cast< uint32_t >( nextFreeOffset / 2 );

      const auto rawTypeDescription{ StringUtils_encodeString( typeV->first ) };
      assert( rawTypeDescription.size() % 2 == 0 );

      // description
      rawFile.insert(
        rawFile.end(),
        rawTypeDescription.begin(),
        rawTypeDescription.end() );

      rawFile.resize( rawFile.size() + sizeof( uint16_t ) );
      Helper::setInt< uint16_t>(
        rawFile.begin() + nextFreeOffset + static_cast< ptrdiff_t>( rawTypeDescription.size() ),
        typeV->second );

      nextFreeOffset += static_cast< ptrdiff_t>( rawTypeDescription.size() + sizeof( uint16_t ) );
    }

    Helper::setInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffsetV3,
      loadTypePtr );
  }

  // THW ID list
  auto rawThwIdsList{ StringUtils_encodeStrings( targetHardwareIds() ) };
  assert( rawThwIdsList.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2 ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawThwIdsList.size() );

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end() );


  // THW ID + Positions (only in V3 mode)
  if ( encodeV3Data )
  {
    uint16_t thwIdPosCount{ 0 };
    RawFile rawThwPos( sizeof( uint16_t ) );

    for ( const auto &[ thwId, positions ] : targetHardwareIdPositionsV )
    {
      // skip if no positions are stored
      if ( positions.empty() )
      {
        continue;
      }

      // THW ID
      const auto rawThwId{ StringUtils_encodeString( thwId ) };
      assert( rawThwId.size() % 2 == 0 );

      rawThwPos.insert(
        rawThwPos.end(),
        rawThwId.begin(),
        rawThwId.end() );

      // Positions
      const auto rawPositions{ StringUtils_encodeStrings( positions ) };
      assert( rawThwId.size() % 2 == 0 );

      rawThwPos.insert(
        rawThwPos.end(),
        rawPositions.begin(),
        rawPositions.end() );

      ++thwIdPosCount;
    }

    Helper::setInt< uint16_t>( rawThwPos.begin(), thwIdPosCount );

    uint32_t thwIdPosPtr{ 0 };

    if ( 0 != thwIdPosCount )
    {
      thwIdPosPtr = static_cast< uint32_t >( nextFreeOffset / 2 );
      rawFile.insert( rawFile.end(), rawThwPos.begin(), rawThwPos.end() );
      nextFreeOffset += static_cast< ptrdiff_t>( rawThwPos.size() );
    }

    Helper::setInt< uint32_t>(
      rawFile.begin() + ThwIdPositionsPointerFieldOffsetV3,
      thwIdPosPtr );
  }


  // data files list pointer
  auto rawDataFiles{ encodeDataFiles( encodeV3Data ) };
  assert( rawDataFiles.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2 ) );
  nextFreeOffset += static_cast< ptrdiff_t>( rawDataFiles.size() );

  rawFile.insert( rawFile.end(), rawDataFiles.begin(), rawDataFiles.end() );


  // support files (only if support files are present)
  auto rawSupportFiles{ encodeSupportFiles( encodeV3Data ) };
  assert( rawSupportFiles.size() % 2 == 0 );

  uint32_t supportFileListPtr{ 0 };

  if ( !supportFiles().empty() )
  {
    supportFileListPtr = static_cast< uint32_t >( nextFreeOffset / 2 );
    nextFreeOffset += static_cast< ptrdiff_t>( rawSupportFiles.size() );

    rawFile.insert(
      rawFile.end(),
      rawSupportFiles.begin(),
      rawSupportFiles.end());
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffsetV2,
    supportFileListPtr );


  // user defined data pointer
  assert( userDefinedDataV.size() % 2 == 0 );
  uint32_t userDefinedDataPtr{ 0 };

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = static_cast< uint32_t >( nextFreeOffset / 2 );
    nextFreeOffset += static_cast< ptrdiff_t>( userDefinedDataV.size() );

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end() );
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr );

  // amount of data of Check Values and CRCs
  uint32_t checkValueCrcSizes{ sizeof( uint16_t ) + sizeof( uint32_t ) };

  // Load Check Value (only in V3 mode)
  if ( encodeV3Data )
  {
    // Alternative implementation set Load Check Pointer to zero, when Load
    // Check Value is not given

    checkValueCrcSizes += CheckValueUtils_size( loadCheckValueTypeV );

    // Set Pointer to Load Check Value Field
    Helper::setInt< uint32_t >(
      rawFile.begin() + LoadCheckValuePointerFieldOffsetV3,
      static_cast< uint32_t >( nextFreeOffset / 2 ) );

    // actual check value must be encoded by external means
  }


  // Resize to final size ( Check Value + File CRC + Load CRC)
  rawFile.resize( rawFile.size() + checkValueCrcSizes );

  // set header and crc
  insertHeader( rawFile );

  // load CRC must be encoded by external means

  return rawFile;
}

void LoadHeaderFile::decodeBody( const ConstRawFileSpan &rawFile )
{
  BOOST_LOG_FUNCTION()

  bool decodeV3Data{ false };

  uint16_t partFlags{};
  Helper::getInt< uint16_t>(
    rawFile.begin() + PartFlagsFieldOffsetV3,
    partFlags );

  switch ( arincVersion() )
  {
    case SupportedArinc665Version::Supplement2:
      // Spare
      if ( partFlags != 0U )
      {
        partFlagsV = 0;
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Spare not 0" } );
      }
      break;

    case SupportedArinc665Version::Supplement345:
      partFlagsV = partFlags;
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Unsupported ARINC 665 Version" } );
  }

  uint32_t loadPartNumberPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + LoadPartNumberPointerFieldOffsetV2,
    loadPartNumberPtr);

  uint32_t targetHardwareIdListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffsetV2,
    targetHardwareIdListPtr);

  uint32_t dataFileListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + DataFilesPointerFieldOffsetV2,
    dataFileListPtr);

  uint32_t supportFileListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + SupportFilesPointerFieldOffsetV2,
    supportFileListPtr);

  uint32_t userDefinedDataPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr);

  uint32_t loadTypeDescriptionPtr{ 0U };
  uint32_t thwIdsPositionPtr{ 0U };
  uint32_t loadCheckValuePtr{ 0U };

  // only decode this pointers in V3 mode
  if ( decodeV3Data )
  {
    Helper::getInt< uint32_t>(
      rawFile.begin() + LoadTypeDescriptionPointerFieldOffsetV3,
      loadTypeDescriptionPtr );

    Helper::getInt< uint32_t>(
      rawFile.begin() + ThwIdPositionsPointerFieldOffsetV3,
      thwIdsPositionPtr );

    Helper::getInt< uint32_t>(
      rawFile.begin() + LoadCheckValuePointerFieldOffsetV3,
      loadCheckValuePtr );
  }


  // load part number
  StringUtils_decodeString(
    rawFile.begin() + static_cast< ptrdiff_t >( loadPartNumberPtr ) * 2,
    partNumberV );


  // Load Type Description Field (ARINC 665-3)
  if ( decodeV3Data && ( 0!=loadTypeDescriptionPtr ) )
  {
    std::string loadTypeDescription{};
    auto it{ StringUtils_decodeString(
      rawFile.begin() + static_cast< ptrdiff_t >( loadTypeDescriptionPtr ) * 2,
      loadTypeDescription ) };

    uint16_t loadTypeValue{};
    Helper::getInt< uint16_t>( it, loadTypeValue );

    loadType( {std::make_pair(std::move( loadTypeDescription ), loadTypeValue ) } );
  }


  // target hardware id list
  TargetHardwareIds targetHardwareIdsValue{};

  StringUtils_decodeStrings(
    rawFile.begin() + static_cast< ptrdiff_t >( targetHardwareIdListPtr ) * 2,
    targetHardwareIdsValue);

  targetHardwareIds( targetHardwareIdsValue );

  // THW IDs with Positions Field (ARINC 665-3)
  if ( decodeV3Data && ( 0U != thwIdsPositionPtr ) )
  {
    uint16_t numberOfThwIdsWithPos{};
    auto it{ Helper::getInt< uint16_t >(
      rawFile.begin() + static_cast< ptrdiff_t >( thwIdsPositionPtr ) * 2,
      numberOfThwIdsWithPos ) };

    for ( uint16_t thwIdIndex = 0; thwIdIndex < numberOfThwIdsWithPos; ++thwIdIndex )
    {
      std::string thwId{};
      it = StringUtils_decodeString( it, thwId );

      Positions positions{};
      it = StringUtils_decodeStrings( it, positions );

      targetHardwareId( std::move( thwId ), std::move( positions ) );
    }
  }


  // data file list
  decodeDataFiles(
    rawFile.subspan( static_cast< ptrdiff_t >( dataFileListPtr ) * 2 ),
    decodeV3Data );


  // support file list
  if ( 0U != supportFileListPtr )
  {
    decodeSupportFiles(
      rawFile.subspan( static_cast< ptrdiff_t >( supportFileListPtr ) * 2 ),
      decodeV3Data );
  }


  // user defined data
  if ( 0U != userDefinedDataPtr )
  {
    ptrdiff_t endOfUserDefinedData{ static_cast< ptrdiff_t>( rawFile.size() ) - FileCrcOffset };

    if (loadCheckValuePtr != 0)
    {
      if (loadCheckValuePtr <= userDefinedDataPtr)
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "Invalid Pointers" } );
      }

      endOfUserDefinedData = static_cast< ptrdiff_t >( loadCheckValuePtr ) * 2;
    }

    userDefinedDataV.assign(
      rawFile.begin() + static_cast< ptrdiff_t >( userDefinedDataPtr ) * 2,
      rawFile.begin() + endOfUserDefinedData );
  }

  // Load Check Value Field (ARINC 665-3) - Only Type is stored.
  // Check must be performed by other means
  loadCheckValueTypeV = Arinc645::CheckValueType::NotUsed;
  if ( decodeV3Data && ( 0U!=loadCheckValuePtr ) )
  {
    loadCheckValueTypeV = std::get< 0 >( CheckValueUtils_decode(
      rawFile.subspan( static_cast< size_t >( loadCheckValuePtr ) * 2U ) ) );
  }


  // file crc decoded and checked within base class

  // load crc is not decoded - this must be done by other means
}

RawFile LoadHeaderFile::encodeDataFiles( const bool encodeV3Data ) const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFileList( sizeof( uint16_t ) );

  // Number of files must not exceed field
  if ( dataFilesV.size() > std::numeric_limits< uint16_t >::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More files than allowed" } );
  }

  // number of files
  Helper::setInt< uint16_t >(
    rawFileList.begin(),
    Helper::safeCast< uint16_t >( dataFilesV.size() ) );

  // iterate over files
  uint16_t fileCounter{ 0 };
  for ( auto const &fileInfo : dataFilesV )
  {
    ++fileCounter;

    RawFile rawFileInfo( sizeof( uint16_t ) );

    // filename
    auto const rawFilename{ StringUtils_encodeString( fileInfo.filename ) };
    assert( rawFilename.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawFilename.begin(),
      rawFilename.end() );

    // part number
    auto const rawPartNumber{ StringUtils_encodeString( fileInfo.partNumber ) };
    assert( rawPartNumber.size() % 2 == 0 );
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawPartNumber.begin(),
      rawPartNumber.end() );

    // resize for file length, CRC
    rawFileInfo.resize(
      rawFileInfo.size() + sizeof( uint32_t ) + sizeof( uint16_t ) );

    // file length - rounded number of 16-bit words
    const uint32_t fileLength{
      Helper::safeCast< uint32_t>( ( fileInfo.length + 1U ) / 2U ) };

    Helper::setInt< uint32_t>(
      rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - ( sizeof( uint32_t ) + sizeof( uint16_t ) ),
      fileLength );

    // CRC
    Helper::setInt< uint16_t>(
      rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - sizeof( uint16_t ),
      fileInfo.crc );

    // following fields are available in ARINC 665-3 ff
    if ( encodeV3Data )
    {
      // length in bytes (Data File List)
      rawFileInfo.resize( rawFileInfo.size() + sizeof( uint64_t ) );
      Helper::setInt<uint64_t>(
        rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - sizeof( uint64_t ),
        fileInfo.length );

      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue ) };
      assert( rawCheckValue.size() % 2 == 0 );
      rawFileInfo.insert(
        rawFileInfo.end(),
        rawCheckValue.begin(),
        rawCheckValue.end() );
    }

    // next load pointer (is set to 0 for last load)
    Helper::setInt< uint16_t>(
      rawFileInfo.begin(),
      ( fileCounter == dataFilesV.size() ) ?
        0U :
        Helper::safeCast< uint16_t>( rawFileInfo.size() / 2U ) );

    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end() );
  }

  return rawFileList;
}

RawFile LoadHeaderFile::encodeSupportFiles( const bool encodeV3Data ) const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFileList( sizeof( uint16_t ) );

  // Number of files must not exceed field
  if ( supportFilesV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More files than allowed" } );
  }

  // number of loads
  Helper::setInt< uint16_t>(
    rawFileList.begin(),
    Helper::safeCast< uint16_t>( supportFilesV.size() ) );

  // iterate over files
  uint16_t fileCounter{ 0 };
  for ( auto const &fileInfo : supportFilesV )
  {
    ++fileCounter;

    RawFile rawFileInfo( sizeof( uint16_t ) );

    // filename
    auto const rawFilename{ StringUtils_encodeString( fileInfo.filename ) };
    assert( rawFilename.size() % 2 == 0 );
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawFilename.begin(),
      rawFilename.end() );

    // part number
    auto const rawPartNumber{ StringUtils_encodeString( fileInfo.partNumber ) };
    assert( rawPartNumber.size() % 2 == 0);
    rawFileInfo.insert(
      rawFileInfo.end(),
      rawPartNumber.begin(),
      rawPartNumber.end() );

    // resize for file length and CRC
    rawFileInfo.resize(
      rawFileInfo.size() + sizeof( uint32_t) + sizeof( uint16_t) );

    // file length in number of bytes
    uint32_t fileLength{ Helper::safeCast< uint32_t>( fileInfo.length ) };

    Helper::setInt< uint32_t>(
      rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - (sizeof( uint32_t) + sizeof( uint16_t)),
      fileLength );

    // CRC
    Helper::setInt< uint16_t>(
      rawFileInfo.begin() + static_cast< ptrdiff_t>( rawFileInfo.size() ) - sizeof( uint16_t ),
      fileInfo.crc );

    // following fields are available in ARINC 665-3 ff
    if ( encodeV3Data )
    {
      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue ) };
      assert( rawCheckValue.size() % 2 == 0 );
      rawFileInfo.insert(
        rawFileInfo.end(),
        rawCheckValue.begin(),
        rawCheckValue.end() );
    }

    // next load pointer (is set to 0 for last load)
    Helper::setInt< uint16_t>(
      rawFileInfo.begin(),
      ( fileCounter == supportFilesV.size() ) ?
        0U :
        Helper::safeCast< uint16_t>( rawFileInfo.size() / 2U ) );

    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end() );
  }

  return rawFileList;
}

void LoadHeaderFile::decodeDataFiles(
  const ConstRawFileSpan &rawFile,
  const bool decodeV3Data )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() };

  // clear data files
  dataFilesV.clear();

  // number of data files
  uint16_t numberOfFiles{};
  it = Helper::getInt< uint16_t>( it, numberOfFiles );

  // iterate over file index
  for ( uint16_t fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex )
  {
    auto listIt{ it };

    // next file pointer
    uint16_t filePointer{};
    listIt = Helper::getInt< uint16_t>( listIt, filePointer );

    // check file pointer for validity
    if ( fileIndex != numberOfFiles - 1U )
    {
      if ( filePointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is 0" } );
      }
    }
    else
    {
      if ( filePointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is not 0" } );
      }
    }

    // filename
    std::string name{};
    listIt = StringUtils_decodeString( listIt, name);

    // part number
    std::string partNumber{};
    listIt = StringUtils_decodeString( listIt, partNumber);

    // file length
    uint32_t length{};
    listIt = Helper::getInt< uint32_t>( listIt, length );

    // rounded number of 16-bit words
    uint64_t realLength{  static_cast< uint64_t >( length ) * 2U };

    // CRC
    uint16_t crc{};
    listIt = Helper::getInt< uint16_t>( listIt, crc );

    // CheckValue - if not decoded no check value
    Arinc645::CheckValue checkValue{ Arinc645::NoCheckValue };

    // following fields are available in ARINC 665-3 ff
    if ( decodeV3Data )
    {
      // length in bytes (Data File List)
      uint64_t fileLengthInBytes{};
      listIt = Helper::getInt< uint64_t>( listIt, fileLengthInBytes );

      // check length fields for consistency
      if ( ( ( fileLengthInBytes + 1 ) / 2 ) <= std::numeric_limits< uint32_t >::max()
        && ( length != ( static_cast< uint32_t >( (fileLengthInBytes + 1) / 2 ) ) ) )
      {
        BOOST_THROW_EXCEPTION( Arinc665Exception()
          << Helper::AdditionalInfo{ "Inconsistent length fields" } );
      }

      // update real length
      realLength = fileLengthInBytes;

      // check Value
      checkValue = CheckValueUtils_decode(
        rawFile.subspan( std::distance( rawFile.begin(), listIt ) ) );
    }

    // set it to begin of next file
    it += static_cast< ptrdiff_t >( filePointer ) * 2;

    // file info
    dataFilesV.emplace_back( LoadFileInfo{
      .filename = std::move( name ),
      .partNumber = std::move( partNumber ),
      .length = realLength,
      .crc = crc,
      .checkValue = std::move( checkValue ) } );
  }
}

void LoadHeaderFile::decodeSupportFiles(
  const ConstRawFileSpan &rawFile,
  bool decodeV3Data )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() };

  // clear support files
  supportFilesV.clear();

  // number of data files
  uint16_t numberOfFiles{};
  it = Helper::getInt< uint16_t>( it, numberOfFiles );

  // iterate over file index
  for ( uint16_t fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex )
  {
    auto listIt{ it };

    // next file pointer
    uint16_t filePointer{};
    listIt = Helper::getInt< uint16_t>( listIt, filePointer );

    // check file pointer for validity
    if ( fileIndex != numberOfFiles - 1U )
    {
      if ( filePointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is 0" } );
      }
    }
    else
    {
      if ( filePointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next file pointer is not 0" } );
      }
    }

    // filename
    std::string name{};
    listIt = StringUtils_decodeString( listIt, name);

    // part number
    std::string partNumber{};
    listIt = StringUtils_decodeString( listIt, partNumber);

    // file length
    uint32_t length{};
    listIt = Helper::getInt< uint32_t>( listIt, length);

    // CRC
    uint16_t crc{};
    listIt = Helper::getInt< uint16_t>( listIt, crc );

    // CheckValue - if not decoded no check value
   Arinc645::CheckValue checkValue{ Arinc645::NoCheckValue };

    // following fields are available in ARINC 665-3 ff
    if ( decodeV3Data )
    {
      // check Value
      checkValue = CheckValueUtils_decode(
        rawFile.subspan( std::distance( rawFile.begin(), listIt ) ) );
    }

    // set it to begin of next file
    it += static_cast< ptrdiff_t >( filePointer ) * 2;

    // file info
    supportFilesV.emplace_back( LoadFileInfo{
      std::move( name ),
      std::move( partNumber ),
      length,
      crc,
      std::move( checkValue ) } );
  }
}

void LoadHeaderFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back( 0U );
  }
}

}

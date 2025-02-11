// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::LoadHeaderFile.
 **/

#include "LoadHeaderFile.hpp"

#include <arinc_665/files/CheckValueUtils.hpp>
#include <arinc_665/files/StringUtils.hpp>

#include <arinc_665/Arinc665Exception.hpp>
#include <arinc_665/Logger.hpp>

#include <arinc_645/CheckValueGenerator.hpp>

#include <helper/Exception.hpp>
#include <helper/RawData.hpp>
#include <helper/SafeCast.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Files {

void LoadHeaderFile::processLoadCrc( ConstRawDataSpan rawFile, Arinc645::Arinc645Crc32 &loadCrc )
{
  loadCrc.process_bytes( std::data( rawFile ), rawFile.size() - LoadCrcOffset );
}

void LoadHeaderFile::encodeLoadCrc( RawDataSpan rawFile, const uint32_t crc )
{
  Helper::RawData_setInt< uint32_t >( rawFile.last( LoadCrcOffset ), crc );
}

uint32_t LoadHeaderFile::decodeLoadCrc( ConstRawDataSpan rawFile )
{
  auto [ _, crc ]{ Helper::RawData_getInt< uint32_t >( rawFile.last( LoadCrcOffset ) ) };
  return crc;
}

void
LoadHeaderFile::processLoadCheckValue( ConstRawDataSpan rawFile, Arinc645::CheckValueGenerator &checkValueGenerator )
{
  if ( loadFileFormatVersion( rawFile ) != LoadFileFormatVersion::Version345 )
  {
    // load check value can only be processed on ARINC 665-3/4/5 files
    return;
  }

  // Obtain Load Check Value Pointer
  auto [ _, loadCheckValuePtr ]{
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadCheckValuePointerFieldOffsetV3 ) ) };

  if ( 0U == loadCheckValuePtr )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{} << Helper::AdditionalInfo{ "Load Check Value Ptr invalid" } );
  }

  // process check value
  checkValueGenerator.process( rawFile.first( static_cast< size_t >( loadCheckValuePtr ) * 2U ) );
}

void LoadHeaderFile::encodeLoadCheckValue( RawDataSpan rawFile, const Arinc645::CheckValue &checkValue )
{
  if ( loadFileFormatVersion( rawFile ) != LoadFileFormatVersion::Version345 )
  {
    // load check value can only be stored on ARINC 665-3/4/5 files
    return;
  }

  // Obtain Load Check Value Pointer
  auto [ _, loadCheckValuePtr ]{
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadCheckValuePointerFieldOffsetV3 ) ) };

  if ( 0U == loadCheckValuePtr )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{} << Helper::AdditionalInfo{ "Load Check Value Ptr invalid" } );
  }

  // write load check value to position
  std::ranges::copy(
    CheckValueUtils_encode( checkValue ),
    rawFile.begin() + static_cast< ptrdiff_t >( loadCheckValuePtr ) * 2 );

  // Update File CRC, which is also calculated over Load Check Value
  const auto calculatedCrc{ calculateChecksum( rawFile.first( rawFile.size() - FileCrcOffset ) ) };

  Helper::RawData_setInt< uint16_t >( rawFile.last( FileCrcOffset ), calculatedCrc );
}

Arinc645::CheckValue LoadHeaderFile::decodeLoadCheckValue( ConstRawDataSpan rawFile )
{
  if ( loadFileFormatVersion( rawFile ) != LoadFileFormatVersion::Version345 )
  {
    return Arinc645::CheckValue::NoCheckValue;
  }

  auto [ _, loadCheckValuePtr ]{
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadCheckValuePointerFieldOffsetV3 ) ) };

  if ( 0U == loadCheckValuePtr )
  {
    return Arinc645::CheckValue::NoCheckValue;
  }

  return CheckValueUtils_decode( rawFile.subspan( static_cast< size_t >( loadCheckValuePtr ) * 2U ) );
}

LoadHeaderFile::LoadHeaderFile( const SupportedArinc665Version version ) :
  Arinc665File{ version, FileCrcOffset }
{
}

LoadHeaderFile::LoadHeaderFile( ConstRawDataSpan rawFile ) :
  Arinc665File{ rawFile, FileType::LoadUploadHeader, FileCrcOffset }
{
  decodeBody( rawFile );
}

LoadHeaderFile& LoadHeaderFile::operator=( ConstRawDataSpan rawFile )
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

void LoadHeaderFile::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
}

const LoadHeaderFile::TargetHardwareIds& LoadHeaderFile::targetHardwareIds() const
{
  return targetHardwareIdsV;
}

LoadHeaderFile::TargetHardwareIds& LoadHeaderFile::targetHardwareIds()
{
  return targetHardwareIdsV;
}

void LoadHeaderFile::targetHardwareIds( TargetHardwareIds targetHardwareIds )
{
  targetHardwareIdsV = std::move( targetHardwareIds );
}

void LoadHeaderFile::targetHardwareId( std::string targetHardwareId )
{
  targetHardwareIdsV.emplace_back( std::move( targetHardwareId ) );
}

const LoadHeaderFile::TargetHardwareIdsPositions& LoadHeaderFile::targetHardwareIdsPositions() const
{
  return targetHardwareIdsPositionsV;
}

LoadHeaderFile::TargetHardwareIdsPositions& LoadHeaderFile::targetHardwareIdsPositions()
{
  return targetHardwareIdsPositionsV;
}

void LoadHeaderFile::targetHardwareIdsPositions( TargetHardwareIdsPositions targetHardwareIdsPositions )
{
  targetHardwareIdsPositionsV = std::move( targetHardwareIdsPositions );
}

void LoadHeaderFile::targetHardwareIdPositions( std::string targetHardwareId, Positions positions )
{
  targetHardwareIdsPositionsV.emplace_back( std::move( targetHardwareId ), std::move( positions ) );
}

const LoadHeaderFile::LoadType& LoadHeaderFile::loadType() const
{
  return typeV;
}

void LoadHeaderFile::loadType( LoadType type )
{
  typeV = std::move( type );
}

const LoadFilesInfo& LoadHeaderFile::dataFiles() const noexcept
{
  return dataFilesV;
}

LoadFilesInfo& LoadHeaderFile::dataFiles() noexcept
{
  return dataFilesV;
}

void LoadHeaderFile::dataFiles( LoadFilesInfo filesInformation )
{
  dataFilesV = std::move( filesInformation );
}

void LoadHeaderFile::dataFile( LoadFileInfo dataFileInfo )
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

void LoadHeaderFile::supportFiles( LoadFilesInfo filesInformation )
{
  supportFilesV = std::move( filesInformation );
}

void LoadHeaderFile::supportFile( LoadFileInfo supportFileInfo )
{
  supportFilesV.push_back( std::move( supportFileInfo ) );
}

ConstUserDefinedDataSpan LoadHeaderFile::userDefinedData() const
{
  return userDefinedDataV;
}

void LoadHeaderFile::userDefinedData( UserDefinedData userDefinedData )
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

RawData LoadHeaderFile::encode() const
{
  BOOST_LOG_FUNCTION()

  bool encodeV3Data{ false };
  std::size_t baseSize;

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
      BOOST_THROW_EXCEPTION( Arinc665Exception{} << Helper::AdditionalInfo{ "Unsupported ARINC 665 Version" } );
  }

  RawData rawFile( baseSize );

  // Part Flags or Spare
  Helper::RawData_setInt< uint16_t >(
    RawDataSpan{ rawFile }.subspan( PartFlagsFieldOffsetV3 ),
    encodeV3Data ? partFlagsV : 0U );

  // Next free Offset (used for optional pointer calculation)
  ptrdiff_t nextFreeOffset{ static_cast< ptrdiff_t>( rawFile.size() ) };

  // Load Part Number
  auto rawLoadPn{ StringUtils_encodeString( partNumber() ) };
  assert( rawLoadPn.size() % 2 == 0 );

  Helper::RawData_setInt< uint32_t >(
    RawDataSpan{ rawFile }.subspan( LoadPartNumberPointerFieldOffsetV2 ),
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
      rawFile.insert( rawFile.end(), rawTypeDescription.begin(), rawTypeDescription.end() );

      rawFile.resize( rawFile.size() + sizeof( uint16_t ) );
      Helper::RawData_setInt< uint16_t >(
        RawDataSpan{ rawFile }.subspan( nextFreeOffset + static_cast< ptrdiff_t >( rawTypeDescription.size() ) ),
        typeV->second );

      nextFreeOffset += static_cast< ptrdiff_t>( rawTypeDescription.size() + sizeof( uint16_t ) );
    }

    Helper::RawData_setInt< uint32_t >(
      RawDataSpan{ rawFile }.subspan( LoadTypeDescriptionPointerFieldOffsetV3 ),
      loadTypePtr );
  }

  // THW ID list
  auto rawThwIdsList{ StringUtils_encodeStrings( targetHardwareIdsV ) };
  assert( rawThwIdsList.size() % 2 == 0 );

  Helper::RawData_setInt< uint32_t >(
    RawDataSpan{ rawFile }.subspan( ThwIdsPointerFieldOffsetV2 ),
    static_cast< uint32_t >( nextFreeOffset / 2 ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawThwIdsList.size() );

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end() );


  // THW ID + Positions (only in V3 mode)
  if ( encodeV3Data )
  {
    uint16_t thwIdPosCount{ 0 };
    RawData rawThwPos( sizeof( uint16_t ) );

    for ( const auto &[ thwId, positions ] : targetHardwareIdsPositionsV )
    {
      // skip if no positions are stored
      if ( positions.empty() )
      {
        continue;
      }

      // THW ID
      const auto rawThwId{ StringUtils_encodeString( thwId ) };
      assert( rawThwId.size() % 2 == 0 );

      rawThwPos.insert( rawThwPos.end(), rawThwId.begin(), rawThwId.end() );

      // Positions
      const auto rawPositions{ StringUtils_encodeStrings( positions ) };
      assert( rawThwId.size() % 2 == 0 );

      rawThwPos.insert( rawThwPos.end(), rawPositions.begin(), rawPositions.end() );

      ++thwIdPosCount;
    }

    Helper::RawData_setInt< uint16_t>( rawThwPos, thwIdPosCount );

    uint32_t thwIdPosPtr{ 0 };

    if ( 0 != thwIdPosCount )
    {
      thwIdPosPtr = static_cast< uint32_t >( nextFreeOffset / 2 );
      rawFile.insert( rawFile.end(), rawThwPos.begin(), rawThwPos.end() );
      nextFreeOffset += static_cast< ptrdiff_t>( rawThwPos.size() );
    }

    Helper::RawData_setInt< uint32_t >(
      RawDataSpan{ rawFile }.subspan( ThwIdPositionsPointerFieldOffsetV3 ),
      thwIdPosPtr );
  }


  // data files list pointer
  auto rawDataFiles{ encodeDataFiles( encodeV3Data ) };
  assert( rawDataFiles.size() % 2 == 0 );

  Helper::RawData_setInt< uint32_t>(
    RawDataSpan{ rawFile }.subspan( DataFilesPointerFieldOffsetV2 ),
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

    rawFile.insert( rawFile.end(), rawSupportFiles.begin(), rawSupportFiles.end() );
  }

  Helper::RawData_setInt< uint32_t >(
    RawDataSpan{ rawFile }.subspan( SupportFilesPointerFieldOffsetV2 ),
    supportFileListPtr );

  // user defined data pointer
  assert( userDefinedDataV.size() % 2 == 0 );
  uint32_t userDefinedDataPtr{ 0 };

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = static_cast< uint32_t >( nextFreeOffset / 2 );
    nextFreeOffset += static_cast< ptrdiff_t>( userDefinedDataV.size() );

    rawFile.insert( rawFile.end(), userDefinedDataV.begin(), userDefinedDataV.end() );
  }

  Helper::RawData_setInt< uint32_t >(
    RawDataSpan{ rawFile }.subspan( UserDefinedDataPointerFieldOffsetV2 ),
    userDefinedDataPtr );

  // amount of data of Check Values and CRCs
  // by default File CRC (16bit) + load CRC (32 bit)
  uint32_t checkValueCrcSizes{ sizeof( uint16_t ) + sizeof( uint32_t ) };

  // Load Check Value (only in V3 mode)
  if ( encodeV3Data )
  {
    // Alternative implementation set Load Check Pointer to zero, when Load
    // Check Value is not given

    checkValueCrcSizes += static_cast< uint32_t >( CheckValueUtils_size( loadCheckValueTypeV ) );

    // Set Pointer to Load Check Value Field
    Helper::RawData_setInt< uint32_t >(
      RawDataSpan{ rawFile }.subspan( LoadCheckValuePointerFieldOffsetV3 ),
      static_cast< uint32_t >( nextFreeOffset / 2 ) );

    // actual check value must be encoded by external means
  }

  // set header
  insertHeader( rawFile, checkValueCrcSizes );

  // Resize to final size ( Check Value + File CRC + Load CRC)
  rawFile.resize( rawFile.size() + checkValueCrcSizes );

  // set CRC
  calculateFileCrc( rawFile );

  // load CRC must be encoded by external means

  return rawFile;
}

void LoadHeaderFile::decodeBody( ConstRawDataSpan rawFile )
{
  BOOST_LOG_FUNCTION()

  bool decodeV3Data{ false };

  uint16_t partFlags;
  std::tie( std::ignore, partFlags ) = Helper::RawData_getInt< uint16_t >( rawFile.subspan( PartFlagsFieldOffsetV3 ) );

  switch ( arincVersion() )
  {
    case SupportedArinc665Version::Supplement2:
      // Spare
      if ( partFlags != 0U )
      {
        partFlagsV = 0;
        BOOST_THROW_EXCEPTION( Arinc665Exception{} << Helper::AdditionalInfo{ "Spare not 0" } );
      }
      break;

    case SupportedArinc665Version::Supplement345:
      partFlagsV = partFlags;
      decodeV3Data = true;
      break;

    default:
      BOOST_THROW_EXCEPTION( Arinc665Exception{} << Helper::AdditionalInfo{ "Unsupported ARINC 665 Version" } );
  }

  uint32_t loadPartNumberPtr{};
  std::tie( std::ignore, loadPartNumberPtr ) =
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadPartNumberPointerFieldOffsetV2 ) );

  uint32_t targetHardwareIdListPtr{};
  std::tie( std::ignore, targetHardwareIdListPtr ) =
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( ThwIdsPointerFieldOffsetV2 ) );

  uint32_t dataFileListPtr{};
  std::tie( std::ignore, dataFileListPtr ) =
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( DataFilesPointerFieldOffsetV2 ) );

  uint32_t supportFileListPtr{};
  std::tie( std::ignore, supportFileListPtr ) =
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( SupportFilesPointerFieldOffsetV2 ) );

  uint32_t userDefinedDataPtr{};
  std::tie( std::ignore, userDefinedDataPtr ) =
    Helper::RawData_getInt< uint32_t >( rawFile.subspan( UserDefinedDataPointerFieldOffsetV2 ) );

  uint32_t loadTypeDescriptionPtr{ 0U };
  uint32_t thwIdsPositionPtr{ 0U };
  uint32_t loadCheckValuePtr{ 0U };

  // only decode this pointers in V3 mode
  if ( decodeV3Data )
  {
    std::tie( std::ignore, loadTypeDescriptionPtr ) =
      Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadTypeDescriptionPointerFieldOffsetV3 ) );

    std::tie( std::ignore, thwIdsPositionPtr ) =
      Helper::RawData_getInt< uint32_t >( rawFile.subspan( ThwIdPositionsPointerFieldOffsetV3 ) );

    std::tie( std::ignore, loadCheckValuePtr ) =
      Helper::RawData_getInt< uint32_t >( rawFile.subspan( LoadCheckValuePointerFieldOffsetV3 ) );
  }


  // load part number
  std::tie( std::ignore, partNumberV ) = StringUtils_decodeString( rawFile.subspan( loadPartNumberPtr * 2ULL ) );


  // Load Type Description Field (ARINC 665-3)
  typeV.reset();
  if ( decodeV3Data && ( 0!=loadTypeDescriptionPtr ) )
  {
    auto [ remaining, loadTypeDescription ]{
      StringUtils_decodeString( rawFile.subspan( loadTypeDescriptionPtr * 2ULL ) ) };

    auto [ _, loadTypeValue ]{ Helper::RawData_getInt< uint16_t>( remaining ) };

    loadType( LoadType{ { std::string{ loadTypeDescription }, loadTypeValue } } );
  }

  // Target Hardware ID list
  TargetHardwareIds targetHardwareIdsValue;
  std::tie( std::ignore, targetHardwareIdsValue ) =
    StringUtils_decodeStrings( rawFile.subspan( targetHardwareIdListPtr * 2ULL ) );
  targetHardwareIds( std::move( targetHardwareIdsValue ) );

  // THW IDs with Positions Field (ARINC 665-3)
  targetHardwareIdsPositionsV.clear();
  if ( decodeV3Data && ( 0U != thwIdsPositionPtr ) )
  {
    auto [ remaining, numberOfThwIdsWithPos ]{
      Helper::RawData_getInt< uint16_t >( rawFile.subspan( thwIdsPositionPtr * 2ULL ) ) };

    for ( uint16_t thwIdIndex{ 0 }; thwIdIndex < numberOfThwIdsWithPos; ++thwIdIndex )
    {
      std::string thwId;
      std::tie( remaining, thwId ) = StringUtils_decodeString( remaining );

      Positions positions;
      std::tie( remaining, positions ) = StringUtils_decodeStrings( remaining );

      targetHardwareIdPositions( std::move( thwId ), std::move( positions ) );
    }
  }

  // data file list
  decodeDataFiles( rawFile.subspan( dataFileListPtr * 2ULL ), decodeV3Data );

  // support file list
  if ( 0U != supportFileListPtr )
  {
    decodeSupportFiles( rawFile.subspan( supportFileListPtr * 2ULL ), decodeV3Data );
  }

  // user defined data
  userDefinedDataV.clear();
  if ( 0U != userDefinedDataPtr )
  {
    ptrdiff_t endOfUserDefinedData{ static_cast< ptrdiff_t>( rawFile.size() ) - FileCrcOffset };

    if ( loadCheckValuePtr != 0 )
    {
      if ( loadCheckValuePtr <= userDefinedDataPtr )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "Invalid Pointers" } );
      }

      endOfUserDefinedData = loadCheckValuePtr * 2LL;
    }

    userDefinedDataV.assign( rawFile.begin() + userDefinedDataPtr * 2LL, rawFile.begin() + endOfUserDefinedData );
  }

  // Load Check Value Field (ARINC 665-3) - Only Type is stored.
  // Check must be performed by other means
  loadCheckValueTypeV = Arinc645::CheckValueType::NotUsed;
  if ( decodeV3Data && ( 0U!=loadCheckValuePtr ) )
  {
    loadCheckValueTypeV = CheckValueUtils_decode( rawFile.subspan( loadCheckValuePtr * 2ULL ) ).type();
  }

  // file crc decoded and checked within base class

  // load crc is not decoded - this must be done by other means
}

RawData LoadHeaderFile::encodeDataFiles( const bool encodeV3Data ) const
{
  BOOST_LOG_FUNCTION()

  RawData rawFileList( sizeof( uint16_t ) );

  // Number of files must not exceed field
  if ( dataFilesV.size() > std::numeric_limits< uint16_t >::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "More files than allowed" } );
  }

  // number of files
  Helper::RawData_setInt< uint16_t >( rawFileList, Helper::safeCast< uint16_t >( dataFilesV.size() ) );

  // iterate over files
  uint16_t fileCounter{ 0 };
  for ( auto const &fileInfo : dataFilesV )
  {
    ++fileCounter;

    RawData rawFileInfo( sizeof( uint16_t ) );

    // filename
    auto const rawFilename{ StringUtils_encodeString( fileInfo.filename ) };
    assert( rawFilename.size() % 2 == 0);
    rawFileInfo.insert( rawFileInfo.end(), rawFilename.begin(), rawFilename.end() );

    // part number
    auto const rawPartNumber{ StringUtils_encodeString( fileInfo.partNumber ) };
    assert( rawPartNumber.size() % 2 == 0 );
    rawFileInfo.insert( rawFileInfo.end(), rawPartNumber.begin(), rawPartNumber.end() );

    // resize for file length, CRC
    rawFileInfo.resize( rawFileInfo.size() + sizeof( uint32_t ) + sizeof( uint16_t ) );

    // file length - rounded number of 16-bit words
    const uint32_t fileLength{ Helper::safeCast< uint32_t >( ( fileInfo.length + 1U ) / 2U ) };

    Helper::RawData_setInt< uint32_t >(
      RawDataSpan{ rawFileInfo }.last( sizeof( uint32_t ) + sizeof( uint16_t ) ),
      fileLength );

    // CRC
    Helper::RawData_setInt< uint16_t >( RawDataSpan{ rawFileInfo }.last( sizeof( uint16_t ) ), fileInfo.crc );

    // following fields are available in ARINC 665-3 ff
    if ( encodeV3Data )
    {
      // length in bytes (Data File List)
      rawFileInfo.resize( rawFileInfo.size() + sizeof( uint64_t ) );
      Helper::RawData_setInt< uint64_t >( RawDataSpan{ rawFileInfo }.last( sizeof( uint64_t ) ), fileInfo.length );

      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue ) };
      assert( rawCheckValue.size() % 2 == 0 );
      rawFileInfo.insert( rawFileInfo.end(), rawCheckValue.begin(), rawCheckValue.end() );
    }

    // next load pointer (is set to 0 for last load)
    Helper::RawData_setInt< uint16_t >(
      rawFileInfo,
      ( fileCounter == dataFilesV.size() ) ? 0U : Helper::safeCast< uint16_t >( rawFileInfo.size() / 2U ) );

    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end() );
  }

  return rawFileList;
}

RawData LoadHeaderFile::encodeSupportFiles( const bool encodeV3Data ) const
{
  BOOST_LOG_FUNCTION()

  RawData rawFileList( sizeof( uint16_t ) );

  // Number of files must not exceed field
  if ( supportFilesV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "More files than allowed" } );
  }

  // number of loads
  Helper::RawData_setInt< uint16_t >( rawFileList, Helper::safeCast< uint16_t >( supportFilesV.size() ) );

  // iterate over files
  uint16_t fileCounter{ 0 };
  for ( auto const &fileInfo : supportFilesV )
  {
    ++fileCounter;

    RawData rawFileInfo( sizeof( uint16_t ) );

    // filename
    auto const rawFilename{ StringUtils_encodeString( fileInfo.filename ) };
    assert( rawFilename.size() % 2 == 0 );
    rawFileInfo.insert( rawFileInfo.end(), rawFilename.begin(), rawFilename.end() );

    // part number
    auto const rawPartNumber{ StringUtils_encodeString( fileInfo.partNumber ) };
    assert( rawPartNumber.size() % 2 == 0);
    rawFileInfo.insert( rawFileInfo.end(), rawPartNumber.begin(), rawPartNumber.end() );

    // resize for file length and CRC
    rawFileInfo.resize( rawFileInfo.size() + sizeof( uint32_t ) + sizeof( uint16_t ) );

    // file length in number of bytes
    uint32_t fileLength{ Helper::safeCast< uint32_t>( fileInfo.length ) };

    Helper::RawData_setInt< uint32_t >(
      RawDataSpan{ rawFileInfo }.last( sizeof( uint32_t ) + sizeof( uint16_t ) ),
      fileLength );

    // CRC
    Helper::RawData_setInt< uint16_t >( RawDataSpan{ rawFileInfo }.last( sizeof( uint16_t ) ), fileInfo.crc );

    // following fields are available in ARINC 665-3 ff
    if ( encodeV3Data )
    {
      // check Value
      const auto rawCheckValue{ CheckValueUtils_encode( fileInfo.checkValue ) };
      assert( rawCheckValue.size() % 2 == 0 );
      rawFileInfo.insert( rawFileInfo.end(), rawCheckValue.begin(), rawCheckValue.end() );
    }

    // next load pointer (is set to 0 for last load)
    Helper::RawData_setInt< uint16_t >(
      rawFileInfo,
      ( fileCounter == supportFilesV.size() ) ? 0U : Helper::safeCast< uint16_t >( rawFileInfo.size() / 2U ) );

    // add file info to files info
    rawFileList.insert( rawFileList.end(), rawFileInfo.begin(), rawFileInfo.end() );
  }

  return rawFileList;
}

void LoadHeaderFile::decodeDataFiles( ConstRawDataSpan rawData, const bool decodeV3Data )
{
  BOOST_LOG_FUNCTION()

  auto remaining{ rawData };

  // clear data files
  dataFilesV.clear();

  // number of data files
  uint16_t numberOfFiles;
  std::tie( remaining, numberOfFiles ) = Helper::RawData_getInt< uint16_t>( remaining );

  // iterate over file index
  for ( uint16_t fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex )
  {
    auto listRemaining{ remaining };

    // next file pointer
    uint16_t filePointer;
    std::tie( listRemaining, filePointer ) = Helper::RawData_getInt< uint16_t>( listRemaining );

    // check file pointer for validity
    if ( fileIndex != numberOfFiles - 1U )
    {
      if ( filePointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "next file pointer is 0" } );
      }
    }
    else
    {
      if ( filePointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "next file pointer is not 0" } );
      }
    }

    // filename
    std::string name;
    std::tie( listRemaining, name ) = StringUtils_decodeString( listRemaining );

    // part number
    std::string partNumber;
    std::tie( listRemaining, partNumber ) = StringUtils_decodeString( listRemaining );

    // file length
    uint32_t length;
    std::tie( listRemaining, length ) = Helper::RawData_getInt< uint32_t>( listRemaining );

    // rounded number of 16-bit words
    uint64_t realLength{ length * 2ULL };

    // CRC
    uint16_t crc;
    std::tie( listRemaining, crc ) = Helper::RawData_getInt< uint16_t >( listRemaining );

    // CheckValue - if not decoded "no check value"
    Arinc645::CheckValue checkValue{ Arinc645::CheckValue::NoCheckValue };

    // following fields are available in ARINC 665-3 ff
    if ( decodeV3Data )
    {
      // length in bytes (Data File List)
      uint64_t fileLengthInBytes{};
      std::tie( listRemaining, fileLengthInBytes ) = Helper::RawData_getInt< uint64_t>( listRemaining );

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
      checkValue = CheckValueUtils_decode( listRemaining );
    }

    // set it to begin of next file
    remaining = remaining.subspan( filePointer * 2ULL );

    // file info
    dataFilesV.emplace_back( LoadFileInfo{
      .filename = std::move( name ),
      .partNumber = std::move( partNumber ),
      .length = realLength,
      .crc = crc,
      .checkValue = std::move( checkValue ) } );
  }
}

void LoadHeaderFile::decodeSupportFiles( ConstRawDataSpan rawData, bool decodeV3Data )
{
  BOOST_LOG_FUNCTION()

  auto remaining{ rawData };

  // clear support files
  supportFilesV.clear();

  // number of data files
  uint16_t numberOfFiles{};
  std::tie( remaining, numberOfFiles ) = Helper::RawData_getInt< uint16_t>( remaining );

  // iterate over file index
  for ( uint16_t fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex )
  {
    auto listRemaining{ remaining };

    // next file pointer
    uint16_t filePointer{};
    std::tie( listRemaining, filePointer ) = Helper::RawData_getInt< uint16_t >( listRemaining );

    // check file pointer for validity
    if ( fileIndex != numberOfFiles - 1U )
    {
      if ( filePointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "next file pointer is 0" } );
      }
    }
    else
    {
      if ( filePointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File{} << Helper::AdditionalInfo{ "next file pointer is not 0" } );
      }
    }

    // filename
    std::string name{};
    std::tie( listRemaining, name ) = StringUtils_decodeString( listRemaining );

    // part number
    std::string partNumber{};
    std::tie( listRemaining, partNumber ) = StringUtils_decodeString( listRemaining );

    // file length
    uint32_t length{};
    std::tie( listRemaining, length ) = Helper::RawData_getInt< uint32_t >( listRemaining );

    // CRC
    uint16_t crc{};
    std::tie( listRemaining, crc ) = Helper::RawData_getInt< uint16_t >( listRemaining );

    // CheckValue - if not decoded "no check value"
    Arinc645::CheckValue checkValue{ Arinc645::CheckValue::NoCheckValue };

    // following fields are available in ARINC 665-3 ff
    if ( decodeV3Data )
    {
      // check Value
      checkValue = CheckValueUtils_decode( listRemaining );
    }

    // set it to begin of next file
    remaining = remaining.subspan( filePointer * 2ULL );

    // file info
    supportFilesV.emplace_back(
      LoadFileInfo{ std::move( name ), std::move( partNumber ), length, crc, std::move( checkValue ) } );
  }
}

void LoadHeaderFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U )
  {
    BOOST_LOG_SEV( Logger::get(), Helper::Severity::warning )
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back( std::byte{ 0U } );
  }
}

}

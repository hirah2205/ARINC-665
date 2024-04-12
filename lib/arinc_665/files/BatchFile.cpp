// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Files::BatchFile.
 **/

#include "BatchFile.hpp"

#include <arinc_665/files/StringUtils.hpp>

#include <arinc_665/Arinc665Exception.hpp>

#include <helper/Endianness.hpp>
#include <helper/Exception.hpp>
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Files {

BatchFile::BatchFile( const SupportedArinc665Version version ) noexcept :
  Arinc665File{ version }
{
}

BatchFile::BatchFile( ConstRawFileSpan rawFile ) :
  Arinc665File{ rawFile, FileType::BatchFile }
{
  decodeBody( rawFile );
}

BatchFile& BatchFile::operator=( ConstRawFileSpan rawFile )
{
  // call inherited operator
  Arinc665File::operator =( rawFile );
  decodeBody( rawFile );

  return *this;
}

FileType BatchFile::fileType() const noexcept
{
  return FileType::BatchFile;
}

std::string_view BatchFile::partNumber() const
{
  return partNumberV;
}

void BatchFile::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
}

std::string_view BatchFile::comment() const
{
  return commentV;
}

void BatchFile::comment( std::string comment )
{
  commentV = std::move( comment);
}

const BatchTargetsInfo& BatchFile::targetsHardware() const
{
  return targetsHardwareV;
}

BatchTargetsInfo& BatchFile::targetsHardware()
{
  return targetsHardwareV;
}

void BatchFile::targetHardware( BatchTargetInfo targetHardwareInfo )
{
  targetsHardwareV.push_back( std::move( targetHardwareInfo ) );
}

RawFile BatchFile::encode() const
{
  RawFile rawFile( BatchFileHeaderSizeV2 );

  // spare field
  Helper::setInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, 0U );


  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ BatchFileHeaderSizeV2 };


  // batch part number + comment
  auto rawBatchPn{ StringUtils_encodeString( partNumberV ) };
  assert( rawBatchPn.size() % 2 == 0 );
  auto rawComment{ StringUtils_encodeString( commentV ) };
  assert( rawComment.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + BatchPartNumberPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += rawBatchPn.size() + rawComment.size();

  rawFile.insert( rawFile.end(), rawBatchPn.begin(), rawBatchPn.end() );
  rawFile.insert( rawFile.end(), rawComment.begin(), rawComment.end() );


  // THW ID load list
  auto rawThwIdsList{ encodeBatchTargetsInfo() };
  assert( rawThwIdsList.size() % 2 == 0 );

  Helper::setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end() );

  // set header
  insertHeader( rawFile );

  // Resize file for file CRC
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );

  // set CRC
  calculateFileCrc( rawFile );

  return rawFile;
}

void BatchFile::decodeBody( ConstRawFileSpan rawFile )
{
  // Spare field
  uint16_t spare{};
  Helper::getInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, spare );

  if ( 0U != spare )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "Spare is not 0" } );
  }

  uint32_t batchPartNumberPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + BatchPartNumberPointerFieldOffsetV2,
    batchPartNumberPtr);

  uint32_t targetHardwareIdListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffsetV2,
    targetHardwareIdListPtr);

  // batch part number
  auto it{ StringUtils_decodeString(
    rawFile.begin() + static_cast< ptrdiff_t >( batchPartNumberPtr ) * 2, partNumberV ) };

  // comment
  StringUtils_decodeString( it, commentV );

  // target hardware ID load list
  decodeBatchTargetsInfo( rawFile, static_cast< ptrdiff_t >( targetHardwareIdListPtr ) * 2 );
}

RawFile BatchFile::encodeBatchTargetsInfo() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawBatchTargetsInfo( sizeof( uint16_t ) ); // Number of THW IDs

  // Number of targets must not exceed field
  if ( targetsHardwareV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More THW IDs than allowed" } );
  }

  Helper::setInt< uint16_t>(
    rawBatchTargetsInfo.begin(),
    Helper::safeCast< uint16_t>( targetsHardwareV.size() ) );

  // iterate over target HWs
  uint16_t thwCounter{ 0};
  for ( auto const &targetHardwareInfo : targetsHardwareV )
  {
    ++thwCounter;

    auto const rawThwIdPosition{
      StringUtils_encodeString( targetHardwareInfo.targetHardwareIdPosition ) };
    assert( rawThwIdPosition.size() % 2 == 0);

    RawFile rawLoadsInfo{};
    /* iterate over loads */
    for ( auto const &loadInfo : targetHardwareInfo.loads )
    {
      auto const rawHeaderFilename{ StringUtils_encodeString( loadInfo.headerFilename ) };
      assert( rawHeaderFilename.size() % 2 == 0 );

      auto const rawPartNumber{ StringUtils_encodeString( loadInfo.partNumber ) };
      assert( rawPartNumber.size() % 2 == 0 );

      rawLoadsInfo.insert(
        rawLoadsInfo.end(),
        rawHeaderFilename.begin(),
        rawHeaderFilename.end());
      rawLoadsInfo.insert(
        rawLoadsInfo.end(),
        rawPartNumber.begin(),
        rawPartNumber.end());
    }
    assert( rawLoadsInfo.size() % 2 == 0);

    RawFile rawBatchTargetInfo(
      sizeof( uint16_t ) + // next THW ID pointer
      rawThwIdPosition.size() +
      sizeof( uint16_t ) + // number of loads
      rawLoadsInfo.size() );

    auto batchTargetInfoIt{ rawBatchTargetInfo.begin() };

    // next load pointer (is set to 0 for last load)
    batchTargetInfoIt = Helper::setInt< uint16_t>(
      batchTargetInfoIt,
      ( thwCounter == targetsHardwareV.size() ) ?
        ( 0U ) :
        Helper::safeCast< uint16_t>( rawBatchTargetInfo.size() / 2 ) );

    // THW ID
    batchTargetInfoIt = std::copy(
      rawThwIdPosition.begin(),
      rawThwIdPosition.end(),
      batchTargetInfoIt );

    // Number of Loads
    batchTargetInfoIt = Helper::setInt< uint16_t>(
      batchTargetInfoIt,
      Helper::safeCast< uint16_t>( targetHardwareInfo.loads.size() ) );

    // Loads list
    std::copy( rawLoadsInfo.begin(), rawLoadsInfo.end(), batchTargetInfoIt );

    // add THW info to files info
    rawBatchTargetsInfo.insert(
      rawBatchTargetsInfo.end(),
      rawBatchTargetInfo.begin(),
      rawBatchTargetInfo.end() );
  }

  return rawBatchTargetsInfo;
}

void BatchFile::decodeBatchTargetsInfo(
  ConstRawFileSpan rawFile,
  const ptrdiff_t offset )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + offset };

  // clear potentially data
  targetsHardwareV.clear();

  // number of target HW IDs
  uint16_t numberOfTargetHardwareIds{};
  it = Helper::getInt< uint16_t>( it, numberOfTargetHardwareIds );

  // iterate over THW ID index
  for (
    unsigned int thwIdIndex = 0U;
    thwIdIndex < numberOfTargetHardwareIds;
    ++thwIdIndex )
  {
    auto listIt{ it };

    // next THW ID pointer
    uint16_t thwIdPointer{};
    listIt = Helper::getInt< uint16_t>( listIt, thwIdPointer );

    // check file pointer for validity
    if ( thwIdIndex != numberOfTargetHardwareIds - 1U )
    {
      if ( thwIdPointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next THW ID pointer is 0" } );
      }
    }
    else
    {
      if ( thwIdPointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next THW ID pointer is not 0" } );
      }
    }

    // THW ID
    std::string thwId{};
    listIt = StringUtils_decodeString( listIt, thwId);

    // Loads list
    BatchLoadsInfo batchLoadsInfo{};

    // number of loads
    uint16_t numberOfLoads{};
    listIt = Helper::getInt< uint16_t>( listIt, numberOfLoads );

    // iterate over load index
    for ( uint16_t loadIndex = 0U; loadIndex < numberOfLoads; ++loadIndex )
    {
      // header filename
      std::string filename{};
      listIt = StringUtils_decodeString( listIt, filename );

      // Load PN
      std::string partNumber{};
      listIt = StringUtils_decodeString( listIt, partNumber );

      // Batch Load info
      batchLoadsInfo.emplace_back( BatchLoadInfo{
        .headerFilename = std::move( filename ),
        .partNumber = std::move( partNumber ) } );
    }

    // set it to begin of next file
    it += static_cast< ptrdiff_t >( thwIdPointer ) * 2;

    // THW ID info
    targetsHardwareV.emplace_back( BatchTargetInfo{
      .targetHardwareIdPosition = std::move( thwId ),
      .loads = std::move( batchLoadsInfo ) } );
  }
}

}

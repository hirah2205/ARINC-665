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
 * @brief Definition of Class Arinc665::Files::LoadListFile.
 **/

#include "LoadListFile.hpp"

#include <arinc665/files/StringUtils.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianness.hpp>
#include <helper/Exception.hpp>
#include <helper/SafeCast.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Files {

LoadListFile::LoadListFile( const SupportedArinc665Version version ) noexcept :
  ListFile{ version }
{
}

LoadListFile::LoadListFile( ConstRawFileSpan rawFile ) :
  ListFile{ rawFile, FileType::LoadList }
{
  decodeBody( rawFile );
}

LoadListFile& LoadListFile::operator=( ConstRawFileSpan rawFile )
{
  // call inherited operator
  Arinc665File::operator =( rawFile );
  decodeBody( rawFile );

  return *this;
}

FileType LoadListFile::fileType() const noexcept
{
  return FileType::LoadList;
}

size_t LoadListFile::numberOfLoads() const
{
  return loadsV.size();
}

const LoadsInfo& LoadListFile::loads() const
{
  return loadsV;
}

LoadsInfo& LoadListFile::loads()
{
  return loadsV;
}

void LoadListFile::loads( LoadsInfo loadsInformation )
{
  loadsV = std::move( loadsInformation );
}

void LoadListFile::load( LoadInfo load )
{
  loadsV.push_back( std::move( load ) );
}

ConstUserDefinedDataSpan LoadListFile::userDefinedData() const
{
  return userDefinedDataV;
}

void LoadListFile::userDefinedData( UserDefinedData userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData);

  checkUserDefinedData();
}

bool LoadListFile::belongsToSameMediaSet( const LoadListFile &other) const
{
  BOOST_LOG_FUNCTION()

  return ( mediaSetPn() == other.mediaSetPn() )
    && ( numberOfMediaSetMembers() == other.numberOfMediaSetMembers() )
    && std::equal(
      userDefinedDataV.begin(),
      userDefinedDataV.end(),
      other.userDefinedData().begin() )
    && ( loadsV == other.loads() );
}

RawFile LoadListFile::encode() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFile( FileHeaderSizeV2 );

  // Spare Field
  Helper::setInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, 0U );

  // Next free Offset (used for optional pointer calculation)
  ptrdiff_t nextFreeOffset{ static_cast< ptrdiff_t >( rawFile.size() ) };


  // media set information
  const auto rawMediaInformation{ encodeMediaInformation() };
  assert( rawMediaInformation.size() % 2 == 0);
  rawFile.insert( rawFile.end(), rawMediaInformation.begin(), rawMediaInformation.end() );

  // Update Pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawMediaInformation.size() );

  // Loads info
  const auto rawLoadsInfo{ encodeLoadsInfo() };
  assert( rawLoadsInfo.size() % 2 == 0);

  // loads list pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + LoadFilesPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawLoadsInfo.size() );

  rawFile.insert( rawFile.end(), rawLoadsInfo.begin(), rawLoadsInfo.end() );


  // user defined data
  assert( userDefinedDataV.size() % 2 == 0 );
  uint32_t userDefinedDataPtr = 0;

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = static_cast< uint32_t >( nextFreeOffset / 2U );

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end());
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr);


  // set header
  insertHeader( rawFile );

  // Resize file for file CRC
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );

  // set CRC
  calculateFileCrc( rawFile );

  return rawFile;
}

void LoadListFile::decodeBody( ConstRawFileSpan rawFile )
{
  BOOST_LOG_FUNCTION()

  // Spare Field
  uint16_t spare{};
  Helper::getInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, spare);

  if ( 0U != spare )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "Spare is not 0" } );
  }

  // media information pointer
  uint32_t mediaInformationPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    mediaInformationPtr );

  // Loads list pointer
  uint32_t loadListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + LoadFilesPointerFieldOffsetV2,
    loadListPtr );


  // user defined data pointer
  uint32_t userDefinedDataPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr );

  // decode Media Information
  decodeMediaInformation( rawFile, mediaInformationPtr );

  // Loads list
  decodeLoadsInfo( rawFile, static_cast< ptrdiff_t >( loadListPtr ) * 2 );


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    userDefinedDataV.assign(
      rawFile.begin() + static_cast< ptrdiff_t >( userDefinedDataPtr ) * 2,
      rawFile.begin() + static_cast< ptrdiff_t >( rawFile.size() ) - DefaultChecksumPosition );
  }

  // file crc decoded and checked within base class
}

RawFile LoadListFile::encodeLoadsInfo() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawLoadsInfo( sizeof( uint16_t ) );

  // Number of loads must not exceed field
  if ( loadsV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More loads than allowed" } );
  }

  // number of loads
  Helper::setInt< uint16_t>(
    rawLoadsInfo.begin(),
    static_cast< uint16_t >( loadsV.size() ) );

  // iterate over files
  uint16_t loadCounter( 0);
  for ( auto const &loadInfo : loadsV )
  {
    ++loadCounter;
    auto const rawPartNumber( StringUtils_encodeString( loadInfo.partNumber ) );
    assert( rawPartNumber.size() % 2 == 0);

    auto const rawHeaderFilename( StringUtils_encodeString( loadInfo.headerFilename ) );
    assert( rawHeaderFilename.size() % 2 == 0);

    auto const rawThwIds( StringUtils_encodeStrings( loadInfo.targetHardwareIds ) );
    assert( rawThwIds.size() % 2 == 0);

    RawFile rawLoadInfo(
      sizeof( uint16_t) + // next load pointer
      rawPartNumber.size() +
      rawHeaderFilename.size() +
      sizeof( uint16_t) + // member sequence number
      rawThwIds.size());

    auto loadInfoIt( rawLoadInfo.begin());

    // next load pointer (is set to 0 for last load)
    loadInfoIt = Helper::setInt< uint16_t>(
      loadInfoIt,
      (loadCounter == numberOfLoads()) ?
        0U :
        static_cast< uint16_t >( rawLoadInfo.size() / 2U ) );

    // part number
    loadInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), loadInfoIt );

    // header filename
    loadInfoIt = std::copy( rawHeaderFilename.begin(), rawHeaderFilename.end(), loadInfoIt );

    // member sequence number
    loadInfoIt = Helper::setInt< uint16_t>(
      loadInfoIt,
      static_cast< uint8_t >( loadInfo.memberSequenceNumber ) );

    // THW IDs list
    std::copy( rawThwIds.begin(), rawThwIds.end(), loadInfoIt );

    // add file info to files info
    rawLoadsInfo.insert( rawLoadsInfo.end(), rawLoadInfo.begin(), rawLoadInfo.end());
  }

  return rawLoadsInfo;
}

void LoadListFile::decodeLoadsInfo(
  ConstRawFileSpan rawFile,
  const ptrdiff_t offset )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + offset };

  // number of loads
  uint16_t numberOfLoads{};
  it = Helper::getInt< uint16_t>( it, numberOfLoads );

  // iterate of load counter
  for ( uint16_t loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex )
  {
    auto listIt{ it };

    // next load pointer
    uint16_t loadPointer{};
    listIt = Helper::getInt< uint16_t>( listIt, loadPointer);

    // check load pointer for validity
    if ( loadIndex != numberOfLoads - 1U )
    {
      if ( loadPointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next load pointer is 0" } );
      }
    }
    else
    {
      if ( loadPointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next load pointer is not 0" } );
      }
    }

    // part number
    std::string partNumber{};
    listIt = StringUtils_decodeString( listIt, partNumber );

    // header filename
    std::string headerFilename{};
    listIt = StringUtils_decodeString( listIt, headerFilename );

    // member sequence number
    uint16_t fileMemberSequenceNumber{};
    listIt = Helper::getInt< uint16_t>( listIt, fileMemberSequenceNumber );
    if ( ( fileMemberSequenceNumber < 1U ) || ( fileMemberSequenceNumber > 255U ) )
    {
      BOOST_THROW_EXCEPTION( InvalidArinc665File()
        << Helper::AdditionalInfo{ "member sequence number out of range" } );
    }

    LoadInfo::ThwIds thwIds{};
    StringUtils_decodeStrings( listIt, thwIds );

    loadsV.emplace_back( LoadInfo{
      .partNumber = std::move( partNumber ),
      .headerFilename = std::move( headerFilename ),
      .memberSequenceNumber =
        MediumNumber{ Helper::safeCast< uint8_t>( fileMemberSequenceNumber ) },
      .targetHardwareIds = std::move( thwIds ) } );

    // set it to begin of next load
    it += static_cast< ptrdiff_t >( loadPointer ) * 2;
  }
}

void LoadListFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning)
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back( 0U );
  }
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::LoadListFile.
 **/

#include "LoadListFile.hpp"

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665::File {

LoadListFile::LoadListFile( SupportedArinc665Version version):
  ListFile{ version },
  mediaSequenceNumberV{ 0 },
  numberOfMediaSetMembersV{ 0 }
{
}

LoadListFile::LoadListFile(
  SupportedArinc665Version version,
  std::string_view mediaSetPn,
  uint8_t mediaSequenceNumber,
  uint8_t numberOfMediaSetMembers,
  const LoadsInfo &loads,
  const UserDefinedData &userDefinedData):
  ListFile{ version },
  mediaSetPnV{ mediaSetPn },
  mediaSequenceNumberV{ mediaSequenceNumber },
  numberOfMediaSetMembersV{ numberOfMediaSetMembers },
  loadsV{ loads },
  userDefinedDataV{ userDefinedData }
{
  checkUserDefinedData();
}

LoadListFile::LoadListFile(
  SupportedArinc665Version version,
  std::string &&mediaSetPn,
  uint8_t mediaSequenceNumber,
  uint8_t numberOfMediaSetMembers,
  LoadsInfo &&loads,
  UserDefinedData &&userDefinedData):
  ListFile{ version},
  mediaSetPnV{ std::move( mediaSetPn ) },
  mediaSequenceNumberV{ mediaSequenceNumber },
  numberOfMediaSetMembersV{ numberOfMediaSetMembers },
  loadsV{ std::move( loads ) },
  userDefinedDataV{ std::move( userDefinedData ) }
{
  checkUserDefinedData();
}

LoadListFile::LoadListFile( const ConstRawFileSpan &rawFile):
  ListFile{ rawFile, FileType::LoadList }
{
  decodeBody( rawFile );
}

LoadListFile& LoadListFile::operator=( const ConstRawFileSpan &rawFile )
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

std::string_view LoadListFile::mediaSetPn() const
{
  return mediaSetPnV;
}

void LoadListFile::mediaSetPn( std::string_view mediaSetPn )
{
  mediaSetPnV = mediaSetPn;
}

void LoadListFile::mediaSetPn( std::string &&mediaSetPn )
{
  mediaSetPnV = std::move( mediaSetPn);
}

uint8_t LoadListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberV;
}

void LoadListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber )
{
  mediaSequenceNumberV = mediaSequenceNumber;
}

uint8_t LoadListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersV;
}

void LoadListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  numberOfMediaSetMembersV = numberOfMediaSetMembers;
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

void LoadListFile::load( const LoadInfo &load)
{
  loadsV.push_back( load);
}

void LoadListFile::load( LoadInfo &&load)
{
  loadsV.push_back( std::move( load));
}

const LoadListFile::UserDefinedData& LoadListFile::userDefinedData() const
{
  return userDefinedDataV;
}

void LoadListFile::userDefinedData( const UserDefinedData &userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = userDefinedData;

  checkUserDefinedData();
}

void LoadListFile::userDefinedData( UserDefinedData &&userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData);

  checkUserDefinedData();
}

bool LoadListFile::belongsToSameMediaSet( const LoadListFile &other) const
{
  return
    ( mediaSetPnV == other.mediaSetPn()) &&
    ( numberOfMediaSetMembersV == other.numberOfMediaSetMembers()) &&
    ( loadsV == other.loads()) &&
    ( userDefinedDataV == other.userDefinedData());
}

RawFile LoadListFile::encode() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFile( FileHeaderSizeV2 );

  // Spare Field
  Helper::setInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, 0U);

  // Next free Offset (used for optional pointer calculation)
  ptrdiff_t nextFreeOffset{ static_cast< ptrdiff_t >( rawFile.size() ) };


  // media set information
  const auto rawMediaSetPn{ encodeString( mediaSetPn())};
  assert( rawMediaSetPn.size() % 2 == 0);

  // media set part number
  rawFile.insert( rawFile.end(), rawMediaSetPn.begin(), rawMediaSetPn.end());

  rawFile.resize( rawFile.size() + 2 * sizeof( uint8_t));

  // media sequence number
  Helper::setInt< uint8_t>(
    rawFile.begin() + nextFreeOffset + static_cast< ptrdiff_t >( rawMediaSetPn.size() ),
    mediaSequenceNumberV );

  // number of media set members
  Helper::setInt< uint8_t>(
    rawFile.begin() + nextFreeOffset + static_cast< ptrdiff_t >( rawMediaSetPn.size() ) + sizeof( uint8_t ),
    numberOfMediaSetMembersV );

  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    nextFreeOffset / 2);
  nextFreeOffset += static_cast< ptrdiff_t >( rawMediaSetPn.size() + 2 * sizeof( uint8_t ) );


  // Loads info
  const auto rawLoadsInfo{ encodeLoadsInfo() };
  assert( rawLoadsInfo.size() % 2 == 0);

  // loads list pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + LoadFilesPointerFieldOffsetV2,
    nextFreeOffset / 2);
  nextFreeOffset += static_cast< ptrdiff_t >( rawLoadsInfo.size() );

  rawFile.insert( rawFile.end(), rawLoadsInfo.begin(), rawLoadsInfo.end());


  // user defined data
  assert( userDefinedDataV.size() % 2 == 0);
  uint32_t userDefinedDataPtr = 0;

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = nextFreeOffset / 2;
    // nextFreeOffset += userDefinedDataValue.size();

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end());
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr);


  // Resize to final size ( File CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );


  // set header and CRC
  insertHeader( rawFile);

  return rawFile;
}

void LoadListFile::decodeBody( const ConstRawFileSpan &rawFile )
{
  BOOST_LOG_FUNCTION()

  // Spare Field
  uint16_t spare{};
  Helper::getInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, spare);

  if ( 0U != spare )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo( "Spare is not 0"));
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


  // media set part number
  auto it = decodeString(
    rawFile.begin() + mediaInformationPtr * 2, mediaSetPnV );

  // media sequence number
  it = Helper::getInt< uint8_t>( it, mediaSequenceNumberV );

  // number of media set members
  Helper::getInt< uint8_t>( it, numberOfMediaSetMembersV );


  // Loads list
  decodeLoadsInfo( rawFile, 2 * loadListPtr);


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    userDefinedDataV.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + static_cast< ptrdiff_t >( rawFile.size() ) - DefaultChecksumPosition );
  }

  // file crc decoded and checked within base class
}

RawFile LoadListFile::encodeLoadsInfo() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawLoadsInfo( sizeof( uint16_t));

  // number of loads
  Helper::setInt< uint16_t>( rawLoadsInfo.begin(), numberOfLoads());

  // iterate over files
  uint16_t loadCounter( 0);
  for ( auto const &loadInfo : loadsV )
  {
    ++loadCounter;
    auto const rawPartNumber( encodeString( loadInfo.partNumber()));
    assert( rawPartNumber.size() % 2 == 0);

    auto const rawHeaderFilename( encodeString( loadInfo.headerFilename()));
    assert( rawHeaderFilename.size() % 2 == 0);

    auto const rawThwIds( encodeStringList( loadInfo.targetHardwareIds()));
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
        (0U) :
        (rawLoadInfo.size() / 2));

    // part number
    loadInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), loadInfoIt);

    // header filename
    loadInfoIt = std::copy( rawHeaderFilename.begin(), rawHeaderFilename.end(), loadInfoIt);

    // member sequence number
    loadInfoIt = Helper::setInt< uint16_t>( loadInfoIt, loadInfo.memberSequenceNumber());

    // THW IDs list
    loadInfoIt = std::copy( rawThwIds.begin(), rawThwIds.end(), loadInfoIt);

    // add file info to files info
    rawLoadsInfo.insert( rawLoadsInfo.end(), rawLoadInfo.begin(), rawLoadInfo.end());
  }

  return rawLoadsInfo;
}

void LoadListFile::decodeLoadsInfo(
  const ConstRawFileSpan &rawFile,
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

    //! @todo check pointer for != 0 (all except last ==> OK, last ==> error)

    // part number
    std::string partNumber{};
    listIt = Arinc665File::decodeString( listIt, partNumber );

    // header filename
    std::string headerFilename{};
    listIt = Arinc665File::decodeString( listIt, headerFilename );

    // member sequence number
    uint16_t fileMemberSequenceNumber{};
    listIt = Helper::getInt< uint16_t>( listIt, fileMemberSequenceNumber );
    if ( ( fileMemberSequenceNumber < 1U ) || ( fileMemberSequenceNumber > 255U ) )
    {
      BOOST_THROW_EXCEPTION( InvalidArinc665File()
        << Helper::AdditionalInfo( "member sequence number out of range" ) );
    }

    LoadInfo::ThwIds thwIds{};
    listIt = Arinc665File::decodeStringList( listIt, thwIds);

    loadsV.emplace_back(
      std::move( partNumber),
      std::move( headerFilename),
      static_cast< uint8_t>( fileMemberSequenceNumber),
      std::move( thwIds));

    // set it to begin of next load
    it += loadPointer * 2;
  }
}

void LoadListFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning)
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back( 0U );
  }
}

}

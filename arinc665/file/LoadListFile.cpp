/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::File::LoadListFile.
 **/

#include "LoadListFile.hpp"

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665::File {

LoadListFile::LoadListFile( SupportedArinc665Version version):
  ListFile( FileType::LoadList, version),
  mediaSequenceNumberValue( 0),
  numberOfMediaSetMembersValue( 0)
{
}

LoadListFile::LoadListFile( const RawFile &rawFile):
  ListFile( FileType::LoadList, rawFile)
{
  decodeBody( rawFile);
}

LoadListFile& LoadListFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

std::string LoadListFile::mediaSetPn() const
{
  return mediaSetPnValue;
}

void LoadListFile::mediaSetPn( const std::string &mediaSetPn)
{
  mediaSetPnValue = mediaSetPn;
}

void LoadListFile::mediaSetPn( std::string &&mediaSetPn)
{
  mediaSetPnValue = std::move( mediaSetPn);
}

uint8_t LoadListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberValue;
}

void LoadListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  mediaSequenceNumberValue = mediaSequenceNumber;
}

uint8_t LoadListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersValue;
}

void LoadListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  numberOfMediaSetMembersValue = numberOfMediaSetMembers;
}

size_t LoadListFile::numberOfLoads() const
{
  return loadsValue.size();
}

const LoadsInfo& LoadListFile::loads() const
{
  return loadsValue;
}

LoadsInfo& LoadListFile::loads()
{
  return loadsValue;
}

LoadListFile::LoadsInfoMap LoadListFile::loadsAsMap() const
{
  LoadsInfoMap loads;

  for (const auto & loadInfo : loadsValue)
  {
    loads.insert( std::make_pair(
      std::make_pair(
        loadInfo.memberSequenceNumber(),
        loadInfo.headerFilename()),
      loadInfo));
  }

  return loads;
}

void LoadListFile::load( const LoadInfo &load)
{
  loadsValue.push_back( load);
}

void LoadListFile::load( LoadInfo &&load)
{
  loadsValue.push_back( std::move( load));
}

const LoadListFile::UserDefinedData& LoadListFile::userDefinedData() const
{
  return userDefinedDataValue;
}

void LoadListFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  BOOST_LOG_FUNCTION();

  userDefinedDataValue = userDefinedData;

  if (userDefinedData.size() % 2 != 0)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
      "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataValue.push_back(0);
  }
}

bool LoadListFile::belongsToSameMediaSet( const LoadListFile &other) const
{
  return
    (mediaSetPnValue == other.mediaSetPn()) &&
    (numberOfMediaSetMembersValue == other.numberOfMediaSetMembers()) &&
    (loadsValue == other.loads()) &&
    (userDefinedDataValue == other.userDefinedData());
}

RawFile LoadListFile::encode() const
{
  RawFile rawFile( FileHeaderSize);

  // Spare Field
  setInt< uint16_t>( rawFile.begin() + SpareFieldOffset, 0U);

  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ rawFile.size()};


  // media set informations
  const auto rawMediaSetPn{ encodeString( mediaSetPn())};
  assert( rawMediaSetPn.size() % 2 == 0);

  // media set part number
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


  // Loads info
  const auto rawLoadsInfo{ encodeLoadsInfo()};
  assert( rawLoadsInfo.size() % 2 == 0);

  // loads list pointer
  setInt< uint32_t>(
    rawFile.begin() + LoadFilesPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawLoadsInfo.size();

  rawFile.insert( rawFile.end(), rawLoadsInfo.begin(), rawLoadsInfo.end());


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


  // set header and CRC
  insertHeader( rawFile);

  return rawFile;
}

void LoadListFile::decodeBody( const RawFile &rawFile)
{
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

  // Loads list pointer
  uint32_t loadListPtr;
  getInt< uint32_t>(
    rawFile.begin() + LoadFilesPointerFieldOffset,
    loadListPtr);


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


  // Loads list
  decodeLoadsInfo( rawFile, 2 * loadListPtr);


  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    userDefinedDataValue.assign(
      rawFile.begin() + userDefinedDataPtr * 2,
      rawFile.begin() + rawFile.size() - DefaultChecksumPosition);
  }


  // file crc decoded and checked within base class
}

RawFile LoadListFile::encodeLoadsInfo() const
{
  RawFile rawLoadsInfo( sizeof( uint16_t));

  // number of loads
  setInt< uint16_t>( rawLoadsInfo.begin(), numberOfLoads());

  // iterate over files
  uint16_t loadCounter( 0);
  for (auto const &loadInfo : loadsValue)
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
    loadInfoIt = setInt< uint16_t>(
      loadInfoIt,
      (loadCounter == numberOfLoads()) ?
        (0U) :
        (rawLoadInfo.size() / 2));

    // part number
    loadInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), loadInfoIt);

    // header filename
    loadInfoIt = std::copy( rawHeaderFilename.begin(), rawHeaderFilename.end(), loadInfoIt);

    // member sequence number
    loadInfoIt = setInt< uint16_t>( loadInfoIt, loadInfo.memberSequenceNumber());

    // THW IDs list
    loadInfoIt = std::copy( rawThwIds.begin(), rawThwIds.end(), loadInfoIt);

    // add file info to files info
    rawLoadsInfo.insert( rawLoadsInfo.end(), rawLoadInfo.begin(), rawLoadInfo.end());
  }

  return rawLoadsInfo;
}

void LoadListFile::decodeLoadsInfo(
  const RawFile &rawFile,
  std::size_t offset)
{
  auto it( rawFile.begin() + offset);

  // number of loads
  uint16_t numberOfLoads;
  it = getInt< uint16_t>( it, numberOfLoads);

  // iterate of load counter
  for ( unsigned int loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex)
  {
    auto listIt( it);

    // next load pointer
    uint16_t loadPointer;
    listIt = getInt< uint16_t>( listIt, loadPointer);

    //! @todo check pointer for != 0 (all except last ==> OK, last ==> error)

    // part number
    std::string partNumber;
    listIt = Arinc665File::decodeString( listIt, partNumber);

    // header filename
    std::string headerFilename;
    listIt = Arinc665File::decodeString( listIt, headerFilename);

    // member sequence number
    uint16_t fileMemberSequenceNumber;
    listIt = getInt< uint16_t>( listIt, fileMemberSequenceNumber);
    if (( fileMemberSequenceNumber < 1 ) || (fileMemberSequenceNumber > 255))
    {
      //! @throw InvalidArinc665File When member sequence number is out of range
      BOOST_THROW_EXCEPTION( InvalidArinc665File() <<
        AdditionalInfo( "member sequence number out of range"));
    }

    LoadInfo::ThwIds thwIds;
    listIt = Arinc665File::decodeStringList( listIt, thwIds);

    loadsValue.emplace_back(
      std::move( partNumber),
      std::move( headerFilename),
      static_cast< uint8_t>( fileMemberSequenceNumber),
      std::move( thwIds));

    // set it to begin of next load
    it += loadPointer * 2;
  }
}

}

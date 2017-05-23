/*
 * $Date$
 * $Revision$
 */
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

namespace Arinc665 {
namespace File {

LoadListFile::LoadListFile( Arinc665Version version):
  ListFile( FileType::LoadList, version),
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
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

LoadListFile::string LoadListFile::getMediaSetPn() const
{
  return mediaSetPn;
}

void LoadListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t LoadListFile::getMediaSequenceNumber() const
{
  return mediaSequenceNumber;
}

void LoadListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t LoadListFile::getNumberOfMediaSetMembers() const
{
  return numberOfMediaSetMembers;
}

void LoadListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  this->numberOfMediaSetMembers = numberOfMediaSetMembers;
}

size_t LoadListFile::getNumberOfLoads() const
{
  return loadsInfo.size();
}

const LoadsInfo& LoadListFile::getLoadsInfo() const
{
  return loadsInfo;
}

LoadsInfo& LoadListFile::getLoadsInfo()
{
  return loadsInfo;
}

LoadListFile::LoadsInfoMap LoadListFile::getLoadsInfoAsMap() const
{
  LoadsInfoMap loads;

  for (const auto & loadInfo : loadsInfo)
  {
    loads.insert( std::make_pair(
      std::make_pair(
        loadInfo.getMemberSequenceNumber(),
        loadInfo.getHeaderFilename()),
      loadInfo));
  }

  return loads;
}

void LoadListFile::addLoadInfo( const LoadInfo &loadInfo)
{
  loadsInfo.push_back( loadInfo);
}

void LoadListFile::addLoadInfo( LoadInfo &&loadInfo)
{
  loadsInfo.push_back( loadInfo);
}

const LoadListFile::UserDefinedData& LoadListFile::getUserDefinedData() const
{
  return userDefinedData;
}

void LoadListFile::setUserDefinedData( const UserDefinedData &userDefinedData)
{
  BOOST_LOG_FUNCTION();

  this->userDefinedData = userDefinedData;

  if (userDefinedData.size() % 2 != 0)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), severity_level::warning) <<
      "User defined data must be 2-byte aligned. - extending range";

    this->userDefinedData.push_back(0);
  }
}

bool LoadListFile::belongsToSameMediaSet( const LoadListFile &other) const
{
  return
    (mediaSetPn == other.getMediaSetPn()) &&
    (numberOfMediaSetMembers == other.getNumberOfMediaSetMembers()) &&
    (loadsInfo == other.getLoadsInfo()) &&
    (userDefinedData == other.getUserDefinedData());
}

RawFile LoadListFile::encode() const
{
  RawFile rawFile(
    BaseHeaderOffset +
    3 * sizeof( uint32_t) + // mediaInformationPtr, LoadsListPtr, userDefinedDataPtr
    2 * sizeof( uint8_t)  + // media sequence number, number of media set members
    sizeof( uint16_t));     // crc

  auto rawMediaSetPn( encodeString( getMediaSetPn()));
  assert( rawMediaSetPn.size() % 2 == 0);
  auto rawLoadsInfo( encodeLoadsInfo());
  assert( rawLoadsInfo.size() % 2 == 0);

  auto it( rawFile.begin() + BaseHeaderOffset);

  // media information pointer
  uint32_t mediaInformationPtr =
    (BaseHeaderOffset + (3 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, mediaInformationPtr);

  // file list pointer
  uint32_t fileListPtr =
    mediaInformationPtr + (2 * sizeof( uint8_t)) / 2 + rawMediaSetPn.size() / 2;
  it = setInt< uint32_t>( it, fileListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedData.empty() ? 0 : fileListPtr + rawLoadsInfo.size() / 2;
  it = setInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.insert( it, rawMediaSetPn.begin(), rawMediaSetPn.end());
  it += rawMediaSetPn.size();

  // media sequence number
  it = setInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = setInt< uint8_t>( it, numberOfMediaSetMembers);

  // loads list
  it = rawFile.insert( it, rawLoadsInfo.begin(), rawLoadsInfo.end());
  it += rawFile.size();

  if (!userDefinedData.empty())
  {
    assert( userDefinedData.size() % 2 == 0);
    rawFile.insert( it, userDefinedData.begin(), userDefinedData.end());
  }

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void LoadListFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  RawFile::const_iterator it = rawFile.begin() + BaseHeaderOffset;

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t loadListPtr;
  it = getInt< uint32_t>( it, loadListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.begin() + mediaInformationPtr * 2;
  it = decodeString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // load list
  loadsInfo = decodeLoadsInfo( rawFile, 2 * loadListPtr);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, rawFile.end() - 2);
  }

  // file crc decoded and checked within base class
}

RawFile LoadListFile::encodeLoadsInfo() const
{
  RawFile rawLoadsInfo( sizeof( uint16_t));

   // number of loads
   setInt< uint16_t>( rawLoadsInfo.begin(), getNumberOfLoads());

   // iterate over files
   uint16_t loadCounter( 0);
   for (auto const &loadInfo : getLoadsInfo())
   {
     ++loadCounter;
     auto const rawPartNumber( encodeString( loadInfo.getPartNumber()));
     assert( rawPartNumber.size() % 2 == 0);
     auto const rawHeaderFilename( encodeString( loadInfo.getHeaderFilename()));
     assert( rawHeaderFilename.size() % 2 == 0);
     auto const rawThwIds( encodeStringList( loadInfo.getTargetHardwareIdList()));
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
       (loadCounter == getNumberOfLoads()) ?
         (0U) :
         (rawLoadInfo.size() / 2));

     // part number
     loadInfoIt = std::copy( rawPartNumber.begin(), rawPartNumber.end(), loadInfoIt);

     // header filename
     loadInfoIt = std::copy( rawHeaderFilename.begin(), rawHeaderFilename.end(), loadInfoIt);

     // member sequence number
     loadInfoIt = setInt< uint16_t>( loadInfoIt, loadInfo.getMemberSequenceNumber());

     // THW IDs list
     loadInfoIt = std::copy( rawThwIds.begin(), rawThwIds.end(), loadInfoIt);

     // add file info to files info
     rawLoadsInfo.insert( rawLoadsInfo.end(), rawLoadInfo.begin(), rawLoadInfo.end());
   }

   return rawLoadsInfo;
}

LoadsInfo LoadListFile::decodeLoadsInfo(
  const RawFile &rawFile,
  std::size_t offset)
{
  auto it( rawFile.begin() + offset);

  LoadsInfo loadsInfo;

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
    string partNumber;
    listIt = Arinc665File::decodeString( listIt, partNumber);

    // header filename
    string headerFilename;
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

    loadsInfo.emplace_back(
      std::move( partNumber),
        std::move( headerFilename),
      static_cast< uint8_t>( fileMemberSequenceNumber),
      std::move( thwIds));

    // set it to begin of next load
    it += loadPointer * 2;
  }

  return loadsInfo;
}

}
}

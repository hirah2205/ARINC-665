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

#include <arinc665/file/StringHelper.hpp>

#include <arinc665/Arinc665Logger.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665 {
namespace File {

LoadListFile::LoadListFile( Arinc665Version version):
  ListFile( FileType::LoadList, version),
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
{
}

LoadListFile::LoadListFile( const RawFile &file):
  ListFile( FileType::LoadList, file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  decodeBody( file);
}

LoadListFile& LoadListFile::operator=( const RawFile &file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  decodeBody( file);

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
  return loadInfos.size();
}

const LoadListFile::LoadInfoList& LoadListFile::getLoadInfos() const
{
  return loadInfos;
}

LoadListFile::LoadInfoList& LoadListFile::getLoadInfos()
{
  return loadInfos;
}

LoadListFile::LoadInfoMap LoadListFile::getLoadInfosAsMap() const
{
  LoadInfoMap loads;

  for (const auto & loadInfo : loadInfos)
  {
    loads.insert( std::make_pair(
      std::make_pair(
        loadInfo.getMemberSequenceNumber(),
        loadInfo.getHeaderFilename()),
      loadInfo));
  }

  return loads;
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
    (loadInfos == other.getLoadInfos()) &&
    (userDefinedData == other.getUserDefinedData());
}

RawFile LoadListFile::encode() const
{
  RawFile file( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // set header and crc
  insertHeader( file);

  return file;
}

void LoadListFile::decodeBody( const RawFile &file)
{
  // set processing start to position after spare
  RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t loadListPtr;
  it = getInt< uint32_t>( it, loadListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = file.begin() + mediaInformationPtr * 2;
  it = getString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // load list
  it = file.begin() + 2 * loadListPtr;
  loadInfos = LoadInfo::getLoadInfos( it);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 2);
  }

  // file crc decoded and checked within base class
}

}
}

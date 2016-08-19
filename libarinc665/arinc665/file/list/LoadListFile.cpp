/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Definition of class LoadListFile
 **/

#include "LoadListFile.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

LoadListFile::LoadListFile( void):
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
{
}

LoadListFile::LoadListFile( const RawFile &file) :
  ListFile( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2)
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
  loadList = LoadInfo::getLoadList( it);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 2);
  }

  // file crc decoded and checked within base class
}

Arinc665::Arinc665Version LoadListFile::getArincVersion( void) const
{
  return Arinc665Version::ARINC_665_2;
}

LoadListFile::string LoadListFile::getMediaSetPn( void) const
{
  return mediaSetPn;
}

void LoadListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t LoadListFile::getMediaSequenceNumber( void) const
{
  return mediaSequenceNumber;
}

void LoadListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t LoadListFile::getNumberOfMediaSetMembers( void) const
{
  return numberOfMediaSetMembers;
}

void LoadListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  this->numberOfMediaSetMembers = numberOfMediaSetMembers;
}

unsigned int LoadListFile::getNumberOfLoads( void) const
{
  return loadList.size();
}

const LoadListFile::ListType& LoadListFile::getLoads( void) const
{
  return loadList;
}

LoadListFile::ListType& LoadListFile::getLoads( void)
{
  return loadList;
}

}
}

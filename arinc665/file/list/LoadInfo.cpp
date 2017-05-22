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
 * @brief Definition of class Arinc665::File::LoadInfo.
 **/

#include "LoadInfo.hpp"

#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/list/FileInfo.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665 {
namespace File {

LoadsInfo LoadInfo::getLoadsInfo( RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  LoadsInfo loadList;

  // number of loads
  uint16_t numberOfLoads;
  workIt = getInt< uint16_t>( workIt, numberOfLoads);

  for ( unsigned int loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex)
  {
    loadList.push_back( LoadInfo( workIt));
  }

  it = workIt;

  return loadList;
}

LoadInfo::LoadInfo():
  memberSequenceNumber( 0)
{
}

LoadInfo::LoadInfo( RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  // next load pointer
  uint16_t loadPointer;
  workIt = getInt< uint16_t>( workIt, loadPointer);

  // part number
  workIt = Arinc665File::decodeString( workIt, partNumber);

  // header filename
  workIt = Arinc665File::decodeString( workIt, headerFilename);

  // member sequence number
  uint16_t fileMemberSequenceNumber;
  workIt = getInt< uint16_t>( workIt, fileMemberSequenceNumber);
  if (( fileMemberSequenceNumber < 1 ) || (fileMemberSequenceNumber > 255))
  {
    //! @throw XXX
    BOOST_THROW_EXCEPTION( InvalidArinc665File());
  }
  memberSequenceNumber = static_cast< uint8_t>( fileMemberSequenceNumber);

  workIt = Arinc665File::decodeStringList( workIt, targetHardwareIds);

  // set it to begin of next load
  it += loadPointer * 2;
}

LoadInfo::string LoadInfo::getPartNumber() const
{
  return partNumber;
}

void LoadInfo::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

LoadInfo::string LoadInfo::getHeaderFilename() const
{
  return headerFilename;
}

void LoadInfo::setHeaderFilename( const string &headerFilename)
{
  this->headerFilename = headerFilename;
}

uint8_t LoadInfo::getMemberSequenceNumber() const
{
  return memberSequenceNumber;
}

void LoadInfo::setMemberSequenceNumber( const uint8_t memberSequenceNumber)
{
  this->memberSequenceNumber = memberSequenceNumber;
}

const LoadInfo::ThwIds& LoadInfo::getTargetHardwareIdList() const
{
  return targetHardwareIds;
}

LoadInfo::ThwIds& LoadInfo::getTargetHardwareIdList()
{
  return targetHardwareIds;
}

bool LoadInfo::operator ==( const LoadInfo &other) const
{
  return
    (partNumber == other.getPartNumber()) &&
    (headerFilename == other.getHeaderFilename()) &&
    (memberSequenceNumber == other.getMemberSequenceNumber()) &&
    (targetHardwareIds == other.getTargetHardwareIdList());
}

bool LoadInfo::operator !=( const LoadInfo &other) const
{
  return !(*this == other);
}

bool LoadInfo::operator ==( const FileInfo &other) const
{
  return
    (headerFilename == other.getFilename()) &&
    (memberSequenceNumber == other.getMemberSequenceNumber());
}

bool LoadInfo::operator !=( const FileInfo &other) const
{
  return !(*this == other);
}

bool operator ==( const FileInfo &fileInfo, const LoadInfo &loadInfo)
{
  // delegate to class member compare operator
  return loadInfo == fileInfo;
}

bool operator !=( const FileInfo &fileInfo, const LoadInfo &loadInfo)
{
  // delegate to class member compare operator
  return loadInfo != fileInfo;
}

}
}

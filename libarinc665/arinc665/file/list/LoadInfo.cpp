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

#include <arinc665/file/StringHelper.hpp>
#include <arinc665/file/list/FileInfo.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

LoadInfo::LoadInfos LoadInfo::getLoadInfos( RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  std::list< LoadInfo> loadList;

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

LoadInfo::LoadInfo( void):
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
  workIt = getString( workIt, partNumber);

  // header filename
  workIt = getString( workIt, headerFilename);

  // member sequence number
  workIt = getInt< uint16_t>( workIt, memberSequenceNumber);

  workIt = getStringList( workIt, targetHardwareIds);

  // set it to begin of next load
  it += loadPointer * 2;
}

LoadInfo::string LoadInfo::getPartNumber( void) const
{
  return partNumber;
}

void LoadInfo::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

LoadInfo::string LoadInfo::getHeaderFilename( void) const
{
  return headerFilename;
}

void LoadInfo::setHeaderFilename( const string &headerFilename)
{
  this->headerFilename = headerFilename;
}

uint16_t LoadInfo::getMemberSequenceNumber( void) const
{
  return memberSequenceNumber;
}

void LoadInfo::setMemberSequenceNumber( const uint16_t memberSequenceNumber)
{
  this->memberSequenceNumber = memberSequenceNumber;
}

const LoadInfo::ThwIds& LoadInfo::getTargetHardwareIdList( void) const
{
  return targetHardwareIds;
}

LoadInfo::ThwIds& LoadInfo::getTargetHardwareIdList( void)
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

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

#include <arinc665/file/FileInfo.hpp>

namespace Arinc665 {
namespace File {

LoadInfo::LoadInfo(
  const string &partNumber,
  const string &headerFilename,
  const uint8_t memberSequenceNumber,
  const ThwIds &targetHardwareIds):
  partNumber( partNumber),
  headerFilename( headerFilename),
  memberSequenceNumber( memberSequenceNumber),
  targetHardwareIds( targetHardwareIds)
{
}

LoadInfo::LoadInfo(
  string &&partNumber,
  string &&headerFilename,
  const uint8_t memberSequenceNumber,
  ThwIds &&targetHardwareIds):
  partNumber( partNumber),
  headerFilename( headerFilename),
  memberSequenceNumber( memberSequenceNumber),
  targetHardwareIds( targetHardwareIds)
{
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

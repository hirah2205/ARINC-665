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

namespace Arinc665::File {

LoadInfo::LoadInfo(
  const std::string &partNumber,
  const std::string &headerFilename,
  const uint8_t memberSequenceNumber,
  const ThwIds &targetHardwareIds):
  partNumberValue( partNumber),
  headerFilenameValue( headerFilename),
  memberSequenceNumberValue( memberSequenceNumber),
  targetHardwareIdsValue( targetHardwareIds)
{
}

LoadInfo::LoadInfo(
  std::string &&partNumber,
  std::string &&headerFilename,
  const uint8_t memberSequenceNumber,
  ThwIds &&targetHardwareIds):
  partNumberValue( partNumber),
  headerFilenameValue( headerFilename),
  memberSequenceNumberValue( memberSequenceNumber),
  targetHardwareIdsValue( targetHardwareIds)
{
}

std::string LoadInfo::partNumber() const
{
  return partNumberValue;
}

void LoadInfo::partNumber( const std::string &partNumber)
{
  partNumberValue = partNumber;
}

std::string LoadInfo::headerFilename() const
{
  return headerFilenameValue;
}

void LoadInfo::headerFilename( const std::string &headerFilename)
{
  headerFilenameValue = headerFilename;
}

uint8_t LoadInfo::memberSequenceNumber() const
{
  return memberSequenceNumberValue;
}

void LoadInfo::memberSequenceNumber( const uint8_t memberSequenceNumber)
{
  memberSequenceNumberValue = memberSequenceNumber;
}

const LoadInfo::ThwIds& LoadInfo::targetHardwareIds() const
{
  return targetHardwareIdsValue;
}

LoadInfo::ThwIds& LoadInfo::targetHardwareIds()
{
  return targetHardwareIdsValue;
}

void LoadInfo::addTargetHardwareId( const std::string &targetHardwareId)
{
  targetHardwareIdsValue.push_back( targetHardwareId);
}

void LoadInfo::addTargetHardwareId( std::string &&targetHardwareId)
{
  targetHardwareIdsValue.push_back( targetHardwareId);
}

bool LoadInfo::operator ==( const LoadInfo &other) const
{
  return
    (partNumberValue == other.partNumber()) &&
    (headerFilenameValue == other.headerFilename()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber()) &&
    (targetHardwareIdsValue == other.targetHardwareIds());
}

bool LoadInfo::operator !=( const LoadInfo &other) const
{
  return !(*this == other);
}

bool LoadInfo::operator ==( const FileInfo &other) const
{
  return
    (headerFilenameValue == other.filename()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber());
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

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::LoadInfo.
 **/

#include "LoadInfo.hpp"

#include <arinc665/file/FileInfo.hpp>

namespace Arinc665::File {

LoadInfo::LoadInfo(
  std::string_view partNumber,
  std::string_view headerFilename,
  const uint8_t memberSequenceNumber,
  const ThwIds &targetHardwareIds):
  partNumberValue{ partNumber},
  headerFilenameValue{ headerFilename},
  memberSequenceNumberValue{ memberSequenceNumber},
  targetHardwareIdsValue{ targetHardwareIds}
{
}

LoadInfo::LoadInfo(
  std::string &&partNumber,
  std::string &&headerFilename,
  const uint8_t memberSequenceNumber,
  ThwIds &&targetHardwareIds):
  partNumberValue{ partNumber},
  headerFilenameValue{ headerFilename},
  memberSequenceNumberValue{ memberSequenceNumber},
  targetHardwareIdsValue{ targetHardwareIds}
{
}

std::string_view LoadInfo::partNumber() const
{
  return partNumberValue;
}

void LoadInfo::partNumber( std::string_view partNumber)
{
  partNumberValue = partNumber;
}

void LoadInfo::partNumber( std::string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

std::string_view LoadInfo::headerFilename() const
{
  return headerFilenameValue;
}

void LoadInfo::headerFilename( std::string_view headerFilename)
{
  headerFilenameValue = headerFilename;
}

void LoadInfo::headerFilename( std::string &&headerFilename)
{
  headerFilenameValue = std::move( headerFilename);
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

void LoadInfo::targetHardwareId( std::string_view targetHardwareId)
{
  targetHardwareIdsValue.emplace_back( targetHardwareId);
}

void LoadInfo::targetHardwareId( std::string &&targetHardwareId)
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

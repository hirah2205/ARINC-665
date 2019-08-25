/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::FileInfo.
 **/

#include "FileInfo.hpp"

#include <algorithm>

namespace Arinc665::File {

FileInfo::FileInfo(
  std::string_view filename,
  std::string_view pathName,
  const uint16_t memberSequenceNumber,
  const uint16_t crc,
  const std::optional< CheckValue> &checkValue):
  filenameValue( filename),
  pathNameValue( pathName),
  memberSequenceNumberValue( memberSequenceNumber),
  crcValue( crc),
  checkValueValue( checkValue)
{
}

FileInfo::FileInfo(
  std::string &&filename,
  std::string &&pathName,
  uint16_t memberSequenceNumber,
  uint16_t crc,
  std::optional< CheckValue> &&checkValue):
  filenameValue( std::move( filename)),
  pathNameValue( std::move( pathName)),
  memberSequenceNumberValue( memberSequenceNumber),
  crcValue( crc),
  checkValueValue( std::move( checkValue))
{
}

std::string_view FileInfo::filename() const
{
  return filenameValue;
}

void FileInfo::filename( std::string_view filename)
{
  filenameValue = filename;
}

std::string_view FileInfo::pathName() const
{
  return pathNameValue;
}

void FileInfo::pathName( std::string_view pathName)
{
  pathNameValue = pathName;
}

std::filesystem::path FileInfo::path() const
{
  std::string newPathName( pathNameValue);

  std::replace( newPathName.begin(), newPathName.end(), '\\', '/');

  return std::filesystem::path{ newPathName} / filenameValue;
}

uint16_t FileInfo::memberSequenceNumber() const
{
  return memberSequenceNumberValue;
}

void FileInfo::memberSequenceNumber( const uint16_t memberSequenceNumber)
{
  memberSequenceNumberValue = memberSequenceNumber;
}

uint16_t FileInfo::crc() const
{
  return crcValue;
}

void FileInfo::crc( const uint16_t crc)
{
  crcValue = crc;
}

const std::optional< CheckValue>& FileInfo::checkValue() const
{
  return checkValueValue;
}

void FileInfo::checkValue( const std::optional< CheckValue> &checkValue)
{
  checkValueValue = checkValue;
}

void FileInfo::checkValue( std::optional< CheckValue> &&checkValue)
{
  checkValueValue = std::move( checkValue);
}

bool FileInfo::operator ==( const FileInfo &other) const
{
  return
    (filenameValue == other.filename()) &&
    (pathNameValue == other.pathName()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber()) &&
    (crcValue == other.crc()) &&
    (checkValueValue == other.checkValue());
}

bool FileInfo::operator !=( const FileInfo &other) const
{
  return !(*this == other);
}

}

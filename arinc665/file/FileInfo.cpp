/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::File::FileInfo.
 **/

#include "FileInfo.hpp"

#include <algorithm>

namespace Arinc665 {
namespace File {

FileInfo::FileInfo(
  const string &filename,
  const string &pathName,
  uint16_t memberSequenceNumber,
  uint16_t crc):
  filenameValue( filename),
  pathNameValue( pathName),
  memberSequenceNumberValue( memberSequenceNumber),
  crcValue( crc)
{
}

FileInfo::FileInfo(
  string &&filename,
  string &&pathName,
  uint16_t memberSequenceNumber,
  uint16_t crc):
  filenameValue( std::move( filename)),
  pathNameValue( std::move( pathName)),
  memberSequenceNumberValue( memberSequenceNumber),
  crcValue( crc)
{
}

const FileInfo::string& FileInfo::filename() const
{
  return filenameValue;
}

void FileInfo::filename( const string &filename)
{
  filenameValue = filename;
}

FileInfo::string FileInfo::pathName() const
{
  return pathNameValue;
}

void FileInfo::pathName( const string &pathName)
{
  pathNameValue = pathName;
}

FileInfo::fpath FileInfo::path() const
{
  string newPathName( pathNameValue);

  std::replace( newPathName.begin(), newPathName.end(), '\\', '/');

  return fpath( newPathName) / filenameValue;
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

bool FileInfo::operator ==( const FileInfo &other) const
{
  return
    (filenameValue == other.filename()) &&
    (pathNameValue == other.pathName()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber()) &&
    (crcValue == other.crc());
}

bool FileInfo::operator !=( const FileInfo &other) const
{
  return !(*this == other);
}

}
}

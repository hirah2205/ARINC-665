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
  filename( filename),
  pathName( pathName),
  memberSequenceNumber( memberSequenceNumber),
  crc( crc)
{
}

FileInfo::string FileInfo::getFilename() const
{
  return filename;
}

void FileInfo::setFilename( const string &filename)
{
  this->filename = filename;
}

FileInfo::string FileInfo::getPathName() const
{
  return pathName;
}

void FileInfo::setPathName( const string &pathName)
{
  this->pathName = pathName;
}

FileInfo::path FileInfo::getPath() const
{
  string newPathName( pathName);

  std::replace( newPathName.begin(), newPathName.end(), '\\', '/');

  return path( newPathName) / filename;
}

uint16_t FileInfo::getMemberSequenceNumber() const
{
  return memberSequenceNumber;
}

void FileInfo::setMemberSequenceNumber( const uint16_t memberSequenceNumber)
{
  this->memberSequenceNumber = memberSequenceNumber;
}

uint16_t FileInfo::getCrc() const
{
  return crc;
}

void FileInfo::setCrc( const uint16_t crc)
{
  this->crc = crc;
}

bool FileInfo::operator ==( const FileInfo &other) const
{
  return
    (filename == other.getFilename()) &&
    (pathName == other.getPathName()) &&
    (memberSequenceNumber == other.getMemberSequenceNumber()) &&
    (crc == other.getCrc());
}

bool FileInfo::operator !=( const FileInfo &other) const
{
  return !(*this == other);
}

}
}

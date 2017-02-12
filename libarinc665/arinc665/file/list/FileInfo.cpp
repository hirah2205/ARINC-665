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

#include <arinc665/file/StringHelper.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

#include <algorithm>

namespace Arinc665 {
namespace File {

FileInfo::FileInfoList FileInfo::getFileInfos(
  RawFile::const_iterator &it)
{
  //! @todo pass iterator by value
  FileInfoList fileList;

  // number of files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    fileList.push_back( FileInfo( it));
  }

  return fileList;
}

FileInfo::FileInfo():
  memberSequenceNumber( 0),
  crc( 0)
{
}

FileInfo::FileInfo( RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  // next file pointer
  uint16_t filePointer;
  workIt = getInt< uint16_t>( workIt, filePointer);

  // filename
  workIt = getString( workIt, filename);

  // path name
  workIt = getString( workIt, pathName);

  // member sequence number
  workIt = getInt< uint16_t>( workIt, memberSequenceNumber);

  // crc
  workIt = getInt< uint16_t>( workIt, crc);

  // set it to begin of next file
  it += filePointer * 2;
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

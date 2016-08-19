/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Definition of class FileInfo
 **/

#include "FileInfo.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

std::list< FileInfo> FileInfo::getFileList(
  std::vector< uint8_t>::const_iterator &it)
{
  //! @todo pass iterator by value
  std::list< FileInfo> fileList;

  // number of files
  uint16_t numberOfFiles;
  it = getInt< uint16_t>( it, numberOfFiles);

  for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
  {
    fileList.push_back( FileInfo( it));
  }

  return fileList;
}

FileInfo::FileInfo( void):
  memberSequenceNumber( 0),
  crc( 0)
{
}

FileInfo::FileInfo( std::vector< uint8_t>::const_iterator &it)
{
  std::vector< uint8_t>::const_iterator workIt = it;

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

FileInfo::string FileInfo::getFilename( void) const
{
  return filename;
}

void FileInfo::setFilename( const string &filename)
{
  this->filename = filename;
}

std::string FileInfo::getPathName( void) const
{
  return pathName;
}

void FileInfo::setPathName( const std::string &pathName)
{
  this->pathName = pathName;
}

uint16_t FileInfo::getMemberSequenceNumber( void) const
{
  return memberSequenceNumber;
}

void FileInfo::setMemberSequenceNumber( const uint16_t memberSequenceNumber)
{
  this->memberSequenceNumber = memberSequenceNumber;
}

uint16_t FileInfo::getCrc( void) const
{
  return crc;
}

void FileInfo::setCrc( const uint16_t crc)
{
  this->crc = crc;
}

}
}

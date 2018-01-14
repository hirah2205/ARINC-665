/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::File::BatchInfo.
 **/

#include "BatchInfo.hpp"

#include <arinc665/file/FileInfo.hpp>

namespace Arinc665 {
namespace File {

BatchInfo::BatchInfo(
  const string &partNumber,
  const string &filename,
  uint16_t memberSequenceNumber):
  partNumber( partNumber),
  filename( filename),
  memberSequenceNumber( memberSequenceNumber)
{
}

BatchInfo::BatchInfo(
  string &&partNumber,
  string &&filename,
  uint16_t memberSequenceNumber):
  partNumber( partNumber),
  filename( filename),
  memberSequenceNumber( memberSequenceNumber)
{
}

const BatchInfo::string& BatchInfo::getPartNumber() const
{
  return partNumber;
}

void BatchInfo::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

void BatchInfo::setPartNumber( string &&partNumber)
{
  this->partNumber = std::move( partNumber);
}

const BatchInfo::string& BatchInfo::getFilename() const
{
  return filename;
}

void BatchInfo::setFilename( const string &filename)
{
  this->filename = filename;
}

void BatchInfo::setFilename( string &&filename)
{
  this->filename = std::move( filename);
}

uint16_t BatchInfo::getMemberSequenceNumber() const
{
  return memberSequenceNumber;
}

void BatchInfo::setMemberSequenceNumber( const uint16_t memberSequenceNumber)
{
  this->memberSequenceNumber = memberSequenceNumber;
}

bool BatchInfo::operator ==( const BatchInfo &other) const
{
  return
    (partNumber == other.getPartNumber()) &&
    (filename == other.getFilename()) &&
    (memberSequenceNumber == other.getMemberSequenceNumber());
}

bool BatchInfo::operator !=( const BatchInfo &other) const
{
  return !(*this == other);
}

bool BatchInfo::operator ==( const FileInfo &other) const
{
  return
    (filename == other.filename()) &&
    (memberSequenceNumber == other.memberSequenceNumber());
}

bool BatchInfo::operator !=( const FileInfo &other) const
{
  return !(*this == other);
}

bool operator ==( const FileInfo &fileInfo, const BatchInfo &batchInfo)
{
  return batchInfo == fileInfo;
}

bool operator !=( const FileInfo &fileInfo, const BatchInfo &batchInfo)
{
  return batchInfo != fileInfo;
}

}
}

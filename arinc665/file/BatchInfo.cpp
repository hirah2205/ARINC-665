/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::BatchInfo.
 **/

#include "BatchInfo.hpp"

#include <arinc665/file/FileInfo.hpp>

namespace Arinc665::File {

BatchInfo::BatchInfo(
  const std::string &partNumber,
  const std::string &filename,
  uint16_t memberSequenceNumber):
  partNumberValue( partNumber),
  filenameValue( filename),
  memberSequenceNumberValue( memberSequenceNumber)
{
}

BatchInfo::BatchInfo(
  std::string &&partNumber,
  std::string &&filename,
  uint16_t memberSequenceNumber):
  partNumberValue( partNumber),
  filenameValue( filename),
  memberSequenceNumberValue( memberSequenceNumber)
{
}

const std::string& BatchInfo::partNumber() const
{
  return partNumberValue;
}

void BatchInfo::partNumber( const std::string &partNumber)
{
  partNumberValue = partNumber;
}

void BatchInfo::partNumber( std::string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

const std::string& BatchInfo::filename() const
{
  return filenameValue;
}

void BatchInfo::filename( const std::string &filename)
{
  filenameValue = filename;
}

void BatchInfo::filename( std::string &&filename)
{
  filenameValue = std::move( filename);
}

uint16_t BatchInfo::memberSequenceNumber() const
{
  return memberSequenceNumberValue;
}

void BatchInfo::memberSequenceNumber( const uint16_t memberSequenceNumber)
{
  memberSequenceNumberValue = memberSequenceNumber;
}

bool BatchInfo::operator ==( const BatchInfo &other) const
{
  return
    (partNumberValue == other.partNumber()) &&
    (filenameValue == other.filename()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber());
}

bool BatchInfo::operator !=( const BatchInfo &other) const
{
  return !(*this == other);
}

bool BatchInfo::operator ==( const FileInfo &other) const
{
  return
    (filenameValue == other.filename()) &&
    (memberSequenceNumberValue == other.memberSequenceNumber());
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

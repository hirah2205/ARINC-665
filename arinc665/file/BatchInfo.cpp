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
  std::string_view partNumber,
  std::string_view filename,
  uint16_t memberSequenceNumber):
  partNumberV{ partNumber},
  filenameV{ filename}, memberSequenceNumberV{ memberSequenceNumber}
{
}

BatchInfo::BatchInfo(
  std::string &&partNumber,
  std::string &&filename,
  uint16_t memberSequenceNumber):
  partNumberV{ partNumber},
  filenameV{ filename}, memberSequenceNumberV{ memberSequenceNumber}
{
}

std::string_view BatchInfo::partNumber() const
{
  return partNumberV;
}

void BatchInfo::partNumber( std::string_view partNumber)
{
  partNumberV = partNumber;
}

void BatchInfo::partNumber( std::string &&partNumber)
{
  partNumberV = std::move( partNumber);
}

std::string_view BatchInfo::filename() const
{
  return filenameV;
}

void BatchInfo::filename( std::string_view filename)
{
  filenameV = filename;
}

void BatchInfo::filename( std::string &&filename)
{
  filenameV = std::move( filename);
}

uint16_t BatchInfo::memberSequenceNumber() const
{
  return memberSequenceNumberV;
}

void BatchInfo::memberSequenceNumber( const uint16_t memberSequenceNumber)
{
  memberSequenceNumberV = memberSequenceNumber;
}

bool BatchInfo::operator ==( const BatchInfo &other) const
{
  return
    ( partNumberV == other.partNumber()) &&
    ( filenameV == other.filename()) &&
    ( memberSequenceNumberV == other.memberSequenceNumber());
}

bool BatchInfo::operator !=( const BatchInfo &other) const
{
  return !(*this == other);
}

bool BatchInfo::operator ==( const FileInfo &other) const
{
  return
    ( filenameV == other.filename()) &&
    ( memberSequenceNumberV == other.memberSequenceNumber());
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

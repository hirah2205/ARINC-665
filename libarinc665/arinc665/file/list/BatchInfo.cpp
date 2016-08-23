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
 * @brief Definition of class Arinc665::File::BatchInfo.
 **/

#include "BatchInfo.hpp"

#include <arinc665/file/list/FileInfo.hpp>
#include <arinc665/file/StringHelper.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

BatchInfo::BatchInfoList BatchInfo::getBatchInfos(
  RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  BatchInfoList batchInfo;

  // number of batches
  uint16_t numberOfBatches;
  workIt = getInt< uint16_t>( workIt, numberOfBatches);

  for ( unsigned int batchIndex = 0; batchIndex < numberOfBatches; ++batchIndex)
  {
    batchInfo.push_back( BatchInfo( workIt));
  }

  it = workIt;

  return batchInfo;
}

BatchInfo::BatchInfo( void):
  memberSequenceNumber( 0)
{
}

BatchInfo::BatchInfo( RawFile::const_iterator &it)
{
  RawFile::const_iterator workIt = it;

  // next batch pointer
  uint16_t batchPointer;
  workIt = getInt< uint16_t>( workIt, batchPointer);

  // part number
  workIt = getString( workIt, partNumber);

  // batch filename
  workIt = getString( workIt, filename);

  // member sequence number
  workIt = getInt< uint16_t>( workIt, memberSequenceNumber);

  // set it to begin of next batch
  it += batchPointer * 2;
}

BatchInfo::string BatchInfo::getPartNumber( void) const
{
  return partNumber;
}

void BatchInfo::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

BatchInfo::string BatchInfo::getFilename( void) const
{
  return filename;
}

void BatchInfo::setFilename( const string &filename)
{
  this->filename = filename;
}

uint16_t BatchInfo::getMemberSequenceNumber( void) const
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
    (filename == other.getFilename()) &&
    (memberSequenceNumber == other.getMemberSequenceNumber());
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

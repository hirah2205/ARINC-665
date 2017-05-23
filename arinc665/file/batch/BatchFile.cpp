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
 * @brief Definition of class Arinc665::File::BatchFile.
 **/

#include "BatchFile.hpp"

#include <helper/Endianess.hpp>

namespace Arinc665 {
namespace File {

BatchFile::BatchFile( const RawFile &rawFile):
  Arinc665File( FileType::BatchFile, rawFile)
{
  decodeBody( rawFile);
}

BatchFile& BatchFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

BatchFile::string BatchFile::getPartNumber() const
{
  return partNumber;
}

void BatchFile::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

BatchFile::string BatchFile::getComment() const
{
  return comment;
}

void BatchFile::setComment( const string &comment)
{
  this->comment = comment;
}

const BatchTargetsInfo& BatchFile::getTargetHardwares() const
{
  return targetHardwares;
}

BatchTargetsInfo& BatchFile::getTargetHardwares()
{
  return targetHardwares;
}

RawFile BatchFile::encode() const
{
  RawFile rawFile( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void BatchFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  auto it( rawFile.begin() + BaseHeaderOffset);

  uint32_t batchPartNumberPtr;
  it = getInt< uint32_t>( it, batchPartNumberPtr);

  uint32_t targetHardwareIdListPtr;
  it = getInt< uint32_t>( it, targetHardwareIdListPtr);

  // batch part number
  it = rawFile.begin() + batchPartNumberPtr * 2;
  it = decodeString( it, partNumber);

  // comment
  it = decodeString( it, comment);

  // target hardware id list
  decodeBatchTargetsInfo( rawFile, targetHardwareIdListPtr * 2);
}

BatchTargetsInfo BatchFile::decodeBatchTargetsInfo(
  const RawFile &rawFile,
  const std::size_t offset) const
{
  RawFile::const_iterator it( rawFile.begin() + offset);

  BatchTargetsInfo batchTargetsInfo;

  // number of target HW IDs
  uint16_t numberOfTargetHardwareIds;
  it = getInt< uint16_t>( it, numberOfTargetHardwareIds);

  // iterate over THW ID index
  for ( unsigned int thwIdIndex = 0; thwIdIndex < numberOfTargetHardwareIds; ++thwIdIndex)
  {
    auto listIt( it);

    // next THW ID pointer
    uint16_t thwIdPointer;
    listIt = getInt< uint16_t>( listIt, thwIdPointer);

    // THW ID
    string thwId;
    listIt = decodeString( listIt, thwId);

    // Loads list
    BatchLoadsInfo batchLoadsInfo;

    // number of loads
    uint16_t numberOfLoads;
    listIt = getInt< uint16_t>( listIt, numberOfLoads);

    // iterate over load index
    for ( unsigned int loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex)
    {
      // header filename
      string filename;
      listIt = decodeString( it, filename);

      // Load PN
      string partNumber;
      listIt = decodeString( it, partNumber);

      // Batch Load info
      batchLoadsInfo.emplace_back( filename, partNumber);
    }

    // set it to begin of next file
    it += thwIdPointer * 2;

    // THW ID info
    batchTargetsInfo.emplace_back( std::move( thwId), std::move( batchLoadsInfo));
  }

  return batchTargetsInfo;
}

}
}

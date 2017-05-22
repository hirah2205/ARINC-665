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
 * @brief Definition of class Arinc665::File::BatchListFile.
 **/

#include "BatchListFile.hpp"

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

BatchListFile::BatchListFile( Arinc665Version version):
  ListFile( FileType::BatchList, version),
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
{
}

BatchListFile::BatchListFile( const RawFile &rawFile):
  ListFile( FileType::BatchList, rawFile)
{
  decodeBody( rawFile);
}

BatchListFile& BatchListFile::operator=( const RawFile &rawFile)
{
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

BatchListFile::string BatchListFile::getMediaSetPn() const
{
  return mediaSetPn;
}

void BatchListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t BatchListFile::getMediaSequenceNumber() const
{
  return mediaSequenceNumber;
}

void BatchListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t BatchListFile::getNumberOfMediaSetMembers() const
{
  return numberOfMediaSetMembers;
}

void BatchListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  this->numberOfMediaSetMembers = numberOfMediaSetMembers;
}

unsigned int BatchListFile::getNumberOfBatches() const
{
  return batchesInfo.size();
}

const BatchesInfo& BatchListFile::getBatchesInfo() const
{
  return batchesInfo;
}

BatchesInfo& BatchListFile::getBatchesInfo()
{
  return batchesInfo;
}

BatchListFile::BatchInfoMap BatchListFile::getBatchesInfoAsMap() const
{
  BatchInfoMap batches;

  for (const auto & batch : batchesInfo)
  {
    batches.insert(
      std::make_pair(
        std::make_pair(
          batch.getMemberSequenceNumber(),
          batch.getFilename()),
        batch));
  }

  return batches;
}

const BatchListFile::UserDefinedData& BatchListFile::getUserDefinedData() const
{
  return userDefinedData;
}

void BatchListFile::setUserDefinedData( const UserDefinedData &userDefinedData)
{
  this->userDefinedData = userDefinedData;
}

bool BatchListFile::belongsToSameMediaSet( const BatchListFile &other) const
{
  return
    (mediaSetPn == other.getMediaSetPn()) &&
    (numberOfMediaSetMembers == other.getNumberOfMediaSetMembers()) &&
    (batchesInfo == other.getBatchesInfo());
}

RawFile BatchListFile::encode() const
{
  RawFile rawFile( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // media information ptr

  // batch list ptr

  // user defined data ptr

  // media set part number

  // media sequence number

  // number of media set members

  // batch list

  // user defined data

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void BatchListFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  RawFile::const_iterator it = rawFile.begin() + BaseHeaderOffset;

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t batchListPtr;
  it = getInt< uint32_t>( it, batchListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.begin() + mediaInformationPtr * 2;
  it = decodeString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // batch list
  it = rawFile.begin() + 2 * batchListPtr;
  batchesInfo = BatchInfo::getBatchesInfo( it);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, rawFile.end() - 2);
  }

  // file crc decoded and checked within base class
}

}
}

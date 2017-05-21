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

#include <arinc665/file/StringHelper.hpp>

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

BatchListFile::BatchListFile( const RawFile &file):
  ListFile( FileType::BatchList, file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  decodeBody( file);
}

BatchListFile& BatchListFile::operator=( const RawFile &file)
{
  Arinc665File::operator =( file);
  decodeBody( file);

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
  return batchInfoList.size();
}

const BatchListFile::BatchInfoList& BatchListFile::getBatchInfos() const
{
  return batchInfoList;
}

BatchListFile::BatchInfoList& BatchListFile::getBatchInfos()
{
  return batchInfoList;
}

BatchListFile::BatchInfoMap BatchListFile::getBatchInfosAsMap() const
{
  BatchInfoMap batches;

  for (const auto & batch : batchInfoList)
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
    (batchInfoList == other.getBatchInfos());
}

RawFile BatchListFile::encode() const
{
  RawFile file( BaseHeaderOffset + 3 * sizeof( uint32_t));

  // media information ptr

  // batch list ptr

  // user defined data ptr

  // media set part number

  // media sequence number

  // number of media set members

  // batch list

  // user defined data

  // set header and crc
  insertHeader( file);

  return file;
}

void BatchListFile::decodeBody( const RawFile &file)
{
  // set processing start to position after spare
  RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t batchListPtr;
  it = getInt< uint32_t>( it, batchListPtr);

  uint32_t userDefinedDataPtr;
  it = getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = file.begin() + mediaInformationPtr * 2;
  it = getString( it, mediaSetPn);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumber);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembers);

  // batch list
  it = file.begin() + 2 * batchListPtr;
  batchInfoList = BatchInfo::getBatchInfos( it);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 2);
  }

  // file crc decoded and checked within base class
}

}
}

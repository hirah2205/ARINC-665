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
 * @brief Definition of class Arinc665::Files::BatchListFile.
 **/

#include "BatchListFile.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

BatchListFile::BatchListFile( void):
  mediaSequenceNumber( 0),
  numberOfMediaSetMembers( 0)
{
}

BatchListFile::BatchListFile( const RawFile &file):
  ListFile( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2)
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
  batchList = BatchInfo::getBatchList( it);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = file.begin() + userDefinedDataPtr * 2;
    userDefinedData.assign( it, file.end() - 2);
  }

  // file crc decoded and checked within base class
}

Arinc665Version BatchListFile::getArincVersion( void) const
{
  return Arinc665Version::ARINC_665_2;
}

BatchListFile::string BatchListFile::getMediaSetPn( void) const
{
  return mediaSetPn;
}

void BatchListFile::setMediaSetPn( const string &mediaSetPn)
{
  this->mediaSetPn = mediaSetPn;
}

uint8_t BatchListFile::getMediaSequenceNumber( void) const
{
  return mediaSequenceNumber;
}

void BatchListFile::setMediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  this->mediaSequenceNumber = mediaSequenceNumber;
}

uint8_t BatchListFile::getNumberOfMediaSetMembers( void) const
{
  return numberOfMediaSetMembers;
}

void BatchListFile::setNumberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
}

unsigned int BatchListFile::getNumberOfBatches( void) const
{
  return batchList.size();
}

const BatchListFile::BatchList& BatchListFile::getBatches( void) const
{
  return batchList;
}

BatchListFile::BatchList& BatchListFile::getBatches( void)
{
  return batchList;
}

BatchListFile::BatchMap BatchListFile::getBatchesMap( void) const
{
  BatchMap batches;

  for (const auto & batch : batchList)
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

const BatchListFile::UserDefinedData& BatchListFile::getUserDefinedData( void) const
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
    (batchList == other.getBatches());
}

}
}

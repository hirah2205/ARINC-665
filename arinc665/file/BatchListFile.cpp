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
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

BatchListFile::BatchListFile( Arinc665Version version):
  ListFile( FileType::BatchList, version),
  mediaSequenceNumberValue( 0),
  numberOfMediaSetMembersValue( 0)
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

std::string BatchListFile::mediaSetPn() const
{
  return mediaSetPnValue;
}

void BatchListFile::mediaSetPn( const std::string &mediaSetPn)
{
  mediaSetPnValue = mediaSetPn;
}

void BatchListFile::mediaSetPn( std::string &&mediaSetPn)
{
  mediaSetPnValue = std::move( mediaSetPn);
}

uint8_t BatchListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberValue;
}

void BatchListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber)
{
  mediaSequenceNumberValue = mediaSequenceNumber;
}

uint8_t BatchListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersValue;
}

void BatchListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  numberOfMediaSetMembersValue = numberOfMediaSetMembers;
}

size_t BatchListFile::numberOfBatches() const
{
  return batchesInfoValue.size();
}

const BatchesInfo& BatchListFile::batchesInfo() const
{
  return batchesInfoValue;
}

BatchesInfo& BatchListFile::batchesInfo()
{
  return batchesInfoValue;
}

BatchListFile::BatchInfoMap BatchListFile::batchesInfoAsMap() const
{
  BatchInfoMap batches;

  for (const auto & batch : batchesInfoValue)
  {
    batches.insert(
      std::make_pair(
        std::make_pair(
          batch.memberSequenceNumber(),
          batch.filename()),
        batch));
  }

  return batches;
}

void BatchListFile::batchInfo( const BatchInfo &batchInfo)
{
  batchesInfoValue.push_back( batchInfo);
}

void BatchListFile::batchInfo( BatchInfo &&batchInfo)
{
  batchesInfoValue.push_back( batchInfo);
}

const BatchListFile::UserDefinedData& BatchListFile::userDefinedData() const
{
  return userDefinedDataValue;
}

void BatchListFile::userDefinedData( const UserDefinedData &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

void BatchListFile::userDefinedData( UserDefinedData &&userDefinedData)
{
  userDefinedDataValue = std::move( userDefinedData);
}

bool BatchListFile::belongsToSameMediaSet( const BatchListFile &other) const
{
  return
    (mediaSetPnValue == other.mediaSetPn()) &&
    (numberOfMediaSetMembersValue == other.numberOfMediaSetMembers()) &&
    (batchesInfoValue == other.batchesInfo());
}

RawFile BatchListFile::encode() const
{
  RawFile rawFile(
    BaseHeaderOffset +
    3 * sizeof( uint32_t)); // mediaInformationPtr, BatchesListPtr, userDefinedDataPtr

  const auto rawMediaSetPn{ encodeString( mediaSetPn())};
  assert( rawMediaSetPn.size() % 2 == 0);

  const auto rawBatchesInfo{ encodeBatchesInfo()};
  assert( rawBatchesInfo.size() % 2 == 0);

  auto it{ rawFile.begin() + BaseHeaderOffset};

  // media information pointer
  uint32_t mediaInformationPtr =
    (BaseHeaderOffset + (3 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, mediaInformationPtr);

  // batches list pointer
  uint32_t batchesListPtr =
    mediaInformationPtr + 
    (2 * sizeof( uint8_t)) / 2 + 
    safeCast< uint32_t>( rawMediaSetPn.size() / 2);
  it = setInt< uint32_t>( it, batchesListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedDataValue.empty() ? 
      0U :
      batchesListPtr + safeCast< uint32_t>( rawBatchesInfo.size() / 2);
  it = setInt< uint32_t>( it, userDefinedDataPtr);


  // media set part number
  it = rawFile.insert( it, rawMediaSetPn.begin(), rawMediaSetPn.end());

  // media sequence number, number of media set members
  auto oldSize{ rawFile.size()};
  rawFile.resize( oldSize + 2 * sizeof( uint8_t) );
  it = rawFile.begin() + oldSize;

  // media sequence number
  it = setInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  it = setInt< uint8_t>( it, numberOfMediaSetMembersValue);


  // batches list
  it = rawFile.insert( it, rawBatchesInfo.begin(), rawBatchesInfo.end());
  it += rawFile.size();

  // user defined data
  if (!userDefinedDataValue.empty())
  {
    //! TODO change to normal exception
    assert( userDefinedDataValue.size() % 2 == 0);
    rawFile.insert(
      rawFile.end(),
      userDefinedDataValue.begin(),
      userDefinedDataValue.end());
  }

  // resize buffer for crc
  rawFile.resize( rawFile.size() + sizeof( uint16_t));

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void BatchListFile::decodeBody( const RawFile &rawFile)
{
  // set processing start to position after spare
  auto it{ rawFile.begin() + BaseHeaderOffset};

  uint32_t mediaInformationPtr;
  it = getInt< uint32_t>( it, mediaInformationPtr);

  uint32_t batchListPtr;
  it = getInt< uint32_t>( it, batchListPtr);

  uint32_t userDefinedDataPtr;
  getInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.begin() + mediaInformationPtr * 2;
  it = decodeString( it, mediaSetPnValue);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  getInt< uint8_t>( it, numberOfMediaSetMembersValue);

  // batch list
  decodeBatchesInfo( rawFile, 2 * batchListPtr);

  // user defined data
  if ( 0 != userDefinedDataPtr)
  {
    it = rawFile.begin() + userDefinedDataPtr * 2;
    userDefinedDataValue.assign( it, rawFile.end() - 2);
  }

  // file CRC decoded and checked within base class
}

RawFile BatchListFile::encodeBatchesInfo() const
{
  RawFile rawBatchesInfo( sizeof( uint16_t));

  // number of batches
  setInt< uint16_t>( rawBatchesInfo.begin(), numberOfBatches());

  // iterate over batches
  uint16_t batchCounter( 0);
  for (auto const &batchInfo : batchesInfoValue)
  {
    ++batchCounter;

    RawFile rawBatchInfo( sizeof( uint16_t));

    auto const rawPartNumber{ encodeString( batchInfo.partNumber())};
    assert( rawPartNumber.size() % 2 == 0);

    auto const rawFilename{ encodeString( batchInfo.filename())};
    assert( rawFilename.size() % 2 == 0);

    // next pointer
    setInt< uint16_t>(
      rawBatchInfo.begin(),
      (batchCounter == numberOfBatches()) ?
      (0U) :
      ((sizeof( uint16_t)
        + rawPartNumber.size()
        + rawFilename.size()
        + sizeof( uint16_t)) / 2));

    // Part Number
    rawBatchInfo.insert(
      rawBatchInfo.end(),
      rawPartNumber.begin(),
      rawPartNumber.end());

    // Batch Filename
    rawBatchInfo.insert(
      rawBatchInfo.end(),
      rawFilename.begin(),
      rawFilename.end());

    // member sequence number
    auto oldSize{ rawBatchInfo.size()};
    rawBatchInfo.resize( oldSize + sizeof( uint16_t) );

    setInt< uint16_t>(
      rawBatchInfo.begin() + oldSize,
      batchInfo.memberSequenceNumber());

    // add file info to files info
    rawBatchesInfo.insert(
      rawBatchesInfo.end(),
      rawBatchInfo.begin(),
      rawBatchInfo.end());
  }

  return rawBatchesInfo;
}

void BatchListFile::decodeBatchesInfo(
  const RawFile &rawFile,
  const std::size_t offset)
{
  auto it( rawFile.begin() + offset);

  // clear eventually stored infos
  batchesInfoValue.clear();

  // number of batches
  uint16_t numberOfBatches;
  it = getInt< uint16_t>( it, numberOfBatches);

  // iterate over batch indexes
  for ( unsigned int batchIndex = 0; batchIndex < numberOfBatches; ++batchIndex)
  {
    auto listIt( it);

    // next batch pointer
    uint16_t batchPointer;
    listIt = getInt< uint16_t>( listIt, batchPointer);

    //! @todo check pointer for != 0 (all except last ==> OK, last ==> error)

    // part number
    std::string partNumber;
    listIt = Arinc665File::decodeString( listIt, partNumber);

    // batch filename
    std::string filename;
    listIt = Arinc665File::decodeString( listIt, filename);

    // member sequence number
    uint16_t memberSequenceNumber;
    listIt = getInt< uint16_t>( listIt, memberSequenceNumber);

    // set it to begin of next batch
    it += batchPointer * 2;

    batchesInfoValue.emplace_back( partNumber, filename, memberSequenceNumber);
  }
}

}

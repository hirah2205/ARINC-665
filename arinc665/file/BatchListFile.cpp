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

BatchListFile::string BatchListFile::mediaSetPn() const
{
  return mediaSetPnValue;
}

void BatchListFile::mediaSetPn( const string &mediaSetPn)
{
  mediaSetPnValue = mediaSetPn;
}

void BatchListFile::mediaSetPn( string &&mediaSetPn)
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
          batch.getMemberSequenceNumber(),
          batch.getFilename()),
        batch));
  }

  return batches;
}

void BatchListFile::addBatchInfo( const BatchInfo &batchInfo)
{
  batchesInfoValue.push_back( batchInfo);
}

void BatchListFile::addBatchInfo( BatchInfo &&batchInfo)
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
    3 * sizeof( uint32_t) + // mediaInformationPtr, BatchesListPtr, userDefinedDataPtr
    2 * sizeof( uint8_t)  + // media sequence number, number of media set members
    sizeof( uint16_t));     // crc

  auto rawMediaSetPn( encodeString( mediaSetPn()));
  assert( rawMediaSetPn.size() % 2 == 0);
  auto rawBatchesInfo( encodeBatchesInfo());
  assert( rawBatchesInfo.size() % 2 == 0);

  auto it( rawFile.begin() + BaseHeaderOffset);

  // media information pointer
  uint32_t mediaInformationPtr =
    (BaseHeaderOffset + (3 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, mediaInformationPtr);

  // batches list pointer
  uint32_t batchesListPtr =
    mediaInformationPtr + (2 * sizeof( uint8_t)) / 2 + rawMediaSetPn.size() / 2;
  it = setInt< uint32_t>( it, batchesListPtr);

  // user defined data pointer
  uint32_t userDefinedDataPtr =
    userDefinedDataValue.empty() ? 0 : batchesListPtr + rawBatchesInfo.size() / 2;
  it = setInt< uint32_t>( it, userDefinedDataPtr);

  // media set part number
  it = rawFile.insert( it, rawMediaSetPn.begin(), rawMediaSetPn.end());
  it += rawMediaSetPn.size();

  // media sequence number
  it = setInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  it = setInt< uint8_t>( it, numberOfMediaSetMembersValue);

  // loads list
  it = rawFile.insert( it, rawBatchesInfo.begin(), rawBatchesInfo.end());
  it += rawFile.size();

  if (!userDefinedDataValue.empty())
  {
    assert( userDefinedDataValue.size() % 2 == 0);
    rawFile.insert(
      it,
      userDefinedDataValue.begin(),
      userDefinedDataValue.end());
  }

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
  it = decodeString( it, mediaSetPnValue);

  // media sequence number
  it = getInt< uint8_t>( it, mediaSequenceNumberValue);

  // number of media set members
  it = getInt< uint8_t>( it, numberOfMediaSetMembersValue);

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
    auto const rawPartNumber( encodeString( batchInfo.getPartNumber()));
    assert( rawPartNumber.size() % 2 == 0);
    auto const rawFilename( encodeString( batchInfo.getFilename()));
    assert( rawFilename.size() % 2 == 0);

    RawFile rawBatchInfo(
      sizeof( uint16_t) + // next pointer
      rawPartNumber.size() +
      rawFilename.size() +
      sizeof( uint16_t)); // member sequence number

    auto batchInfoIt( rawBatchInfo.begin());

    // next batch pointer (is set to 0 for last file)
    batchInfoIt = setInt< uint16_t>(
      batchInfoIt,
      (batchCounter == numberOfBatches()) ?
        (0U) :
        (rawBatchInfo.size() / 2));

    // part number
    batchInfoIt =
      std::copy( rawPartNumber.begin(), rawPartNumber.end(), batchInfoIt);

    // filename
    batchInfoIt =
      std::copy( rawFilename.begin(), rawFilename.end(), batchInfoIt);

    // member sequence number
    batchInfoIt =
      setInt< uint16_t>( batchInfoIt, batchInfo.getMemberSequenceNumber());

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
    string partNumber;
    listIt = Arinc665File::decodeString( listIt, partNumber);

    // batch filename
    string filename;
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
}

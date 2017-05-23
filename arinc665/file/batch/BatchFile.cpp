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

BatchFile::BatchFile( const Arinc665Version version):
    Arinc665File( FileType::BatchFile, version)
{
}

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

void BatchFile::addTargetHardware( const BatchTargetInfo &targetHardwareInfo)
{
  targetHardwares.push_back( targetHardwareInfo);
}

void BatchFile::addTargetHardware( BatchTargetInfo &&targetHardwareInfo)
{
  targetHardwares.push_back( targetHardwareInfo);
}

RawFile BatchFile::encode() const
{
  RawFile rawFile(
    BaseHeaderOffset +
    2 * sizeof( uint32_t) + // batchPartNumberPtr, targetHardwareIdListPtr
    sizeof( uint16_t));     // CRC

  auto rawBatchPn( encodeString( getPartNumber()));
  assert( rawBatchPn.size() % 2 == 0);

  auto rawComment( encodeString( getComment()));
  assert( rawComment.size() % 2 == 0);

  auto rawThwIdsList( encodeBatchTargetsInfo());
  assert( rawThwIdsList.size() % 2 == 0);


  auto it( rawFile.begin() + BaseHeaderOffset);

  // batch part number information pointer
  const uint32_t batchPartNumberPtr=
    (BaseHeaderOffset + (2 * sizeof( uint32_t))) / 2;
  it = setInt< uint32_t>( it, batchPartNumberPtr);

  // THW ID list pointer
  const uint32_t targetHardwareIdListPtr =
    batchPartNumberPtr +
    (rawBatchPn.size() / 2) +
    (rawComment.size() / 2);
  it = setInt< uint32_t>( it, targetHardwareIdListPtr);


  // batch part number
  it = rawFile.insert( it, rawBatchPn.begin(), rawBatchPn.end());
  it += rawBatchPn.size();

  // comment
  it = rawFile.insert( it, rawComment.begin(), rawComment.end());
  it += rawComment.size();

  // THW ID load list
  it = rawFile.insert( it, rawThwIdsList.begin(), rawThwIdsList.end());
  it += rawThwIdsList.size();


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

  // target hardware ID load list
  decodeBatchTargetsInfo( rawFile, targetHardwareIdListPtr * 2);
}

RawFile BatchFile::encodeBatchTargetsInfo() const
{
  RawFile rawBatchTargetsInfo( sizeof(uint16_t)); // Number of THW IDs

  // number of THW IDs
  setInt< uint16_t>( rawBatchTargetsInfo.begin(), targetHardwares.size());

  // iterate over target HWs
  uint16_t thwCounter( 0);
  for (auto const &targetHardwareInfo : targetHardwares)
  {
    ++thwCounter;

    auto const rawThwId( encodeString( targetHardwareInfo.getTargetHardwareId()));
    assert( rawThwId.size() % 2 == 0);

    RawFile rawLoadsInfo;
    /* iterate over loads */
    for ( auto const loadInfo : targetHardwareInfo.getLoads())
    {
      auto const rawHeaderFilename( encodeString( loadInfo.getHeaderFilename()));
      assert( rawHeaderFilename.size() % 2 == 0);

      auto const rawPartNumber( encodeString( loadInfo.getPartNumber()));
      assert( rawPartNumber.size() % 2 == 0);

      rawLoadsInfo.insert( rawLoadsInfo.end(), rawHeaderFilename.begin(), rawHeaderFilename.end());
      rawLoadsInfo.insert( rawLoadsInfo.end(), rawPartNumber.begin(), rawPartNumber.end());
    }
    assert( rawLoadsInfo.size() % 2 == 0);

    RawFile rawBatchTargetInfo(
      sizeof( uint16_t) + // next THW ID pointer
      rawThwId.size() +
      sizeof( uint16_t) + // number of loads
      rawLoadsInfo.size());

    auto batchTargetInfoIt( rawBatchTargetInfo.begin());

    // next load pointer (is set to 0 for last load)
    batchTargetInfoIt = setInt< uint16_t>(
      batchTargetInfoIt,
      (thwCounter == targetHardwares.size()) ?
        (0U) :
        (rawBatchTargetInfo.size() / 2));

    // THW ID
    batchTargetInfoIt =
      std::copy( rawThwId.begin(), rawThwId.end(), batchTargetInfoIt);

    // Number of Loads
    batchTargetInfoIt =
      setInt< uint16_t>( batchTargetInfoIt, targetHardwareInfo.getLoads().size());

    // Loads list
    batchTargetInfoIt =
      std::copy( rawLoadsInfo.begin(), rawLoadsInfo.end(), batchTargetInfoIt);

    // add THW info to files info
    rawBatchTargetsInfo.insert(
      rawBatchTargetsInfo.end(),
      rawBatchTargetInfo.begin(),
      rawBatchTargetInfo.end());
  }

  return rawBatchTargetsInfo;
}

void BatchFile::decodeBatchTargetsInfo(
  const RawFile &rawFile,
  const std::size_t offset)
{
  RawFile::const_iterator it( rawFile.begin() + offset);

  // clear potently data
  targetHardwares.clear();

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
    targetHardwares.emplace_back( std::move( thwId), std::move( batchLoadsInfo));
  }
}

}
}

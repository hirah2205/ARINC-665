/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::BatchFile.
 **/

#include "BatchFile.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>
#include <helper/Logger.hpp>

namespace Arinc665::File {

BatchFile::BatchFile( const SupportedArinc665Version version):
  Arinc665File{ version}
{
}

BatchFile::BatchFile(
  SupportedArinc665Version version,
  std::string_view partNumber,
  std::string_view comment,
  const BatchTargetsInfo &targets):
  Arinc665File{ version},
  partNumberValue{ partNumber},
  commentValue{ comment},
  targetsHardwareV{ targets}
{
}

BatchFile::BatchFile(
  SupportedArinc665Version version,
  std::string &&partNumber,
  std::string &&comment,
  BatchTargetsInfo &&targets):
  Arinc665File{ version},
  partNumberValue{ std::move( partNumber)},
  commentValue{ std::move( comment)},
  targetsHardwareV{ std::move( targets)}
{
}

BatchFile::BatchFile( const ConstRawFileSpan &rawFile):
  Arinc665File{ rawFile, FileType::BatchFile}
{
  decodeBody( rawFile);
}

BatchFile& BatchFile::operator=( const ConstRawFileSpan &rawFile)
{
  // call inherited operator
  Arinc665File::operator =( rawFile);
  decodeBody( rawFile);

  return *this;
}

FileType BatchFile::fileType() const noexcept
{
  return FileType::BatchFile;
}

std::string_view BatchFile::partNumber() const
{
  return partNumberValue;
}

void BatchFile::partNumber( std::string_view partNumber)
{
  partNumberValue = partNumber;
}

void BatchFile::partNumber( std::string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

std::string_view BatchFile::comment() const
{
  return commentValue;
}

void BatchFile::comment( std::string_view comment)
{
  commentValue = comment;
}

void BatchFile::comment( std::string &&comment)
{
  commentValue = std::move( comment);
}

const BatchTargetsInfo& BatchFile::targetsHardware() const
{
  return targetsHardwareV;
}

BatchTargetsInfo& BatchFile::targetsHardware()
{
  return targetsHardwareV;
}

void BatchFile::targetHardware( const BatchTargetInfo &targetHardwareInfo)
{
  targetsHardwareV.push_back( targetHardwareInfo);
}

void BatchFile::targetHardware( BatchTargetInfo &&targetHardwareInfo)
{
  targetsHardwareV.push_back( targetHardwareInfo);
}

RawFile BatchFile::encode() const
{
  RawFile rawFile( BatchFileHeaderSize);

  // spare field
  Helper::setInt< uint32_t>( rawFile.begin() + SpareFieldOffset, 0U);


  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ BatchFileHeaderSize};


  // batch part number + comment
  auto rawBatchPn{ encodeString( partNumberValue)};
  assert( rawBatchPn.size() % 2 == 0);
  auto rawComment{ encodeString( commentValue)};
  assert( rawComment.size() % 2 == 0);

  Helper::setInt< uint32_t>(
    rawFile.begin() + BatchPartNumberPointerFieldOffset,
    nextFreeOffset / 2);
  nextFreeOffset += rawBatchPn.size() + rawComment.size();

  rawFile.insert( rawFile.end(), rawBatchPn.begin(), rawBatchPn.end());
  rawFile.insert( rawFile.end(), rawComment.begin(), rawComment.end());


  // THW ID load list
  auto rawThwIdsList{ encodeBatchTargetsInfo()};
  assert( rawThwIdsList.size() % 2 == 0);

  Helper::setInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    nextFreeOffset / 2);
  // nextFreeOffset += rawThwIdsList.size();

  rawFile.insert( rawFile.end(), rawThwIdsList.begin(), rawThwIdsList.end());


  // Resize file for file CRC
  rawFile.resize( rawFile.size() + sizeof( uint16_t));

  // set header and crc
  insertHeader( rawFile);

  return rawFile;
}

void BatchFile::decodeBody( const ConstRawFileSpan &rawFile)
{
  // Spare field
  uint32_t spare{};
  Helper::getInt< uint32_t>( rawFile.begin() + SpareFieldOffset, spare);

  if (0U != spare)
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo( "Spare is not 0"));
  }

  uint32_t batchPartNumberPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + BatchPartNumberPointerFieldOffset,
    batchPartNumberPtr);

  uint32_t targetHardwareIdListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + ThwIdsPointerFieldOffset,
    targetHardwareIdListPtr);

  // batch part number
  auto it{ decodeString(
    rawFile.begin() + batchPartNumberPtr * 2,
    partNumberValue)};

  // comment
  decodeString( it, commentValue);

  // target hardware ID load list
  decodeBatchTargetsInfo( rawFile, targetHardwareIdListPtr * 2);
}

RawFile BatchFile::encodeBatchTargetsInfo() const
{
  RawFile rawBatchTargetsInfo( sizeof(uint16_t)); // Number of THW IDs

  // number of THW IDs TODO: Check for maximum number of entries.
  Helper::setInt< uint16_t>(
    rawBatchTargetsInfo.begin(),
    Helper::safeCast< uint16_t>( targetsHardwareV.size()));

  // iterate over target HWs
  uint16_t thwCounter{ 0};
  for ( auto const &targetHardwareInfo : targetsHardwareV)
  {
    ++thwCounter;

    auto const rawThwIdPosition{
      encodeString( targetHardwareInfo.targetHardwareIdPosition())};
    assert( rawThwIdPosition.size() % 2 == 0);

    RawFile rawLoadsInfo{};
    /* iterate over loads */
    for ( auto const &loadInfo : targetHardwareInfo.loads())
    {
      auto const rawHeaderFilename{ encodeString( loadInfo.headerFilename())};
      assert( rawHeaderFilename.size() % 2 == 0);

      auto const rawPartNumber{ encodeString( loadInfo.partNumber())};
      assert( rawPartNumber.size() % 2 == 0);

      rawLoadsInfo.insert(
        rawLoadsInfo.end(),
        rawHeaderFilename.begin(),
        rawHeaderFilename.end());
      rawLoadsInfo.insert(
        rawLoadsInfo.end(),
        rawPartNumber.begin(),
        rawPartNumber.end());
    }
    assert( rawLoadsInfo.size() % 2 == 0);

    RawFile rawBatchTargetInfo(
      sizeof( uint16_t) + // next THW ID pointer
      rawThwIdPosition.size() +
      sizeof( uint16_t) + // number of loads
      rawLoadsInfo.size());

    auto batchTargetInfoIt{ rawBatchTargetInfo.begin()};

    // next load pointer (is set to 0 for last load)
    batchTargetInfoIt = Helper::setInt< uint16_t>(
      batchTargetInfoIt,
      (thwCounter == targetsHardwareV.size()) ?
        (0U) :
        Helper::safeCast< uint16_t>( rawBatchTargetInfo.size() / 2));

    // THW ID
    batchTargetInfoIt = std::copy(
      rawThwIdPosition.begin(),
      rawThwIdPosition.end(),
      batchTargetInfoIt);

    // Number of Loads
    batchTargetInfoIt = Helper::setInt< uint16_t>(
      batchTargetInfoIt,
      Helper::safeCast< uint16_t>( targetHardwareInfo.loads().size()));

    // Loads list
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
  const ConstRawFileSpan &rawFile,
  const std::size_t offset)
{
  //BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + offset};

  // clear potentially data
  targetsHardwareV.clear();

  // number of target HW IDs
  uint16_t numberOfTargetHardwareIds{};
  it = Helper::getInt< uint16_t>( it, numberOfTargetHardwareIds);

  // iterate over THW ID index
  for (
    unsigned int thwIdIndex = 0U;
    thwIdIndex < numberOfTargetHardwareIds;
    ++thwIdIndex)
  {
    auto listIt{ it};

    // next THW ID pointer
    uint16_t thwIdPointer{};
    listIt = Helper::getInt< uint16_t>( listIt, thwIdPointer);

    // THW ID
    std::string thwId{};
    listIt = decodeString( listIt, thwId);

    // Loads list
    BatchLoadsInfo batchLoadsInfo{};

    // number of loads
    uint16_t numberOfLoads{};
    listIt = Helper::getInt< uint16_t>( listIt, numberOfLoads);

    // iterate over load index
    for ( unsigned int loadIndex = 0U; loadIndex < numberOfLoads; ++loadIndex)
    {
      // header filename
      std::string filename{};
      listIt = decodeString( listIt, filename);

      // Load PN
      std::string partNumber{};
      listIt = decodeString( listIt, partNumber);

      // Batch Load info
      batchLoadsInfo.emplace_back( std::move( filename), std::move( partNumber));
    }

    // set it to begin of next file
    it += thwIdPointer * 2U;

    // THW ID info
    targetsHardwareV.emplace_back(
      std::move( thwId),
      std::move( batchLoadsInfo));
  }
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Files::BatchListFile.
 **/

#include "BatchListFile.hpp"

#include <arinc665/files/StringUtils.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::Files {

BatchListFile::BatchListFile( const SupportedArinc665Version version) noexcept:
  ListFile{ version }
{
}

BatchListFile::BatchListFile( const ConstRawFileSpan &rawFile ):
  ListFile{ rawFile, FileType::BatchList }
{
  decodeBody( rawFile );
}

BatchListFile& BatchListFile::operator=( const ConstRawFileSpan &rawFile)
{
  Arinc665File::operator =( rawFile );
  decodeBody( rawFile );

  return *this;
}

FileType BatchListFile::fileType() const noexcept
{
  return FileType::BatchList;
}

size_t BatchListFile::numberOfBatches() const
{
  return batchesV.size();
}

const BatchesInfo& BatchListFile::batches() const
{
  return batchesV;
}

BatchesInfo& BatchListFile::batches()
{
  return batchesV;
}

void BatchListFile::batch( const BatchInfo &batch )
{
  batchesV.push_back( batch );
}

void BatchListFile::batch( BatchInfo &&batch )
{
  batchesV.push_back( std::move( batch ) );
}

const BatchListFile::UserDefinedData& BatchListFile::userDefinedData() const
{
  return userDefinedDataV;
}

void BatchListFile::userDefinedData( const UserDefinedData &userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = userDefinedData;

  checkUserDefinedData();
}

void BatchListFile::userDefinedData( UserDefinedData &&userDefinedData )
{
  BOOST_LOG_FUNCTION()

  userDefinedDataV = std::move( userDefinedData );

  checkUserDefinedData();
}

bool BatchListFile::belongsToSameMediaSet( const BatchListFile &other) const
{
  return
    ( mediaSetPn() == other.mediaSetPn() ) &&
    ( numberOfMediaSetMembers() == other.numberOfMediaSetMembers() ) &&
    ( batchesV == other.batches() );
}

RawFile BatchListFile::encode() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawFile( FileHeaderSizeV2 );

  // Spare Field
  Helper::setInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, 0U );

  // Next free Offset (used for optional pointer calculation)
  size_t nextFreeOffset{ rawFile.size() };


  // media set information
  const auto rawMediaInformation{ encodeMediaInformation() };
  assert( rawMediaInformation.size() % 2 == 0 );
  rawFile.insert( rawFile.end(), rawMediaInformation.begin(), rawMediaInformation.end() );

  // Update Pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += static_cast< ptrdiff_t >( rawMediaInformation.size() );


  // Batch Information
  const auto rawBatchesInfo{ encodeBatchesInfo() };
  assert( rawBatchesInfo.size() % 2 == 0 );

  // batches list pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + BatchFilesPointerFieldOffsetV2,
    static_cast< uint32_t >( nextFreeOffset / 2U ) );
  nextFreeOffset += rawBatchesInfo.size();

  rawFile.insert( rawFile.end(), rawBatchesInfo.begin(), rawBatchesInfo.end() );


  // user defined data
  assert( userDefinedDataV.size() % 2 == 0 );
  uint32_t userDefinedDataPtr = 0;

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = static_cast< uint32_t >( nextFreeOffset / 2U );

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end() );
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr );


  // set header
  insertHeader( rawFile );

  // Resize file for file CRC
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );

  // set CRC
  calculateFileCrc( rawFile );

  return rawFile;
}

void BatchListFile::decodeBody( const ConstRawFileSpan &rawFile )
{
  BOOST_LOG_FUNCTION()

  // Spare Field
  uint16_t spare{};
  Helper::getInt< uint16_t>( rawFile.begin() + SpareFieldOffsetV2, spare );

  if ( 0U != spare )
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << Helper::AdditionalInfo{ "Spare is not 0" } );
  }


  // media information pointer
  uint32_t mediaInformationPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    mediaInformationPtr );

  // Batch list pointer
  uint32_t batchListPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + BatchFilesPointerFieldOffsetV2,
    batchListPtr );


  // user defined data pointer
  uint32_t userDefinedDataPtr{};
  Helper::getInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr );

  // decode Media Information
  decodeMediaInformation( rawFile, mediaInformationPtr );

  // batch list
  decodeBatchesInfo( rawFile, 2U * static_cast< size_t >( batchListPtr ) );


  // user defined data
  if ( 0 != userDefinedDataPtr )
  {
    userDefinedDataV.assign(
      rawFile.begin() + static_cast< ptrdiff_t >( userDefinedDataPtr ) * 2,
      rawFile.begin() + static_cast< ptrdiff_t >( rawFile.size() - DefaultChecksumPosition ) );
  }


  // file crc decoded and checked within base class
}

RawFile BatchListFile::encodeBatchesInfo() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawBatchesInfo( sizeof( uint16_t ) );

  // Number of batches must not exceed field
  if ( batchesV.size() > std::numeric_limits< uint16_t>::max() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo{ "More batches than allowed" } );
  }

  // number of batches
  Helper::setInt< uint16_t>(
    rawBatchesInfo.begin(),
    static_cast< uint16_t >( batchesV.size() ) );

  // iterate over batches
  uint16_t batchCounter{ 0 };
  for ( auto const &batchInfo : batchesV )
  {
    ++batchCounter;

    RawFile rawBatchInfo( sizeof( uint16_t ) );

    auto const rawPartNumber{ StringUtils_encodeString( batchInfo.partNumber ) };
    assert( rawPartNumber.size() % 2 == 0 );

    auto const rawFilename{ StringUtils_encodeString( batchInfo.filename ) };
    assert( rawFilename.size() % 2 == 0 );

    // next pointer
    Helper::setInt< uint16_t>(
      rawBatchInfo.begin(),
      (batchCounter == numberOfBatches() ) ?
      0U :
      static_cast< uint16_t >( (sizeof( uint16_t )
        + rawPartNumber.size()
        + rawFilename.size()
        + sizeof( uint16_t ) ) / 2U ) );

    // Part Number
    rawBatchInfo.insert(
      rawBatchInfo.end(),
      rawPartNumber.begin(),
      rawPartNumber.end() );

    // Batch Filename
    rawBatchInfo.insert(
      rawBatchInfo.end(),
      rawFilename.begin(),
      rawFilename.end() );

    // member sequence number
    auto oldSize{ rawBatchInfo.size() };
    rawBatchInfo.resize( oldSize + sizeof( uint16_t ) );

    Helper::setInt< uint16_t>(
      rawBatchInfo.begin() + static_cast< ptrdiff_t >( oldSize ),
      batchInfo.memberSequenceNumber );

    // add file info to files info
    rawBatchesInfo.insert(
      rawBatchesInfo.end(),
      rawBatchInfo.begin(),
      rawBatchInfo.end() );
  }

  return rawBatchesInfo;
}

void BatchListFile::decodeBatchesInfo(
  const ConstRawFileSpan &rawFile,
  const std::size_t offset )
{
  BOOST_LOG_FUNCTION()

  auto it{ rawFile.begin() + static_cast< ptrdiff_t >( offset ) };

  // clear eventually stored infos
  batchesV.clear();

  // number of batches
  uint16_t numberOfBatches;
  it = Helper::getInt< uint16_t>( it, numberOfBatches);

  // iterate over batch indexes
  for ( uint16_t batchIndex = 0; batchIndex < numberOfBatches; ++batchIndex )
  {
    auto listIt{ it};

    // next batch pointer
    uint16_t batchPointer{};
    listIt = Helper::getInt< uint16_t>( listIt, batchPointer );

    // check batch pointer for validity
    if ( batchIndex != numberOfBatches - 1U )
    {
      if ( batchPointer == 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next batch pointer is 0" } );
      }
    }
    else
    {
      if ( batchPointer != 0U )
      {
        BOOST_THROW_EXCEPTION( InvalidArinc665File()
          << Helper::AdditionalInfo{ "next batch pointer is not 0" } );
      }
    }

    // part number
    std::string partNumber{};
    listIt = StringUtils_decodeString( listIt, partNumber );

    // batch filename
    std::string filename{};
    listIt = StringUtils_decodeString( listIt, filename );

    // member sequence number
    uint16_t memberSequenceNumber{};
    Helper::getInt< uint16_t>( listIt, memberSequenceNumber );
    if ( ( memberSequenceNumber < 1U ) || ( memberSequenceNumber > 255U ) )
    {
      BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo( "member sequence number out of range" ) );
    }

    // set it to begin of next batch
    it += static_cast< ptrdiff_t >( batchPointer ) * 2;

    batchesV.emplace_back( BatchInfo{
      std::move( partNumber ),
      std::move( filename ),
      static_cast< uint8_t >( memberSequenceNumber ) } );
  }
}

void BatchListFile::checkUserDefinedData()
{
  BOOST_LOG_FUNCTION()

  if ( userDefinedDataV.size() % 2U != 0U)
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning)
      << "User defined data must be 2-byte aligned. - extending range";

    userDefinedDataV.push_back(0U);
  }
}

}

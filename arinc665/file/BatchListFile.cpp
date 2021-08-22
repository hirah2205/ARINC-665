/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::BatchListFile.
 **/

#include "BatchListFile.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::File {

BatchListFile::BatchListFile( const SupportedArinc665Version version) noexcept:
  ListFile{ version },
  mediaSequenceNumberV{ 0 },
  numberOfMediaSetMembersV{ 0 }
{
}

BatchListFile::BatchListFile(
  const SupportedArinc665Version version,
  std::string_view mediaSetPn,
  const uint8_t mediaSequenceNumber,
  const uint8_t numberOfMediaSetMembers,
  const BatchesInfo &batches,
  const UserDefinedData &userDefinedData ):
  ListFile{ version },
  mediaSetPnV{ mediaSetPn },
  mediaSequenceNumberV{ mediaSequenceNumber },
  numberOfMediaSetMembersV{ numberOfMediaSetMembers },
  batchesV{ batches },
  userDefinedDataV{ userDefinedData }
{
  checkUserDefinedData();
}

BatchListFile::BatchListFile(
  SupportedArinc665Version version,
  std::string &&mediaSetPn,
  uint8_t mediaSequenceNumber,
  uint8_t numberOfMediaSetMembers,
  BatchesInfo &&batches,
  UserDefinedData &&userDefinedData ):
  ListFile{ version },
  mediaSetPnV{ std::move( mediaSetPn)},
  mediaSequenceNumberV{ mediaSequenceNumber},
  numberOfMediaSetMembersV{ numberOfMediaSetMembers },
  batchesV{ std::move( batches ) },
  userDefinedDataV{ std::move( userDefinedData ) }
{
  checkUserDefinedData();
}

BatchListFile::BatchListFile( const ConstRawFileSpan &rawFile):
  ListFile{ rawFile, FileType::BatchList }
{
  decodeBody( rawFile);
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

std::string_view BatchListFile::mediaSetPn() const
{
  return mediaSetPnV;
}

void BatchListFile::mediaSetPn( std::string_view mediaSetPn )
{
  mediaSetPnV = mediaSetPn;
}

void BatchListFile::mediaSetPn( std::string &&mediaSetPn )
{
  mediaSetPnV = std::move( mediaSetPn);
}

uint8_t BatchListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberV;
}

void BatchListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber )
{
  mediaSequenceNumberV = mediaSequenceNumber;
}

uint8_t BatchListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersV;
}

void BatchListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers)
{
  numberOfMediaSetMembersV = numberOfMediaSetMembers;
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
  batchesV.push_back( batch );
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
    ( mediaSetPnV == other.mediaSetPn()) &&
    ( numberOfMediaSetMembersV == other.numberOfMediaSetMembers()) &&
    ( batchesV == other.batches());
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
  const auto rawMediaSetPn{ encodeString( mediaSetPn() ) };
  assert( rawMediaSetPn.size() % 2 == 0U );

  // media set part number
  rawFile.insert( rawFile.end(), rawMediaSetPn.begin(), rawMediaSetPn.end() );

  rawFile.resize( rawFile.size() + 2U * sizeof( uint8_t ) );

  // media sequence number
  Helper::setInt< uint8_t>(
    rawFile.begin() + static_cast< ptrdiff_t >( nextFreeOffset + rawMediaSetPn.size() ),
    mediaSequenceNumberV );

  // number of media set members
  Helper::setInt< uint8_t>(
    rawFile.begin() + static_cast< ptrdiff_t >( nextFreeOffset + rawMediaSetPn.size() + sizeof( uint8_t ) ),
    numberOfMediaSetMembersV );

  Helper::setInt< uint32_t>(
    rawFile.begin() + MediaSetPartNumberPointerFieldOffsetV2,
    nextFreeOffset / 2);
  nextFreeOffset += rawMediaSetPn.size() + 2U * sizeof( uint8_t );


  // Batch Information
  const auto rawBatchesInfo{ encodeBatchesInfo() };
  assert( rawBatchesInfo.size() % 2 == 0 );

  // batches list pointer
  Helper::setInt< uint32_t>(
    rawFile.begin() + BatchFilesPointerFieldOffsetV2,
    nextFreeOffset / 2 );
  nextFreeOffset += rawBatchesInfo.size();

  rawFile.insert( rawFile.end(), rawBatchesInfo.begin(), rawBatchesInfo.end() );


  // user defined data
  assert( userDefinedDataV.size() % 2 == 0 );
  uint32_t userDefinedDataPtr = 0;

  if ( !userDefinedDataV.empty() )
  {
    userDefinedDataPtr = nextFreeOffset / 2;
    // nextFreeOffset += userDefinedDataValue.size();

    rawFile.insert(
      rawFile.end(),
      userDefinedDataV.begin(),
      userDefinedDataV.end() );
  }

  Helper::setInt< uint32_t>(
    rawFile.begin() + UserDefinedDataPointerFieldOffsetV2,
    userDefinedDataPtr );


  // Resize to final size ( File CRC)
  rawFile.resize( rawFile.size() + sizeof( uint16_t ) );

  // set header and crc
  insertHeader( rawFile );

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


  // media set part number
  auto it{ decodeString(
    rawFile.begin() + mediaInformationPtr * 2, mediaSetPnV ) };

  // media sequence number
  it = Helper::getInt< uint8_t>( it, mediaSequenceNumberV );

  // number of media set members
  Helper::getInt< uint8_t>( it, numberOfMediaSetMembersV );


  // batch list
  decodeBatchesInfo( rawFile, 2U * batchListPtr );


  // user defined data
  if ( 0 != userDefinedDataPtr )
  {
    userDefinedDataV.assign(
      rawFile.begin() + userDefinedDataPtr * 2U,
      rawFile.begin() + static_cast< ptrdiff_t >( rawFile.size() - DefaultChecksumPosition ) );
  }


  // file crc decoded and checked within base class
}

RawFile BatchListFile::encodeBatchesInfo() const
{
  BOOST_LOG_FUNCTION()

  RawFile rawBatchesInfo( sizeof( uint16_t ) );

  // number of batches
  Helper::setInt< uint16_t>( rawBatchesInfo.begin(), numberOfBatches() );

  // iterate over batches
  uint16_t batchCounter{ 0 };
  for ( auto const &batchInfo : batchesV )
  {
    ++batchCounter;

    RawFile rawBatchInfo( sizeof( uint16_t ) );

    auto const rawPartNumber{ encodeString( batchInfo.partNumber() ) };
    assert( rawPartNumber.size() % 2 == 0 );

    auto const rawFilename{ encodeString( batchInfo.filename() ) };
    assert( rawFilename.size() % 2 == 0 );

    // next pointer
    Helper::setInt< uint16_t>(
      rawBatchInfo.begin(),
      (batchCounter == numberOfBatches() ) ?
      (0U) :
      ( (sizeof( uint16_t )
        + rawPartNumber.size()
        + rawFilename.size()
        + sizeof( uint16_t ) ) / 2 ) );

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
      batchInfo.memberSequenceNumber() );

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
  for ( unsigned int batchIndex = 0; batchIndex < numberOfBatches; ++batchIndex)
  {
    auto listIt{ it};

    // next batch pointer
    uint16_t batchPointer{};
    listIt = Helper::getInt< uint16_t>( listIt, batchPointer);

    //! @todo check pointer for != 0 (all except last ==> OK, last ==> error)

    // part number
    std::string partNumber{};
    listIt = Arinc665File::decodeString( listIt, partNumber);

    // batch filename
    std::string filename{};
    listIt = Arinc665File::decodeString( listIt, filename);

    // member sequence number
    uint16_t memberSequenceNumber{};
    Helper::getInt< uint16_t>( listIt, memberSequenceNumber);
    if ( ( memberSequenceNumber < 1U ) || ( memberSequenceNumber > 255U ) )
    {
      BOOST_THROW_EXCEPTION( InvalidArinc665File()
      << Helper::AdditionalInfo( "member sequence number out of range" ) );
    }

    // set it to begin of next batch
    it += batchPointer * 2U;

    batchesV.emplace_back( partNumber, filename, memberSequenceNumber );
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

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Files::ListFile.
 **/

#include "ListFile.hpp"

#include <arinc665/files/StringUtils.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>

namespace Arinc665::Files {

std::string_view ListFile::mediaSetPn() const
{
  return mediaSetPnV;
}

void ListFile::mediaSetPn( std::string mediaSetPn )
{
  mediaSetPnV = std::move( mediaSetPn );
}


uint8_t ListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberV;
}

void ListFile::mediaSequenceNumber( const uint8_t mediaSequenceNumber )
{
  mediaSequenceNumberV = mediaSequenceNumber;
}

uint8_t ListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersV;
}

void ListFile::numberOfMediaSetMembers(
  const uint8_t numberOfMediaSetMembers )
{
  numberOfMediaSetMembersV = numberOfMediaSetMembers;
}

ListFile::ListFile(
  SupportedArinc665Version version,
  ptrdiff_t checksumPosition ) noexcept:
  Arinc665File{ version, checksumPosition }
{
}

ListFile::ListFile(
  const ConstRawFileSpan &rawFile,
  FileType expectedFileType,
  ptrdiff_t checksumPosition ):
  Arinc665File{ rawFile, expectedFileType, checksumPosition }
{
}

RawFile ListFile::encodeMediaInformation() const
{
  // media set part number
  auto rawMediaInformation{ StringUtils_encodeString( mediaSetPn() ) };
  assert( rawMediaInformation.size() % 2 == 0U );

  const auto partNumberSize{
    static_cast< ptrdiff_t >( rawMediaInformation.size() ) };

  rawMediaInformation.resize( rawMediaInformation.size() + 2 * sizeof( uint8_t ) );

  // media sequence number
  Helper::setInt< uint8_t>(
    rawMediaInformation.begin() + partNumberSize,
    mediaSequenceNumberV );

  // number of media set members
  Helper::setInt< uint8_t>(
    rawMediaInformation.begin() + partNumberSize + sizeof( uint8_t ),
    numberOfMediaSetMembers() );

  return rawMediaInformation;
}

void ListFile::decodeMediaInformation(
  const ConstRawFileSpan &rawFile,
  uint32_t mediaInformationPtr )
{
  if ( static_cast< size_t >( mediaInformationPtr ) * 2U >= rawFile.size() )
  {
    BOOST_THROW_EXCEPTION( InvalidArinc665File{}
      << Helper::AdditionalInfo{ "Media Information Pointer Exceeds File" } );

    //! @todo also check (minimum) processing size for check
  }

  // media set part number
  auto it{ StringUtils_decodeString(
    rawFile.begin()
      + static_cast< ConstRawFileSpan::difference_type>( mediaInformationPtr ) * 2,
    mediaSetPnV ) };

  // media sequence number
  it = Helper::getInt< uint8_t>( it, mediaSequenceNumberV );

  // number of media set members
  Helper::getInt< uint8_t>( it, numberOfMediaSetMembersV );
}

}

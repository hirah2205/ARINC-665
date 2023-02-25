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
#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Endianness.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Files {

std::string_view ListFile::mediaSetPn() const
{
  return mediaSetPnV;
}

void ListFile::mediaSetPn( std::string mediaSetPn )
{
  mediaSetPnV = std::move( mediaSetPn );
}

MediumNumber ListFile::mediaSequenceNumber() const
{
  return mediaSequenceNumberV;
}

void ListFile::mediaSequenceNumber( const MediumNumber mediaSequenceNumber )
{
  mediaSequenceNumberV = mediaSequenceNumber;
}

MediumNumber ListFile::numberOfMediaSetMembers() const
{
  return numberOfMediaSetMembersV;
}

void ListFile::numberOfMediaSetMembers(
  const MediumNumber numberOfMediaSetMembers )
{
  numberOfMediaSetMembersV = numberOfMediaSetMembers;
}

MediaSetInformation ListFile::mediaSetInformation() const
{
  return {
    .partNumber = mediaSetPnV,
    .mediaSequenceNumber = mediaSequenceNumberV,
    .numberOfMediaSetMembers = numberOfMediaSetMembersV };
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
    static_cast< uint8_t >( mediaSequenceNumberV ) );

  // number of media set members
  Helper::setInt< uint8_t>(
    rawMediaInformation.begin() + partNumberSize + sizeof( uint8_t ),
    static_cast< uint8_t >( numberOfMediaSetMembers() ) );

  return rawMediaInformation;
}

void ListFile::decodeMediaInformation(
  const ConstRawFileSpan &rawFile,
  const uint32_t mediaInformationPtr )
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
  uint8_t mediaSequenceNumber{};
  it = Helper::getInt< uint8_t >( it, mediaSequenceNumber );
  mediaSequenceNumberV = mediaSequenceNumber;

  // number of media set members
  uint8_t numberOfMediaSetMembers{};
  Helper::getInt< uint8_t >( it, numberOfMediaSetMembers );
  numberOfMediaSetMembersV = numberOfMediaSetMembers;
}

}

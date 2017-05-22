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
 * @brief Definition of class Arinc665::File::Arinc665File.
 **/

#include "Arinc665File.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Crc.hpp>
#include <arinc665/file/StringHelper.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

uint32_t Arinc665File::getFileLength( const RawFile &file)
{
  // check file size
  if ( file.size() < BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File
    //!   If the file size is to small to represent an valid ARINC 665 file.
    BOOST_THROW_EXCEPTION( InvalidArinc665File()
        << AdditionalInfo( "length of check code string invalid"));
  }

  // decode the file length
  uint32_t fileLength;
  getInt< uint32_t>( file.begin(), fileLength);

  return fileLength;
}

uint16_t Arinc665File::getFormatVersion( const RawFile &file)
{
  // check file size
  if ( file.size() < BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File
    //!   If the file size is to small to represent an valid ARINC 665 file.
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "file content to small"));
  }

  // decode the format version
  uint16_t formatVersion;
  getInt< uint16_t>( file.begin() + 4, formatVersion);

  return formatVersion;
}

uint16_t Arinc665File::calculateChecksum(
  const RawFile &file,
  const std::size_t skipLastBytes)
{
  Arinc665Crc16 arincCrc16;

  arincCrc16.process_block(
    &(*file.begin()),
    &(*file.begin()) + file.size() - skipLastBytes);

  return arincCrc16.checksum();
}

Arinc665File& Arinc665File::operator=( const RawFile &file)
{
  decodeHeader( file, Arinc665FileFormatVersion::MEDIA_FILE_VERSION_2);
  return *this;
}

Arinc665File::operator RawFile() const
{
  return encode();
}

FileType Arinc665File::getFileType() const
{
  return fileType;
}

Arinc665Version Arinc665File::getArincVersion() const
{
  return arinc665Version;
}

void Arinc665File::setArincVersion( Arinc665Version version)
{
  this->arinc665Version = version;
}

uint16_t Arinc665File::getCrc() const noexcept
{
  return crc;
}

void Arinc665File::setCrc( const uint16_t crc) noexcept
{
  this->crc = crc;
}

Arinc665File::Arinc665File(
  const FileType fileType,
  Arinc665Version version,
  const std::size_t checksumPosition) noexcept :
  fileType( fileType),
  checksumPosition( checksumPosition),
  arinc665Version( version),
  crc( 0)
{
}

Arinc665File::Arinc665File(
  FileType fileType,
  const RawFile &file,
  std::size_t checksumPosition) :
  fileType( fileType),
  checksumPosition( checksumPosition)
{
  decodeHeader( file, Arinc665FileFormatVersion::Invalid);
}

Arinc665File& Arinc665File::operator=( const Arinc665File &file)
{
  assert( this->checksumPosition == file.checksumPosition);

  if ( this == &file)
  {
    return *this;
  }

  crc = file.crc;

  return *this;
}

void Arinc665File::insertHeader( RawFile &file) const
{
  // Check file size
  if ( file.size() <= BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File When file is to small
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "File to small"));
  }

  // Check file size
  if ( file.size() % 2 != 0)
  {
    //! @throw InvalidArinc665File When file size is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "Invalid size"));
  }

  auto it( file.begin());

  // file size
  it = setInt< uint32_t>( it, file.size());

  // format version
  it = setInt< uint16_t>( it, static_cast< uint16_t>( 0));

  // spare
  it = setInt< uint16_t>( it, 0U);


  // crc
  setInt< uint16_t>( file.end() - checksumPosition, crc);
}

void Arinc665File::decodeHeader(
  const RawFile &file,
  const Arinc665FileFormatVersion expectedFormatVersion)
{
  // Check file size
  if ( file.size() <= BaseHeaderOffset)
  {
    //! @throw InvalidArinc665File When file is to small
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "File to small"));
  }

  // check size field
  if ( getFileLength( file) * 2 != file.size())
  {
    //! @throw InvalidArinc665File When file size field is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "file size invalid"));
  }

  // check format field
  if (expectedFormatVersion != Arinc665FileFormatVersion::Invalid)
  {
    if ( getFormatVersion( file) != static_cast< uint16_t>(
      expectedFormatVersion))
    {
      //! @throw InvalidArinc665File When file format is wrong
      BOOST_THROW_EXCEPTION(
        InvalidArinc665File() << AdditionalInfo( "wrong file format"));
    }
  }

  // Decode checksum field;
  uint16_t crc;
  getInt< uint16_t>( file.end() - checksumPosition, crc);
  setCrc( crc);

  // calculate checksum and compare against stored
  uint16_t calcCrc = calculateChecksum( file, checksumPosition);
  if ( crc != calcCrc)
  {
    //! @throw InvalidArinc665File When CRC is invalid
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "Invalid Checksum"));
  }
}

}
}

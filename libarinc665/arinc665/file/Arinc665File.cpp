/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665File
 **/

#include "Arinc665File.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/file/StringHelper.hpp>

#include <helper/Endianess.hpp>
#include <helper/Logger.hpp>

#include <boost/crc.hpp>

namespace Arinc665 {
namespace File {

uint32_t Arinc665File::getFileLength( const RawFile &file)
{
  // check file size
  if ( file.size() < 8)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
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
  if ( file.size() < 8)
  {
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
  const unsigned int skipLastBytes)
{
  boost::crc_optimal< 16, Crc16Polynom, Crc16Init, Crc16FinalXor> arincCrc16;

  arincCrc16.process_block(
    &(*file.begin()),
    &(*file.begin()) + file.size() - skipLastBytes);

  return arincCrc16.checksum();
}

uint16_t Arinc665File::getCrc( void) const
{
  return crc;
}

void Arinc665File::setCrc( const uint16_t crc)
{
  this->crc = crc;
}

Arinc665File::Arinc665File( void) :
  crc( 0)
{
}

Arinc665File::Arinc665File(
  const RawFile &file,
  const Arinc665FileFormatVersion expectedFormatVersion,
  const unsigned int checksumPosition)
{
  // Check file size
  if ( file.size() <= BaseHeaderOffset)
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "File to small"));
  }

  // check size field
  if ( getFileLength( file) * 2 != file.size())
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "file size invalid"));
  }

  // check format field
  if ( getFormatVersion( file)
    != static_cast< uint16_t>( expectedFormatVersion))
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "wrong file format"));
  }

  // Decode checksum field;
  uint16_t crc;
  getInt< uint16_t>( file.end() - checksumPosition, crc);
  setCrc( crc);

  // calculate checksum and compare against stored
  uint16_t calcCrc = calculateChecksum( file, checksumPosition);
  if ( crc != calcCrc)
  {
    BOOST_THROW_EXCEPTION(
      InvalidArinc665File() << AdditionalInfo( "Invalid Checksum"));
  }
}

}
}

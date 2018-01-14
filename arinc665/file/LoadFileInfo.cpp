/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::File::LoadFileInfo.
 **/

#include "LoadFileInfo.hpp"

namespace Arinc665 {
namespace File {

LoadFileInfo::LoadFileInfo(
  const string &filename,
  const string &partNumber,
  uint32_t length,
  uint16_t crc):
  filenameValue( filename),
  partNumberValue( partNumber),
  lengthValue( length),
  crcValue( crc)
{
}

LoadFileInfo::LoadFileInfo(
  string &&filename,
  string &&partNumber,
  uint32_t length,
  uint16_t crc):
  filenameValue( std::move( filename)),
  partNumberValue( std::move( partNumber)),
  lengthValue( length),
  crcValue( crc)
{
}

LoadFileInfo::string LoadFileInfo::filename() const
{
  return filenameValue;
}

void LoadFileInfo::filename( const string &filename)
{
  filenameValue = filename;
}

void LoadFileInfo::filename( string &&filename)
{
  filenameValue = std::move( filename);
}

LoadFileInfo::string LoadFileInfo::partNumber() const
{
  return partNumberValue;
}

void LoadFileInfo::partNumber( const string &partNumber)
{
  partNumberValue = partNumber;
}

void LoadFileInfo::partNumber( string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

uint32_t LoadFileInfo::length() const
{
  return lengthValue;
}

void LoadFileInfo::length( const uint32_t length)
{
  lengthValue = length;
}

uint16_t LoadFileInfo::crc() const
{
  return crcValue;
}

void LoadFileInfo::crc( const uint16_t crc)
{
  crcValue = crc;
}

}
}

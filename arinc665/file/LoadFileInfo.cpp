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

namespace Arinc665::File {

LoadFileInfo::LoadFileInfo(
  const std::string &filename,
  const std::string &partNumber,
  const uint64_t length,
  const uint16_t crc,
  const std::optional< CheckValue> &checkValue):
  filenameValue( filename),
  partNumberValue( partNumber),
  lengthValue( length),
  crcValue( crc),
  checkValueValue( checkValue)
{
}

LoadFileInfo::LoadFileInfo(
  std::string &&filename,
  std::string &&partNumber,
  const uint64_t length,
  const uint16_t crc,
  std::optional< CheckValue> &&checkValue):
  filenameValue( std::move( filename)),
  partNumberValue( std::move( partNumber)),
  lengthValue( length),
  crcValue( crc),
  checkValueValue( std::move( checkValue))
{
}

std::string LoadFileInfo::filename() const
{
  return filenameValue;
}

void LoadFileInfo::filename( const std::string &filename)
{
  filenameValue = filename;
}

void LoadFileInfo::filename( std::string &&filename)
{
  filenameValue = std::move( filename);
}

std::string LoadFileInfo::partNumber() const
{
  return partNumberValue;
}

void LoadFileInfo::partNumber( const std::string &partNumber)
{
  partNumberValue = partNumber;
}

void LoadFileInfo::partNumber( std::string &&partNumber)
{
  partNumberValue = std::move( partNumber);
}

uint64_t LoadFileInfo::length() const
{
  return lengthValue;
}

void LoadFileInfo::length( const uint64_t length)
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

const std::optional< CheckValue>& LoadFileInfo::checkValue() const
{
  return checkValueValue;
}

void LoadFileInfo::checkValue( const std::optional< CheckValue> &checkValue)
{
  checkValueValue = checkValue;
}

void LoadFileInfo::checkValue( std::optional< CheckValue> &&checkValue)
{
  checkValueValue = std::move( checkValue);
}

}

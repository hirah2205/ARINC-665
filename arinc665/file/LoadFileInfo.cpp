/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::LoadFileInfo.
 **/

#include "LoadFileInfo.hpp"

namespace Arinc665::File {

LoadFileInfo::LoadFileInfo(
  std::string_view filename,
  std::string_view partNumber,
  const uint64_t length,
  const uint16_t crc,
  const std::optional< CheckValue> &checkValue):
  filenameV{ filename},
  partNumberV{ partNumber}, lengthV{ length}, crcV{ crc}, checkValueV{ checkValue}
{
}

LoadFileInfo::LoadFileInfo(
  std::string &&filename,
  std::string &&partNumber,
  const uint64_t length,
  const uint16_t crc,
  std::optional< CheckValue> &&checkValue):
  filenameV{ std::move( filename) },
  partNumberV{ std::move( partNumber ) },
  lengthV{ length },
  crcV{ crc },
  checkValueV{ std::move( checkValue ) }
{
}

std::string_view LoadFileInfo::filename() const
{
  return filenameV;
}

void LoadFileInfo::filename( std::string_view filename)
{
  filenameV = filename;
}

void LoadFileInfo::filename( std::string &&filename)
{
  filenameV = std::move( filename);
}

std::string_view LoadFileInfo::partNumber() const
{
  return partNumberV;
}

void LoadFileInfo::partNumber( std::string_view partNumber)
{
  partNumberV = partNumber;
}

void LoadFileInfo::partNumber( std::string &&partNumber)
{
  partNumberV = std::move( partNumber);
}

uint64_t LoadFileInfo::length() const
{
  return lengthV;
}

void LoadFileInfo::length( const uint64_t length)
{
  lengthV = length;
}

uint16_t LoadFileInfo::crc() const
{
  return crcV;
}

void LoadFileInfo::crc( const uint16_t crc)
{
  crcV = crc;
}

const std::optional< CheckValue>& LoadFileInfo::checkValue() const
{
  return checkValueV;
}

void LoadFileInfo::checkValue( const std::optional< CheckValue> &checkValue)
{
  checkValueV = checkValue;
}

void LoadFileInfo::checkValue( std::optional< CheckValue> &&checkValue)
{
  checkValueV = std::move( checkValue);
}

}

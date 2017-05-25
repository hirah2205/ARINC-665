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
  filename( filename),
  partNumber( partNumber),
  length( length),
  crc( crc)
{
}

LoadFileInfo::LoadFileInfo(
  string &&filename,
  string &&partNumber,
  uint32_t length,
  uint16_t crc):
  filename( std::move( filename)),
  partNumber( std::move( partNumber)),
  length( length),
  crc( crc)
{
}

LoadFileInfo::string LoadFileInfo::getFilename() const
{
  return filename;
}

void LoadFileInfo::setFilename( const string &filename)
{
  this->filename = filename;
}

LoadFileInfo::string LoadFileInfo::getPartNumber() const
{
  return partNumber;
}

void LoadFileInfo::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

uint32_t LoadFileInfo::getLength() const
{
  return length;
}

void LoadFileInfo::setLength( const uint32_t length)
{
  this->length = length;
}

uint16_t LoadFileInfo::getCrc() const
{
  return crc;
}

void LoadFileInfo::setCrc( const uint16_t crc)
{
  this->crc = crc;
}

}
}

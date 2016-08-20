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
 * @brief Definition of class Arinc665::Media::File.
 **/

#include "File.hpp"

namespace Arinc665 {
namespace Media {

File::File(
  const string &name,
  const uint16_t crc,
  const uint32_t fileLength,
  const string &partNumber) :
  BaseFile( name, partNumber),
  crc( crc),
  fileLength( fileLength)
{
}

File::FileType File::getFileType( void) const
{
  return FileType::RegularFile;
}

uint16_t File::getCrc( void) const
{
  return crc;
}

void File::setCrc( const uint16_t crc)
{
  this->crc = crc;
}

}
}

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

File::File( ContainerEntityPtr parent, const string &name) :
  BaseFile( parent, name),
  fileLength( 0)
{
}

File::FileType File::getFileType( void) const
{
  return FileType::RegularFile;
}

uint32_t File::getFileLength( void) const
{
  return fileLength;
}


void File::setFileLength( const uint32_t fileLength)
{
  this->fileLength = fileLength;
}

}
}

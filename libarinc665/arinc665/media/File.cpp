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

#include <cassert>

namespace Arinc665 {
namespace Media {

File::File( ContainerEntityPtr parent, const string &name) :
  BaseFile( parent, name)
{
  assert( parent);
}

File::FileType File::getFileType( void) const
{
  return FileType::RegularFile;
}

}
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::File.
 **/

#include "File.hpp"

#include <cassert>

namespace Arinc665::Media {

File::File( ContainerEntityPtr parent, std::string_view name) :
  BaseFile( parent, name)
{
}

File::File( ContainerEntityPtr parent, std::string &&name) :
  BaseFile( parent, std::move( name))
{
}

File::FileType File::fileType() const
{
  return FileType::RegularFile;
}

}

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
 * @brief Definition of class Arinc665::Media::Directory.
 **/

#include "Directory.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <cassert>

namespace Arinc665 {
namespace Media {

Directory::Directory( ContainerEntityPtr parent, const string &name):
  ContainerEntity( parent),
  name( name)
{
  assert( parent);
}

ConstMediaSetPtr Directory::getMediaSet( void) const
{
  return getParent()->getMediaSet();
}

MediaSetPtr Directory::getMediaSet( void)
{
  return getParent()->getMediaSet();
}

Directory::Type Directory::getType( void) const
{
  return Type::Directory;
}

const Directory::string& Directory::getName( void) const
{
  return name;
}

const Directory::string& Directory::getPartNumber( void) const
{
  return getParent()->getPartNumber();
}

}
}

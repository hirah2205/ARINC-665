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

ConstMediaSetPtr Directory::getMediaSet() const
{
  auto parentPtr( getParent());

  if ( parentPtr)
  {
    return parentPtr->getMediaSet();
  }

  return {};
}

MediaSetPtr Directory::getMediaSet()
{
  auto parentPtr( getParent());

  if ( parentPtr)
  {
    return parentPtr->getMediaSet();
  }

  return {};
}

Directory::Type Directory::getType() const
{
  return Type::Directory;
}

const Directory::string& Directory::getName() const
{
  return name;
}

Directory::string Directory::getPartNumber() const
{
  auto parentPtr( getParent());

  if ( parentPtr)
  {
    return parentPtr->getPartNumber();
  }

  return {};
}

Directory::path Directory::getPath() const
{
  auto parentPtr( getParent());

  if ( parentPtr)
  {
    return parentPtr->getPath() / name;
  }

  return {};
}

}
}

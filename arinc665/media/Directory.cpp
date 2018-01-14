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
  nameValue( name)
{
  assert( parent);
}

ConstMediaSetPtr Directory::mediaSet() const
{
  auto parentPtr( parent());

  if ( parentPtr)
  {
    return parentPtr->mediaSet();
  }

  return {};
}

MediaSetPtr Directory::mediaSet()
{
  auto parentPtr( parent());

  if ( parentPtr)
  {
    return parentPtr->mediaSet();
  }

  return {};
}

Directory::Type Directory::type() const
{
  return Type::Directory;
}

const Directory::string& Directory::name() const
{
  return nameValue;
}

Directory::string Directory::partNumber() const
{
  auto parentPtr( parent());

  if ( parentPtr)
  {
    return parentPtr->partNumber();
  }

  return {};
}

Directory::fpath Directory::path() const
{
  auto parentPtr( parent());

  if ( parentPtr)
  {
    return parentPtr->path() / nameValue;
  }

  return {};
}

}
}

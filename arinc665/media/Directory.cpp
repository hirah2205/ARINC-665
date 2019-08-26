/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Directory.
 **/

#include "Directory.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <cassert>

namespace Arinc665::Media {

Directory::Directory( ContainerEntityPtr parent, const std::string &name):
  ContainerEntity( parent),
  nameV( name)
{
  assert( parent);
}

ConstMediaSetPtr Directory::mediaSet() const
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

MediaSetPtr Directory::mediaSet()
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

Directory::Type Directory::type() const
{
  return Type::Directory;
}

const std::string& Directory::name() const
{
  return nameV;
}

std::string_view Directory::partNumber() const
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->partNumber();
}

std::filesystem::path Directory::path() const
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->path() / nameV;
}

}

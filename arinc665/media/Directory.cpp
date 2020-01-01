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

Directory::Directory( ContainerEntityPtr parent, std::string_view name):
  nameV{ name},
  parentV{ std::move( parent)}
{
  if ( !parent)
  {
    //! @throw Arinc665::Arinc665Exception when parent is not valid
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "parent must be valid"));
  }
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

std::string_view Directory::name() const
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

ConstContainerEntityPtr Directory::parent() const
{
  return parentV.lock();
}

ContainerEntityPtr Directory::parent()
{
  return parentV.lock();
}

ConstMediumPtr Directory::medium() const
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

MediumPtr Directory::medium()
{
  auto parentPtr( parent());

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

void Directory::parent( const ContainerEntityPtr& parent)
{
  if ( !parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "parent must be valid"));
  }

  if ( shared_from_this() == parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "Recursion not allowed"));
  }

  if ( this->parent() == parent)
  {
    return;
  }

  parentV = parent;
}

}

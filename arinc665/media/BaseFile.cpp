/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::BaseFile.
 **/

#include "BaseFile.hpp"

#include <arinc665/media/ContainerEntity.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665::Media {

ConstMediaSetPtr BaseFile::mediaSet() const
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

MediaSetPtr BaseFile::mediaSet()
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

BaseFile::Type BaseFile::type() const
{
  return Type::File;
}

std::string_view BaseFile::name() const
{
  return nameV;
}

ConstContainerEntityPtr BaseFile::parent() const
{
  return parentV.lock();
}

ContainerEntityPtr BaseFile::parent()
{
  return parentV.lock();
}

ConstMediumPtr BaseFile::medium() const
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

MediumPtr BaseFile::medium()
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

std::filesystem::path BaseFile::path() const
{
  auto parentPtr{ parent()};

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->path() / nameV;
}

BaseFile::BaseFile( const ContainerEntityPtr& parent, std::string_view name ):
  parentV{ parent}, nameV{ name}
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "parent must be valid"));
  }
}

BaseFile::BaseFile( const ContainerEntityPtr& parent, std::string &&name):
  parentV{ parent},
  nameV{ std::move( name) }
{
  if ( !parent )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "parent must be valid" } );
  }
}

void BaseFile::parent( const ContainerEntityPtr &parent )
{
  if ( !parent )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "parent must be valid" } );
  }

  if ( this->parentV.lock() == parent )
  {
    return;
  }

  parentV = parent;
}

}

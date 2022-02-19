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

#include <arinc665/media/ContainerEntity.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665::Media {

ConstMediaSetPtr File::mediaSet() const
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

MediaSetPtr File::mediaSet()
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->mediaSet();
}

File::Type File::type() const
{
  return Type::File;
}

std::string_view File::name() const
{
  return nameV;
}

ConstContainerEntityPtr File::parent() const
{
  return parentV.lock();
}

ContainerEntityPtr File::parent()
{
  return parentV.lock();
}

ConstMediumPtr File::medium() const
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

MediumPtr File::medium()
{
  auto parentPtr{ parent()};

  if (!parentPtr)
  {
    return {};
  }

  return parentPtr->medium();
}

std::filesystem::path File::path() const
{
  auto parentPtr{ parent() };

  if ( !parentPtr)
  {
    return {};
  }

  return parentPtr->path() / nameV;
}

std::optional< Arinc645::CheckValueType > File::checkValueType() const
{
  return checkValueTypeV;
}

void File::checkValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  checkValueTypeV = checkValueType;
}

File::File( const ContainerEntityPtr& parent, std::string_view name ):
  parentV{ parent}, nameV{ name}
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "parent must be valid" } );
  }
}

File::File( const ContainerEntityPtr& parent, std::string &&name):
  parentV{ parent},
  nameV{ std::move( name) }
{
  if ( !parent )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "parent must be valid" } );
  }
}

void File::parent( const ContainerEntityPtr &parent )
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

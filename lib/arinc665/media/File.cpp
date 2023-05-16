// SPDX-License-Identifier: MPL-2.0
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

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/ContainerEntity.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Media {

ConstMediaSetPtr File::mediaSet() const
{
  if ( const auto parentPtr{ parent() }; parentPtr )
  {
    return parentPtr->mediaSet();
  }

  return {};
}

MediaSetPtr File::mediaSet()
{

  if ( const auto parentPtr{ parent() }; parentPtr )
  {
    return parentPtr->mediaSet();
  }

  return {};
}

Type File::type() const
{
  return Type::File;
}

ConstContainerEntityPtr File::parent() const
{
  return parentV.lock();
}

ContainerEntityPtr File::parent()
{
  return parentV.lock();
}

std::filesystem::path File::path() const
{
  if ( const auto parentPtr{ parent() }; parentPtr )
  {
    return parentPtr->path() / nameV;
  }

  return nameV;
}

std::string_view File::name() const
{
  return nameV;
}

void File::rename( std::string name )
{
  if ( const auto parentPtr{ parent() }; parentPtr )
  {
    if ( parentPtr->subdirectory( std::string_view( name ) )
      || parentPtr->file( std::string_view( name ) ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "directory or file with given names exist" }
        << boost::errinfo_file_name{ name } );
    }
  }

  nameV = std::move( name );
}

MediumNumber File::effectiveMediumNumber() const
{
  return mediumNumberV.value_or( parent()->effectiveDefaultMediumNumber() );
}

OptionalMediumNumber File::mediumNumber() const
{
  return mediumNumberV;
}

void File::mediumNumber( const OptionalMediumNumber &mediumNumber )
{
  mediumNumberV = mediumNumber;
}

Arinc645::CheckValueType File::effectiveCheckValueType() const
{
  return checkValueTypeV.value_or( mediaSet()->effectiveFilesCheckValueType() );
}

std::optional< Arinc645::CheckValueType > File::checkValueType() const
{
  return checkValueTypeV;
}

void File::checkValueType( std::optional< Arinc645::CheckValueType > type )
{
  checkValueTypeV = type;
}

File::File(
  const ContainerEntityPtr &parent,
  std::string name,
  const OptionalMediumNumber &mediumNumber ) :
  parentV{  parent },
  nameV{ std::move( name ) },
  mediumNumberV{ mediumNumber }
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

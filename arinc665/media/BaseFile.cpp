/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::BaseFile.
 **/

#include "BaseFile.hpp"

#include <arinc665/media/ContainerEntity.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665 {
namespace Media {

ConstMediaSetPtr BaseFile::getMediaSet() const
{
  auto parent( getParent());

  if (!parent)
  {
    return {};
  }

  return parent->getMediaSet();
}

MediaSetPtr BaseFile::getMediaSet()
{
  auto parent( getParent());

  if (!parent)
  {
    return {};
  }

  return parent->getMediaSet();
}

BaseFile::Type BaseFile::getType() const
{
  return Type::File;
}

const BaseFile::string& BaseFile::getName() const
{
  return name;
}

BaseFile::string BaseFile::getPartNumber() const
{
  return partNumber;
}

void BaseFile::setPartNumber( const string &partNumber)
{
  this->partNumber = partNumber;
}

ContainerEntityPtr BaseFile::getParent()
{
  return parent.lock();
}

ConstContainerEntityPtr BaseFile::getParent() const
{
  return parent.lock();
}

ConstMediumPtr BaseFile::getMedium() const
{
  auto parent( getParent());

  if (!parent)
  {
    return {};
  }

  return parent->getMedium();
}

MediumPtr BaseFile::getMedium()
{
  auto parent( getParent());

  if (!parent)
  {
    return {};
  }

  return parent->getMedium();
}

BaseFile::path BaseFile::getPath() const
{
  auto parentPtr( parent.lock());

  if ( parentPtr)
  {
    return parentPtr->getPath() / name;
  }

  return {};
}

BaseFile::BaseFile( ContainerEntityPtr parent, const string &name) :
  parent( parent),
  name( name)
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }
}

BaseFile::BaseFile( ContainerEntityPtr parent, string &&name):
  parent( parent),
  name( std::move( name))
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }
}

void BaseFile::setParent( ContainerEntityPtr parent)
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }

  if (this->parent.lock() == parent)
  {
    return;
  }

  this->parent = parent;
}

}
}

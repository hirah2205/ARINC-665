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
 * @brief Definition of class Arinc665::Media::BaseFile.
 **/

#include "BaseFile.hpp"

#include <arinc665/media/ContainerEntity.hpp>

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665 {
namespace Media {

ConstMediaSetPtr BaseFile::getMediaSet( void) const
{
  return getParent()->getMediaSet();
}

MediaSetPtr BaseFile::getMediaSet( void)
{
  return getParent()->getMediaSet();
}

BaseFile::Type BaseFile::getType( void) const
{
  return Type::File;
}

const BaseFile::string& BaseFile::getName( void) const
{
  return name;
}

ContainerEntityPtr BaseFile::getParent( void)
{
  return parent.lock();
}

ConstContainerEntityPtr BaseFile::getParent( void) const
{
  return parent.lock();
}

BaseFile::BaseFile( ContainerEntityPtr parent, const string &name) :
  parent( parent),
  name( name)
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
      AdditionalInfo( "parent must be valid"));
  }
}

void BaseFile::setParent( ContainerEntityPtr parent)
{
  if (!parent)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
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

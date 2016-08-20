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

namespace Arinc665 {
namespace Media {

BaseFile::BaseFile(
  const string &name,
  const string &partNumber) :
  PartNumberdEntity( partNumber),
  name( name)
{
}

const BaseFile::string& BaseFile::getName( void) const
{
  return name;
}

}
}

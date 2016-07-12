/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class TargetHardwareId
 **/

#include "TargetHardwareId.hpp"

namespace Arinc665 {

TargetHardwareId::TargetHardwareId( const string& targetHardwareId) :
  targetHardwareId( targetHardwareId)
{
}

const string& TargetHardwareId::get( void) const
{
  return targetHardwareId;
}

void TargetHardwareId::set( const string &targetHardwareId)
{
  this->targetHardwareId = targetHardwareId;
}

}

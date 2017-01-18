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
 * @brief Definition of class Arinc665::TargetHardwareId.
 **/

#include "TargetHardwareId.hpp"

namespace Arinc665 {

TargetHardwareId::TargetHardwareId( const string& targetHardwareId) :
  targetHardwareId( targetHardwareId)
{
}

const TargetHardwareId::string& TargetHardwareId::get() const noexcept
{
  return targetHardwareId;
}

void TargetHardwareId::set( const string &targetHardwareId)
{
  this->targetHardwareId = targetHardwareId;
}

}

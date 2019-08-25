/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::TargetHardwareId.
 **/

#include "TargetHardwareId.hpp"

namespace Arinc665 {

TargetHardwareId::TargetHardwareId( std::string_view targetHardwareId) :
  targetHardwareId{ targetHardwareId}
{
}

TargetHardwareId::TargetHardwareId( std::string &&targetHardwareId) :
  targetHardwareId( std::move( targetHardwareId))
{
}

std::string_view TargetHardwareId::get() const noexcept
{
  return targetHardwareId;
}

void TargetHardwareId::set( std::string_view targetHardwareId)
{
  this->targetHardwareId = targetHardwareId;
}

void TargetHardwareId::set( std::string &&targetHardwareId)
{
  this->targetHardwareId = std::move( targetHardwareId);
}

}

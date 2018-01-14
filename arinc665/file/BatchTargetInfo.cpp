/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::File::BatchTargetInfo.
 **/

#include "BatchTargetInfo.hpp"

namespace Arinc665 {
namespace File {

BatchTargetInfo::BatchTargetInfo(
  const string &targetHardwareId,
  const BatchLoadsInfo &loads):
  targetHardwareIdValue( targetHardwareId),
  loadsValue( loads)
{
}

BatchTargetInfo::BatchTargetInfo(
  string &&targetHardwareId,
  BatchLoadsInfo &&loads):
  targetHardwareIdValue( targetHardwareId),
  loadsValue( loads)
{
}

BatchTargetInfo::string BatchTargetInfo::targetHardwareId() const
{
  return targetHardwareIdValue;
}

void BatchTargetInfo::targetHardwareId( const string &targetHardwareId)
{
  this->targetHardwareIdValue = targetHardwareId;
}

const BatchLoadsInfo& BatchTargetInfo::loads() const
{
  return loadsValue;
}

BatchLoadsInfo& BatchTargetInfo::loads()
{
  return loadsValue;
}

void BatchTargetInfo::loads( const BatchLoadsInfo &loads)
{
  loadsValue = loads;
}

void BatchTargetInfo::loads( BatchLoadsInfo &&loads)
{
  loadsValue = std::move( loads);
}

void BatchTargetInfo::addLoad( const BatchLoadInfo &load)
{
  loadsValue.push_back( load);
}

void BatchTargetInfo::addLoad( BatchLoadInfo &&load)
{
  loadsValue.push_back( load);
}

}
}

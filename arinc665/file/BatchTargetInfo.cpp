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
  targetHardwareIdV( targetHardwareId),
  loadsV( loads)
{
}

BatchTargetInfo::BatchTargetInfo(
  string &&targetHardwareId,
  BatchLoadsInfo &&loads):
  targetHardwareIdV( targetHardwareId),
  loadsV( loads)
{
}

BatchTargetInfo::string BatchTargetInfo::targetHardwareId() const
{
  return targetHardwareIdV;
}

void BatchTargetInfo::targetHardwareId( const string &targetHardwareId)
{
  targetHardwareIdV = targetHardwareId;
}

const BatchLoadsInfo& BatchTargetInfo::loads() const
{
  return loadsV;
}

BatchLoadsInfo& BatchTargetInfo::loads()
{
  return loadsV;
}

void BatchTargetInfo::loads( const BatchLoadsInfo &loads)
{
  loadsV = loads;
}

void BatchTargetInfo::loads( BatchLoadsInfo &&loads)
{
  loadsV = std::move( loads);
}

void BatchTargetInfo::addLoad( const BatchLoadInfo &load)
{
  loadsV.push_back( load);
}

void BatchTargetInfo::addLoad( BatchLoadInfo &&load)
{
  loadsV.push_back( load);
}

}
}

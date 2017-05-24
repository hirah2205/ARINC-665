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
 * @brief Definition of class Arinc665::File::BatchTargetInfo.
 **/

#include "BatchTargetInfo.hpp"

namespace Arinc665 {
namespace File {

BatchTargetInfo::BatchTargetInfo(
  const string &targetHardwareId,
  const BatchLoadsInfo &loads):
  targetHardwareId( targetHardwareId),
  loads( loads)
{
}

BatchTargetInfo::BatchTargetInfo(
  string &&targetHardwareId,
  BatchLoadsInfo &&loads):
  targetHardwareId( targetHardwareId),
  loads( loads)
{
}

BatchTargetInfo::string BatchTargetInfo::getTargetHardwareId() const
{
  return targetHardwareId;
}

void BatchTargetInfo::setTargetHardwareId( const string &targetHardwareId)
{
  this->targetHardwareId = targetHardwareId;
}

const BatchLoadsInfo& BatchTargetInfo::getLoads() const
{
  return loads;
}

BatchLoadsInfo& BatchTargetInfo::getLoads()
{
  return loads;
}

void BatchTargetInfo::setLoads( const BatchLoadsInfo &loads)
{
  this->loads = loads;
}

void BatchTargetInfo::setLoads( BatchLoadsInfo &&loads)
{
  this->loads = loads;
}

void BatchTargetInfo::addLoad( const BatchLoadInfo &load)
{
  loads.push_back( load);
}

void BatchTargetInfo::addLoad( BatchLoadInfo &&load)
{
  loads.push_back( load);
}

}
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::BatchTargetInfo.
 **/

#include "BatchTargetInfo.hpp"

namespace Arinc665::File {

BatchTargetInfo::BatchTargetInfo(
  const std::string &targetHardwareIdPosition,
  const BatchLoadsInfo &loads):
  targetHardwareIdPositionV( targetHardwareIdPosition),
  loadsV( loads)
{
}

BatchTargetInfo::BatchTargetInfo(
  std::string &&targetHardwareIdPosition,
  BatchLoadsInfo &&loads):
  targetHardwareIdPositionV( std::move( targetHardwareIdPosition)),
  loadsV( loads)
{
}

std::string BatchTargetInfo::targetHardwareIdPosition() const
{
  return targetHardwareIdPositionV;
}

void BatchTargetInfo::targetHardwareIdPosition(
  const std::string &targetHardwareIdPosition)
{
  targetHardwareIdPositionV = targetHardwareIdPosition;
}

void BatchTargetInfo::targetHardwareIdPosition(
  std::string &&targetHardwareIdPosition)
{
  targetHardwareIdPositionV = std::move( targetHardwareIdPosition);
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

void BatchTargetInfo::load( const BatchLoadInfo &load)
{
  loadsV.push_back( load);
}

void BatchTargetInfo::load( BatchLoadInfo &&load)
{
  loadsV.push_back( load);
}

}

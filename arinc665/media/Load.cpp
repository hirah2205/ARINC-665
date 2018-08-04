/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::Load.
 **/

#include "Load.hpp"

namespace Arinc665::Media {

Load::Load( ContainerEntityPtr parent, const std::string &name) :
  BaseFile( parent, name)
{
}

Load::Load( ContainerEntityPtr parent, std::string &&name) :
  BaseFile( parent, std::move( name))
{
}

Load::FileType Load::fileType() const
{
  return FileType::LoadFile;
}

const Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions() const
{
  return targetHardwareIdPositionsValue;
}

Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions()
{
  return targetHardwareIdPositionsValue;
}

void Load::targetHardwareIdPositions(
  const TargetHardwareIdPositions &targetHardwareIdPositions)
{
  targetHardwareIdPositionsValue = targetHardwareIdPositions;
}

void Load::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions)
{
  targetHardwareIdPositionsValue = std::move( targetHardwareIdPositions);
}

Load::TargetHardwareIds Load::targetHardwareIds() const
{
  std::list< std::string> thwIds;

  for ( const auto &item : targetHardwareIdPositionsValue)
  {
    thwIds.push_back( item.first);
  }

  return thwIds;
}

void Load::targetHardwareIds( const TargetHardwareIds &thwIds)
{
  for ( const auto &targetHardwareId : thwIds)
  {
    targetHardwareIdPositionsValue.insert(
      std::make_pair( targetHardwareId, Positions{}));
  }
}

const Load::Files& Load::dataFiles() const
{
  return dataFilesValue;
}

void Load::dataFile( const WeakFilePtr dataFile)
{
  dataFilesValue.push_back( dataFile);
}

const Load::Files& Load::supportFiles() const
{
  return supportFilesValue;
}

void Load::supportFile( const WeakFilePtr supportFile)
{
  supportFilesValue.push_back( supportFile);
}

const std::vector< uint8_t>& Load::userDefinedData() const
{
  return userDefinedDataValue;
}

void Load::userDefinedData( const std::vector< uint8_t> &userDefinedData)
{
  userDefinedDataValue = userDefinedData;
}

void Load::userDefinedData( Load::UserDefinedData &&userDefinedData)
{
  userDefinedDataValue= std::move( userDefinedData);
}

const Load::Type& Load::loadType() const
{
  return typeValue;
}

void Load::loadType( const Type &type)
{
  typeValue = type;
}

void Load::loadType( Type &&type)
{
  typeValue = std::move( type);
}

}

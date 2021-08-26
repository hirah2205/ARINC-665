/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Load.
 **/

#include "Load.hpp"

#include <utility>

namespace Arinc665::Media {

Load::Load( ContainerEntityPtr parent, std::string_view name) :
  BaseFile{ std::move( parent ), name }
{
}

Load::Load( ContainerEntityPtr parent, std::string &&name) :
  BaseFile{ std::move( parent ), std::move( name ) }
{
}

Load::FileType Load::fileType() const
{
  return FileType::LoadFile;
}

std::string_view Load::partNumber() const
{
  return partNumberV;
}

void Load::partNumber( std::string_view partNumber )
{
  partNumberV = partNumber;
}

const Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions() const
{
  return targetHardwareIdPositionsV;
}

Load::TargetHardwareIdPositions& Load::targetHardwareIdPositions()
{
  return targetHardwareIdPositionsV;
}

void Load::targetHardwareIdPositions(
  const TargetHardwareIdPositions &targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = targetHardwareIdPositions;
}

void Load::targetHardwareIdPositions(
  TargetHardwareIdPositions &&targetHardwareIdPositions)
{
  targetHardwareIdPositionsV = std::move( targetHardwareIdPositions);
}

Load::TargetHardwareIds Load::targetHardwareIds() const
{
  std::list< std::string> thwIds;

  for ( const auto &item : targetHardwareIdPositionsV )
  {
    thwIds.push_back( item.first);
  }

  return thwIds;
}

void Load::targetHardwareIds( const TargetHardwareIds &thwIds )
{
  for ( const auto &targetHardwareId : thwIds )
  {
    targetHardwareIdPositionsV.emplace_back(
      std::make_pair( targetHardwareId, Positions{} ) );
  }
}

void Load::targetHardwareId(
  std::string_view targetHardwareId,
  const Positions &positions )
{
  targetHardwareIdPositionsV.emplace_back(
    std::make_pair( targetHardwareId, positions ) );
}

void Load::targetHardwareId(
  std::string &&targetHardwareId,
  Positions &&positions )
{
  targetHardwareIdPositionsV.emplace_back(
    std::make_pair( std::move( targetHardwareId ) , std::move( positions ) ) );
}

const Load::Files& Load::dataFiles() const
{
  return dataFilesV;
}

void Load::dataFile( const Files::value_type &file )
{
  dataFilesV.push_back( file );
}

void Load::dataFile( Files::value_type &&file )
{
  dataFilesV.push_back( std::move( file ) );
}

const Load::Files& Load::supportFiles() const
{
  return supportFilesV;
}

void Load::supportFile( const Files::value_type &file )
{
  supportFilesV.push_back( file );
}

void Load::supportFile( Files::value_type &&file )
{
  supportFilesV.push_back( std::move( file ) );
}

const Load::UserDefinedData& Load::userDefinedData() const
{
  return userDefinedDataV;
}

Load::UserDefinedData& Load::userDefinedData()
{
  return userDefinedDataV;
}

void Load::userDefinedData( const std::vector< uint8_t> &userDefinedData)
{
  userDefinedDataV = userDefinedData;
}

void Load::userDefinedData( Load::UserDefinedData &&userDefinedData)
{
  userDefinedDataV = std::move( userDefinedData);
}

const Load::Type& Load::loadType() const
{
  return typeV;
}

void Load::loadType( const Type &type)
{
  typeV = type;
}

void Load::loadType( Type &&type)
{
  typeV = std::move( type);
}

}

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

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <utility>

namespace Arinc665::Media {

Load::Load( const ContainerEntityPtr &parent, std::string name ) :
  File{ parent, std::move( name ) }
{
}

FileType Load::fileType() const
{
  return FileType::LoadFile;
}

uint16_t Load::partFlags() const
{
  return partFlagsV;
}

void Load::partFlags( const uint16_t partFlags )
{
  partFlagsV = partFlags;
}

std::string_view Load::partNumber() const
{
  return partNumberV;
}

void Load::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
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
  TargetHardwareIds thwIds{};

  for ( const auto &[ thwId, positions ] : targetHardwareIdPositionsV )
  {
    thwIds.emplace( thwId );
  }

  return thwIds;
}

void Load::targetHardwareIds( const TargetHardwareIds &thwIds )
{
  for ( const auto &targetHardwareId : thwIds )
  {
    targetHardwareIdPositionsV.insert_or_assign( targetHardwareId, Positions{} );
  }
}

void Load::targetHardwareId(
  std::string_view targetHardwareId,
  const Positions &positions )
{
  // as long there is no override with key like std::map::find we have to
  // manually cast to key_type
  targetHardwareIdPositionsV.insert_or_assign(
    TargetHardwareIdPositions::key_type{ targetHardwareId },
    positions );
}

void Load::targetHardwareId(
  std::string &&targetHardwareId,
  Positions &&positions )
{
  targetHardwareIdPositionsV.insert_or_assign(
    std::move( targetHardwareId ),
    std::move( positions ) );
}

ConstLoadFiles Load::dataFiles( const bool effective ) const
{
  ConstLoadFiles files{};

  for ( const auto &[ filePtr, partNumber, checkValueType  ] : dataFilesV )
  {
    if ( effective )
    {
      files.emplace_back(
        filePtr.lock(),
        partNumber,
        checkValueType.value_or( effectiveDataFilesCheckValueType() ) );
    }
    else
    {
      files.emplace_back( filePtr.lock(), partNumber, checkValueType );
    }
  }

  return files;
}

void Load::dataFiles( const ConstLoadFiles &files )
{
  dataFilesV.assign( files.begin(), files.end() );
}

void Load::dataFile(
  const ConstRegularFilePtr &file,
  std::string partNumber,
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  if ( !file || ( file->mediaSet() != mediaSet() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "invalid File" } );
  }

  dataFilesV.emplace_back(
    file,
    std::move( partNumber ),
    checkValueType );
}

ConstLoadFiles Load::supportFiles( const bool effective ) const
{
  ConstLoadFiles files{};

  for ( const auto &[filePtr, partNumber, checkValueType ] : supportFilesV )
  {
    if ( effective )
    {
      files.emplace_back(
        filePtr.lock(),
        partNumber,
        checkValueType.value_or( effectiveSupportFilesCheckValueType() ) );
    }
    else
    {
      files.emplace_back( filePtr.lock(), partNumber, checkValueType );
    }
  }

  return files;
}

void Load::supportFiles( const ConstLoadFiles &files )
{
  supportFilesV.assign( files.begin(), files.end() );
}

void Load::supportFile(
  const ConstRegularFilePtr &file,
  std::string partNumber,
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  if ( !file || ( file->mediaSet() != mediaSet() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "invalid File" } );
  }

  supportFilesV.emplace_back(
    file,
    std::move( partNumber ),
    checkValueType );
}

ConstUserDefinedDataSpan Load::userDefinedData() const
{
  return userDefinedDataV;
}

UserDefinedData& Load::userDefinedData()
{
  return userDefinedDataV;
}

void Load::userDefinedData( UserDefinedData userDefinedData )
{
  userDefinedDataV = std::move( userDefinedData ) ;
}

const Load::Type& Load::loadType() const
{
  return typeV;
}

void Load::loadType( const Type &type )
{
  typeV = type;
}

void Load::loadType( Type &&type )
{
  typeV = std::move( type );
}

Arinc645::CheckValueType Load::effectiveLoadCheckValueType() const
{
  return loadCheckValueTypeV.value_or(
    mediaSet()->mediaSetCheckValueType().value_or(
      Arinc645::CheckValueType::NotUsed ) );
}

std::optional< Arinc645::CheckValueType > Load::loadCheckValueType() const
{
  return loadCheckValueTypeV;
}

void Load::loadCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  loadCheckValueTypeV = checkValueType;
}

Arinc645::CheckValueType Load::effectiveDataFilesCheckValueType() const
{
  return dataFilesCheckValueTypeV.value_or(
    mediaSet()->effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > Load::dataFilesCheckValueType() const
{
  return dataFilesCheckValueTypeV;
}

void Load::dataFilesCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  dataFilesCheckValueTypeV = checkValueType;
}

Arinc645::CheckValueType Load::effectiveSupportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV.value_or(
    mediaSet()->effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > Load::supportFilesCheckValueType() const
{
  return supportFilesCheckValueTypeV;
}

void Load::supportFilesCheckValueType(
  std::optional< Arinc645::CheckValueType > checkValueType )
{
  supportFilesCheckValueTypeV = checkValueType;
}

}

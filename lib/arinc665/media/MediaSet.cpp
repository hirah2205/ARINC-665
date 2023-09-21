// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::MediaSet.
 **/

#include "MediaSet.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <cassert>

namespace Arinc665::Media {

MediaSet::MediaSet():
  ContainerEntity{ MediumNumber{ 1U } }
{
}

ConstMediaSetPtr MediaSet::mediaSet() const
{
  return std::dynamic_pointer_cast< const MediaSet>( shared_from_this() );
}

MediaSetPtr MediaSet::mediaSet()
{
  return std::dynamic_pointer_cast< MediaSet >( shared_from_this() );
}

Type MediaSet::type() const
{
  return Type::MediaSet;
}

ConstContainerEntityPtr MediaSet::parent() const
{
  return {};
}

ContainerEntityPtr MediaSet::parent()
{
  return {};
}

std::filesystem::path MediaSet::path() const
{
  return std::filesystem::path{ { std::filesystem::path::preferred_separator } };
}

std::string_view MediaSet::partNumber() const
{
  return partNumberV;
}

void MediaSet::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
}

ConstLoads MediaSet::loadsWithFile( const ConstRegularFilePtr &file ) const
{
  ConstLoads foundLoads{};

  for ( const auto &load : recursiveLoads() )
  {
    bool loadAdded{ false };
    for ( const auto &[ dataFile, partNumber, checkValue ] : load->dataFiles() )
    {
      if ( dataFile == file )
      {
        foundLoads.emplace_back( load );
        loadAdded = true;
        break;
      }
    }

    if ( loadAdded )
    {
      break ;
    }

    for ( const auto &[ supportFile, partNumber, checkValue ] : load->supportFiles() )
    {
      if ( supportFile == file )
      {
        foundLoads.emplace_back( load );
      }
    }
  }

  return foundLoads;
}

ConstBatches MediaSet::batchesWithLoad( const ConstLoadPtr &load ) const
{
  ConstBatches foundBatches{};

  for ( const auto & batch : recursiveBatches() )
  {
    for ( const auto &[ thwIdPos, loads ]: batch->targets() )
    {
      if ( loads.end() != std::ranges::find( loads, load ) )
      {
        foundBatches.emplace_back( batch );
      }
    }
  }

  return foundBatches;
}

ConstUserDefinedDataSpan MediaSet::filesUserDefinedData() const
{
  return filesUserDefinedDataV;
}

UserDefinedData& MediaSet::filesUserDefinedData()
{
  return filesUserDefinedDataV;
}

void MediaSet::filesUserDefinedData( UserDefinedData userDefinedData )
{
  filesUserDefinedDataV = std::move( userDefinedData );
}

ConstUserDefinedDataSpan MediaSet::loadsUserDefinedData() const
{
  return loadsUserDefinedDataV;
}

UserDefinedData& MediaSet::loadsUserDefinedData()
{
  return loadsUserDefinedDataV;
}

void MediaSet::loadsUserDefinedData( UserDefinedData userDefinedData )
{
  loadsUserDefinedDataV = std::move( userDefinedData );
}

ConstUserDefinedDataSpan MediaSet::batchesUserDefinedData() const
{
  return batchesUserDefinedDataV;
}

UserDefinedData& MediaSet::batchesUserDefinedData()
{
  return batchesUserDefinedDataV;
}

void MediaSet::batchesUserDefinedData( UserDefinedData userDefinedData )
{
  batchesUserDefinedDataV = std::move( userDefinedData );
}

Arinc645::CheckValueType MediaSet::effectiveMediaSetCheckValueType() const
{
  return mediaSetCheckValueTypeV.value_or( Arinc645::CheckValueType::NotUsed );
}

std::optional< Arinc645::CheckValueType >
MediaSet::mediaSetCheckValueType() const
{
  return mediaSetCheckValueTypeV;
}

void MediaSet::mediaSetCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  mediaSetCheckValueTypeV = type;
}

Arinc645::CheckValueType MediaSet::effectiveListOfFilesCheckValueType() const
{
  return listOfFilesCheckValueTypeV.value_or(
    mediaSetCheckValueTypeV.value_or(
      Arinc645::CheckValueType::NotUsed ) );
}

std::optional< Arinc645::CheckValueType >
MediaSet::listOfFilesCheckValueType() const
{
  return listOfFilesCheckValueTypeV;
}

void MediaSet::listOfFilesCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  listOfFilesCheckValueTypeV = type;
}

Arinc645::CheckValueType MediaSet::effectiveListOfLoadsCheckValueType() const
{
  return
    listOfLoadsCheckValueTypeV.value_or( effectiveFilesCheckValueType() );
}

std::optional< Arinc645::CheckValueType >
MediaSet::listOfLoadsCheckValueType() const
{
  return listOfLoadsCheckValueTypeV;
}

void MediaSet::listOfLoadsCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  listOfLoadsCheckValueTypeV = type;
}

Arinc645::CheckValueType MediaSet::effectiveListOfBatchesCheckValueType() const
{
  return
    listOfBatchesCheckValueTypeV.value_or( effectiveFilesCheckValueType() );
}

std::optional< Arinc645::CheckValueType >
MediaSet::listOfBatchesCheckValueType() const
{
  return listOfBatchesCheckValueTypeV;
}

void MediaSet::listOfBatchesCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  listOfBatchesCheckValueTypeV = type;
}

Arinc645::CheckValueType MediaSet::effectiveFilesCheckValueType() const
{
  return filesCheckValueTypeV.value_or( effectiveMediaSetCheckValueType() );
}

std::optional< Arinc645::CheckValueType > MediaSet::filesCheckValueType() const
{
  return filesCheckValueTypeV;
}

void MediaSet::filesCheckValueType(
  std::optional< Arinc645::CheckValueType > type )
{
  filesCheckValueTypeV = type;
}

}

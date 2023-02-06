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

#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <boost/foreach.hpp>

#include <cassert>

namespace Arinc665::Media {

ConstMediaSetPtr MediaSet::mediaSet() const
{
  return std::dynamic_pointer_cast< const MediaSet>( shared_from_this() );
}

MediaSetPtr MediaSet::mediaSet()
{
  return std::dynamic_pointer_cast< MediaSet>( shared_from_this() );
}

Type MediaSet::type() const
{
  return Type::MediaSet;
}

std::string_view MediaSet::partNumber() const
{
  return partNumberV;
}

void MediaSet::partNumber( std::string partNumber )
{
  partNumberV = std::move( partNumber );
}

uint8_t MediaSet::numberOfMedia() const
{
  assert( mediaV.size() <= std::numeric_limits< uint8_t>::max() );

  return static_cast< uint8_t>( mediaV.size() );
}

ConstMedia MediaSet::media() const
{
  return ConstMedia{ mediaV.begin(), mediaV.end() };
}

Media MediaSet::media()
{
  return mediaV;
}

void MediaSet::addMedia( const uint8_t numberOfMedia )
{
  if ( mediaV.size() + numberOfMedia > 255 )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Number of media exceeds limit " } );
  }

  for ( uint8_t count{ 0 }; count < numberOfMedia; ++count )
  {
    // Add media
    (void)addMedium();
  }
}

ConstMediumPtr MediaSet::medium( const uint8_t number ) const
{
  const auto medium{ mediaV.find( number ) };

  if ( medium == mediaV.end() )
  {
    return {};
  }

  return medium->second;
}

MediumPtr MediaSet::medium( const uint8_t number )
{
  const auto medium{ mediaV.find( number ) };

  if ( medium == mediaV.end() )
  {
    return {};
  }

  return medium->second;
}

MediumPtr MediaSet::addMedium()
{
  if ( mediaV.size() >= 255U )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "Maximum number of media reached";
    return {};
  }

  const auto newIndex{ static_cast< uint8_t>( mediaV.size() + 1U ) };

  auto medium{ std::make_shared< Medium>(
    std::dynamic_pointer_cast< MediaSet>( shared_from_this() ),
    newIndex ) };

  mediaV.try_emplace( newIndex, medium );

  return medium;
}

void MediaSet::removeMedium()
{
  const auto lastMedium{ medium( numberOfMedia() ) };
  assert( lastMedium );

  if ( lastMedium->hasChildren() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Medium not empty" } );
  }

  if ( 1 != mediaV.erase( lastMedium->mediumNumber() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Error erasing medium" } );
  }
}

size_t MediaSet::numberOfFiles() const
{
  size_t numberOfFiles{ 0U };

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    numberOfFiles += medium->recursiveNumberOfFiles();
  }

  return numberOfFiles;
}

ConstFiles MediaSet::files() const
{
  ConstFiles files{};

  // Iterate over all media and add their files to a complete list.
  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice(
      files.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveFiles() );
  }

  return files;
}

Files MediaSet::files()
{
  Files files{};

  // Iterate over all media and add their files to a complete list.
  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), medium->recursiveFiles() );
  }

  return files;
}

ConstFiles MediaSet::files( std::string_view filename ) const
{
  ConstFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice(
      files.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveFiles(
        filename ) );
  }

  return files;
}

Files MediaSet::files( std::string_view filename )
{
  Files files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), medium->recursiveFiles( filename ) );
  }

  return files;
}

size_t MediaSet::numberOfRegularFiles() const
{
  size_t numberOfRegularFiles{ 0 };

  for ( const auto &[  mediumNumber, medium ] : mediaV )
  {
    numberOfRegularFiles += medium->recursiveNumberOfRegularFiles();
  }

  return numberOfRegularFiles;
}

ConstRegularFiles MediaSet::regularFiles() const
{
  ConstRegularFiles regularFiles;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    regularFiles.splice(
      regularFiles.end(),
      std::const_pointer_cast< const Medium >(
        medium )->recursiveRegularFiles() );
  }

  return regularFiles;
}

RegularFiles MediaSet::regularFiles()
{
  RegularFiles regularFiles{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    regularFiles.splice( regularFiles.end(), medium->recursiveRegularFiles() );
  }

  return regularFiles;
}

ConstRegularFiles MediaSet::regularFiles( std::string_view filename ) const
{
  ConstRegularFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice(
      files.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveRegularFiles(
        filename ) );
  }

  return files;
}

RegularFiles MediaSet::regularFiles( std::string_view filename )
{
  RegularFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), medium->recursiveRegularFiles( filename ) );
  }

  return files;
}

size_t MediaSet::numberOfLoads() const
{
  size_t numberOfLoads{ 0 };

  for ( const auto &[  mediumNumber, medium ] : mediaV )
  {
    numberOfLoads += medium->recursiveNumberOfLoads();
  }

  return numberOfLoads;
}

ConstLoads MediaSet::loads() const
{
  ConstLoads loads;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    loads.splice(
      loads.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveLoads() );
  }

  return loads;
}

Loads MediaSet::loads()
{
  Loads loads;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), medium->recursiveLoads() );
  }

  return loads;
}

ConstLoads MediaSet::loads( std::string_view filename ) const
{
  ConstLoads loads{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    loads.splice(
      loads.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveLoads(
        filename ) );
  }

  return loads;
}

Loads MediaSet::loads( std::string_view filename )
{
  Loads loads{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), medium->recursiveLoads( filename ) );
  }

  return loads;
}

ConstLoads MediaSet::loadsWithFile( const ConstRegularFilePtr &file ) const
{
  ConstLoads foundLoads{};

  for ( const auto & load : loads() )
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

size_t MediaSet::numberOfBatches() const
{
  size_t numberOfBatches{ 0 };

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    numberOfBatches += medium->recursiveNumberOfBatches();
  }

  return numberOfBatches;
}

ConstBatches MediaSet::batches() const
{
  ConstBatches batches{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    batches.splice(
      batches.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveBatches() );
  }

  return batches;
}

Batches MediaSet::batches()
{
  Batches batches{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), medium->recursiveBatches() );
  }

  return batches;
}

ConstBatches MediaSet::batches( std::string_view filename ) const
{
  ConstBatches batches{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    batches.splice(
      batches.end(),
      std::const_pointer_cast< const Medium >( medium )->recursiveBatches(
        filename ) );
  }

  return batches;
}

Batches MediaSet::batches( std::string_view filename )
{
  Batches batches{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), medium->recursiveBatches( filename ) );
  }

  return batches;
}

ConstBatches MediaSet::batchesWithLoad( const ConstLoadPtr &load ) const
{
  ConstBatches foundBatches{};

  for ( const auto & batch : batches() )
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

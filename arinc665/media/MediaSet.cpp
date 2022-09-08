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

MediaSet::Type MediaSet::type() const
{
  return Type::MediaSet;
}

std::string_view MediaSet::partNumber() const
{
  return partNumberV;
}

void MediaSet::partNumber( std::string_view partNumber )
{
  partNumberV = partNumber;
}

void MediaSet::partNumber( std::string &&partNumber )
{
  partNumberV = std::move( partNumber );
}

uint8_t MediaSet::numberOfMedia() const
{
  assert( mediaV.size() <= std::numeric_limits< uint8_t>::max() );

  return static_cast< uint8_t>( mediaV.size() );
}

void MediaSet::numberOfMedia(
  const uint8_t numberOfMedia,
  const bool deleteFiles )
{
  if ( numberOfMedia == mediaV.size() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::info )
      << "No actions needed";
    return;
  }

  if ( numberOfMedia > mediaV.size() )
  {
    // Add media
    while ( numberOfMedia > mediaV.size() )
    {
      (void)addMedium();
    }
  }
  else
  {
    // remove media
    while ( numberOfMedia < mediaV.size() )
    {
      removeMedium( deleteFiles );
    }
  }
}

ConstMedia MediaSet::media() const
{
  return ConstMedia{ mediaV.begin(), mediaV.end() };
}

Media MediaSet::media()
{
  return mediaV;
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

void MediaSet::removeMedium( const bool deleteFiles [[maybe_unused]])
{
  //! @todo implement
  BOOST_THROW_EXCEPTION( std::exception() );
}

size_t MediaSet::numberOfFiles() const
{
  size_t numberOfFiles{ 0U };

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    numberOfFiles += recursiveNumberOfFiles( *medium );
  }

  return numberOfFiles;
}

ConstFiles MediaSet::files() const
{
  ConstFiles files{};

  // Iterate over all media and add their files to a complete list.
  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveFiles( *medium ) );
  }

  return files;
}

Files MediaSet::files()
{
  Files files{};

  // Iterate over all media and add their files to a complete list.
  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveFiles( *medium ) );
  }

  return files;
}

ConstFiles MediaSet::files( std::string_view filename ) const
{
  ConstFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveFiles( *medium, filename ) );
  }

  return files;
}

Files MediaSet::files( std::string_view filename )
{
  Files files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveFiles( *medium, filename ) );
  }

  return files;
}

size_t MediaSet::numberOfRegularFiles() const
{
  size_t numberOfRegularFiles{ 0 };

  for ( const auto &[  mediumNumber, medium ] : mediaV )
  {
    numberOfRegularFiles += recursiveNumberOfRegularFiles( *medium );
  }

  return numberOfRegularFiles;
}

ConstRegularFiles MediaSet::regularFiles() const
{
  ConstRegularFiles regularFiles;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    regularFiles.splice( regularFiles.end(), recursiveRegularFiles( *medium) );
  }

  return regularFiles;
}

RegularFiles MediaSet::regularFiles()
{
  RegularFiles regularFiles{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    regularFiles.splice( regularFiles.end(), recursiveRegularFiles( *medium) );
  }

  return regularFiles;
}

ConstRegularFiles MediaSet::regularFiles( std::string_view filename ) const
{
  ConstRegularFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveRegularFiles( *medium, filename ) );
  }

  return files;
}

RegularFiles MediaSet::regularFiles( std::string_view filename )
{
  RegularFiles files{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    files.splice( files.end(), recursiveRegularFiles( *medium, filename ) );
  }

  return files;
}

size_t MediaSet::numberOfLoads() const
{
  size_t numberOfLoads{ 0 };

  for ( const auto &[  mediumNumber, medium ] : mediaV )
  {
    numberOfLoads += recursiveNumberOfLoads( *medium );
  }

  return numberOfLoads;
}

ConstLoads MediaSet::loads() const
{
  ConstLoads loads;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), recursiveLoads( *medium) );
  }

  return loads;
}

Loads MediaSet::loads()
{
  Loads loads;

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), recursiveLoads( *medium) );
  }

  return loads;
}

ConstLoads MediaSet::loads( std::string_view filename ) const
{
  ConstLoads loads{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), recursiveLoads( *medium, filename ) );
  }

  return loads;
}

Loads MediaSet::loads( std::string_view filename )
{
  Loads loads{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    loads.splice( loads.end(), recursiveLoads( *medium, filename ) );
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
    numberOfBatches += recursiveNumberOfBatches( *medium );
  }

  return numberOfBatches;
}

ConstBatches MediaSet::batches() const
{
  ConstBatches batches{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), recursiveBatches( *medium ) );
  }

  return batches;
}

Batches MediaSet::batches()
{
  Batches batches{};

  for ( const auto & [ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), recursiveBatches( *medium ) );
  }

  return batches;
}

ConstBatches MediaSet::batches( std::string_view filename ) const
{
  ConstBatches batches{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), recursiveBatches( *medium, filename ) );
  }

  return batches;
}

Batches MediaSet::batches( std::string_view filename )
{
  Batches batches{};

  for ( const auto &[ mediumNumber, medium ] : mediaV )
  {
    batches.splice( batches.end(), recursiveBatches( *medium, filename ) );
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

void MediaSet::filesUserDefinedData( ConstUserDefinedDataSpan userDefinedData )
{
  filesUserDefinedDataV.assign( userDefinedData.begin(), userDefinedData.end() );
}

void MediaSet::filesUserDefinedData( UserDefinedData &&userDefinedData )
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

void MediaSet::loadsUserDefinedData( ConstUserDefinedDataSpan userDefinedData )
{
  loadsUserDefinedDataV.assign( userDefinedData.begin(), userDefinedData.end() );
}

void MediaSet::loadsUserDefinedData( UserDefinedData &&userDefinedData )
{
  loadsUserDefinedDataV = std::move( userDefinedData);
}

ConstUserDefinedDataSpan MediaSet::batchesUserDefinedData() const
{
  return batchesUserDefinedDataV;
}

UserDefinedData& MediaSet::batchesUserDefinedData()
{
  return batchesUserDefinedDataV;
}

void MediaSet::batchesUserDefinedData( ConstUserDefinedDataSpan userDefinedData )
{
  batchesUserDefinedDataV.assign(
    userDefinedData.begin(),
    userDefinedData.end() );
}

void MediaSet::batchesUserDefinedData( UserDefinedData &&userDefinedData)
{
  batchesUserDefinedDataV = std::move( userDefinedData);
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

size_t MediaSet::recursiveNumberOfFiles( const ContainerEntity &container ) const
{
  size_t numberOfFiles{ container.numberOfFiles() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    numberOfFiles += recursiveNumberOfFiles( *subdirectory );
  }

  return numberOfFiles;
}

ConstFiles MediaSet::recursiveFiles( const ContainerEntity &container ) const
{
  ConstFiles files{ container.files() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice(
      files.begin(),
      recursiveFiles( *subdirectory ) );
  }

  return files;
}

Files MediaSet::recursiveFiles( ContainerEntity & container )
{
  Files files{ container.files() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice(
      files.begin(),
      recursiveFiles( *subdirectory ) );
  }

  return files;
}


ConstFiles MediaSet::recursiveFiles(
  const ContainerEntity &container,
  std::string_view filename ) const
{
  ConstFiles files{};

  if ( auto file{ container.file( filename ) }; file )
  {
    files.emplace_back( std::move( file ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice( files.end(), recursiveFiles( *subdirectory, filename ) );
  }

  return files;
}

Files MediaSet::recursiveFiles(
  ContainerEntity &container,
  std::string_view filename )
{
  Files files{};

  if ( auto file{ container.file( filename ) }; file )
  {
    files.emplace_back( std::move( file ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice( files.end(), recursiveFiles( *subdirectory, filename ) );
  }

  return files;
}

size_t MediaSet::recursiveNumberOfRegularFiles(
  const ContainerEntity &container ) const
{
  size_t numberOfRegularFiles{ container.numberOfRegularFiles() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    numberOfRegularFiles += recursiveNumberOfRegularFiles( *subdirectory );
  }

  return numberOfRegularFiles;
}

ConstRegularFiles MediaSet::recursiveRegularFiles(
  const ContainerEntity &container ) const
{
  ConstRegularFiles files{ container.regularFiles() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice(
      files.begin(),
      recursiveRegularFiles( *subdirectory ) );
  }

  return files;
}

RegularFiles MediaSet::recursiveRegularFiles( ContainerEntity & container )
{
  RegularFiles files{ container.regularFiles() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice(
      files.begin(),
      recursiveRegularFiles( *subdirectory ) );
  }

  return files;
}

ConstRegularFiles MediaSet::recursiveRegularFiles(
  const ContainerEntity &container,
  std::string_view filename ) const
{
  ConstRegularFiles files{};

  if ( auto file{ container.regularFile( filename ) }; file )
  {
    files.emplace_back( std::move( file ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice( files.end(), recursiveRegularFiles( *subdirectory, filename ) );
  }

  return files;
}

RegularFiles MediaSet::recursiveRegularFiles(
  ContainerEntity &container,
  std::string_view filename )
{
  RegularFiles files{};

  if ( auto file{ container.regularFile( filename ) }; file )
  {
    files.emplace_back( std::move( file ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    files.splice( files.end(), recursiveRegularFiles( *subdirectory, filename ) );
  }

  return files;
}

size_t MediaSet::recursiveNumberOfLoads(
  const ContainerEntity &container ) const
{
  size_t numberOfLoads{ container.numberOfLoads() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    numberOfLoads += recursiveNumberOfLoads( *subdirectory );
  }

  return numberOfLoads;
}

ConstLoads MediaSet::recursiveLoads( const ContainerEntity &container ) const
{
  ConstLoads loads{ container.loads() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    loads.splice(
      loads.begin(),
      recursiveLoads( *subdirectory ) );
  }

  return loads;
}

Loads MediaSet::recursiveLoads( ContainerEntity & container )
{
  Loads loads{ container.loads() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    loads.splice(
      loads.begin(),
      recursiveLoads( *subdirectory ) );
  }

  return loads;
}

ConstLoads MediaSet::recursiveLoads(
  const ContainerEntity &container,
  std::string_view filename ) const
{
  ConstLoads loads{};

  if ( auto load{ container.load( filename ) }; load )
  {
    loads.emplace_back( std::move( load ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    loads.splice( loads.end(), recursiveLoads( *subdirectory, filename ) );
  }

  return loads;
}

Loads MediaSet::recursiveLoads(
  ContainerEntity &container,
  std::string_view filename )
{
  Loads loads{};

  if ( auto load{ container.load( filename ) }; load )
  {
    loads.emplace_back( std::move( load ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    loads.splice( loads.end(), recursiveLoads( *subdirectory, filename ) );
  }

  return loads;
}

size_t MediaSet::recursiveNumberOfBatches(
  const ContainerEntity &container ) const
{
  size_t numberOfBatches{ container.numberOfBatches() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    numberOfBatches += recursiveNumberOfBatches( *subdirectory );
  }

  return numberOfBatches;
}

ConstBatches MediaSet::recursiveBatches( const ContainerEntity &container ) const
{
  ConstBatches batches{ container.batches() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    batches.splice(
      batches.begin(),
      recursiveBatches( *subdirectory ) );
  }

  return batches;
}

Batches MediaSet::recursiveBatches( ContainerEntity & container )
{
  Batches batches{ container.batches() };

  for ( const auto &subdirectory : container.subdirectories() )
  {
    batches.splice(
      batches.begin(),
      recursiveBatches( *subdirectory ) );
  }

  return batches;
}


ConstBatches MediaSet::recursiveBatches(
  const ContainerEntity &container,
  std::string_view filename ) const
{
  ConstBatches batches{};

  if ( auto batch{ container.batch( filename ) }; batch )
  {
    batches.emplace_back( std::move( batch ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    batches.splice( batches.end(), recursiveBatches( *subdirectory, filename ) );
  }

  return batches;
}

Batches MediaSet::recursiveBatches(
  ContainerEntity &container,
  std::string_view filename )
{
  Batches batches{};

  if ( auto batch{ container.batch( filename ) }; batch )
  {
    batches.emplace_back( std::move( batch ) );
  }

  for ( const auto &subdirectory : container.subdirectories() )
  {
    batches.splice( batches.end(), recursiveBatches( *subdirectory, filename ) );
  }

  return batches;
}

}

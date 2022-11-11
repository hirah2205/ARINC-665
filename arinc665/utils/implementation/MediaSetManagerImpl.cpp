/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetManagerImpl.
 **/

#include "MediaSetManagerImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>

#include <arinc665/utils/MediaSetImporter.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <boost/format.hpp>

#include <fstream>
#include <utility>

namespace Arinc665::Utils {

MediaSetManagerImpl::MediaSetManagerImpl(
  std::filesystem::path basePath,
  const MediaSetManagerConfiguration &configuration,
  const bool checkFileIntegrity ) :
  basePath{ std::move( basePath ) }
{
  BOOST_LOG_FUNCTION()
  loadMediaSets( configuration, checkFileIntegrity );
}

MediaSetManagerConfiguration MediaSetManagerImpl::configuration() const
{
  MediaSetManagerConfiguration configuration{};

  for ( const auto &[ partNumber, mediaSetPaths ] : mediaSetsPathsV )
  {
    configuration.mediaSets.emplace_back( mediaSetPaths );
  }

  return configuration;
}

bool MediaSetManagerImpl::hasMediaSet( std::string_view partNumber ) const
{
  return mediaSetsInformationV.contains( partNumber );
}

std::optional< MediaSetManagerImpl::MediaSetInformation >
MediaSetManagerImpl::mediaSet( std::string_view partNumber ) const
{
  auto mediaSet{ mediaSetsInformationV.find( partNumber ) };

  if ( mediaSet == mediaSetsInformationV.end() )
  {
    return {};
  }

  return mediaSet->second;
}

const MediaSetManagerImpl::MediaSetsInformation&
MediaSetManagerImpl::mediaSets() const
{
  return mediaSetsInformationV;
}

void MediaSetManagerImpl::registerMediaSet(
  const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
  const bool checkFileIntegrity )
{
  // import media set
  auto importer( MediaSetImporter::create() );

  // configure importer
  importer
    ->fileSizeHandler( std::bind_front(
      &MediaSetManagerImpl::fileSizeHandler,
      this,
      mediaSetPaths ) )
    .readFileHandler( std::bind_front(
      &MediaSetManagerImpl::readFileHandler,
      this,
      mediaSetPaths ) )
    .checkFileIntegrity( checkFileIntegrity );

  // import media set
  auto [ impMediaSet, checkValues ]{ (*importer)() };
  assert( impMediaSet );

  if ( mediaSet( impMediaSet->partNumber() ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set already exist" } );
  }

  std::string partNumber{ impMediaSet->partNumber() };
  // add to media sets information
  mediaSetsInformationV.try_emplace(
    partNumber,
    std::move( impMediaSet ),
    std::move( checkValues ) );

  // add to media sets paths
  mediaSetsPathsV.try_emplace(
    partNumber,
    mediaSetPaths.first,
    mediaSetPaths.second );
}

MediaSetManagerConfiguration::MediaSetPaths
MediaSetManagerImpl::deregisterMediaSet( std::string_view partNumber )
{
  auto mediaSetInformation{
    mediaSetsInformationV.extract( std::string{ partNumber } ) };

  // extract from media sets information
  if ( !mediaSetInformation )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set not found" } );
  }

  // extract from media sets paths
  auto mediaSetPaths{ mediaSetsPathsV.extract( std::string{ partNumber } ) };

  if ( !mediaSetPaths )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set paths not found" } );
  }

  // return the extracted paths information
  return mediaSetPaths.mapped();
}

Media::ConstLoads MediaSetManagerImpl::loads() const
{
  Media::ConstLoads loads{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetsInformationV )
  {
    loads.splice( loads.end(), mediaSet.first->loads() );
  }

  return loads;
}

Media::ConstLoads MediaSetManagerImpl::loads( std::string_view filename ) const
{
  Media::ConstLoads loads{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetsInformationV )
  {
    loads.splice( loads.end(), mediaSet.first->loads( filename ) );
  }

  return loads;
}

Media::ConstLoads MediaSetManagerImpl::loads(
  std::string_view partNumber,
  std::string_view filename,
  const Arinc645::CheckValue &checkValue ) const
{
  // Get Media Set with given Part Number
  const auto mediaSetInfo{ mediaSet( partNumber ) };

  if ( !mediaSetInfo )
  {
    return {};
  }

  auto loads{ mediaSetInfo->first->loads( filename ) };

  // if no check value has been provided -> return directly
  if ( Arinc645::CheckValue::NoCheckValue != checkValue )
  {
    return loads;
  }

  // iterate over found loads
  for ( const auto &load : loads )
  {
    // get check values for load
    const auto checkValueIt{ mediaSetInfo->second.find( load ) };

    if ( checkValueIt == mediaSetInfo->second.end() )
    {
      // file not found within check values -> remove from list
      loads.remove( load );
      continue;
    }

    // check for correct check value
    if ( !checkValueIt->second.contains( checkValue ) )
    {
      // No Check value found within set -> remove from list
      loads.remove( load );
      continue;
    }
  }

  return loads;
}

Media::ConstFiles MediaSetManagerImpl::files(
  std::string_view partNumber,
  std::string_view filename,
  const Arinc645::CheckValue &checkValue ) const
{
  // Get Media Set with given Part Number
  const auto mediaSetInfo{ mediaSet( partNumber ) };

  if ( !mediaSetInfo )
  {
    return {};
  }

  auto files{ mediaSetInfo->first->files( filename ) };

  // if no check value has been provided -> return directly
  if ( Arinc645::CheckValue::NoCheckValue != checkValue )
  {
    return files;
  }

  // iterate over found files
  for ( const auto &file : files )
  {
    // get check values for file
    const auto checkValueIt{ mediaSetInfo->second.find( file ) };

    if ( checkValueIt ==  mediaSetInfo->second.end() )
    {
      // file not found within check values -> remove from list
      files.remove( file );
      continue;
    }

    // check for correct check value
    if ( !checkValueIt->second.contains( checkValue ) )
    {
      // No Check value found within set -> remove from list
      files.remove( file );
      continue;
    }
  }

  return files;
}

std::filesystem::path MediaSetManagerImpl::filePath(
  const Media::ConstFilePtr &file ) const
{
  BOOST_LOG_FUNCTION()

  if ( !file )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
      << "Given file is empty";
  }

  auto mediaSetIt{ mediaSetsPathsV.find( file->mediaSet()->partNumber() ) };

  if ( mediaSetIt == mediaSetsPathsV.end() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
      << "Media Set not found";
    return {};
  }

  auto mediumIt{ mediaSetIt->second.second.find(
    file->parent()->medium()->mediumNumber() ) };

  if ( mediumIt == mediaSetIt->second.second.end() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
      << "Medium not found";
    return {};
  }

  return absolutePath(
    mediaSetIt->second.first / mediumIt->second / file->path().relative_path() );
}

void MediaSetManagerImpl::loadMediaSets(
  const MediaSetManagerConfiguration &configuration,
  const bool checkFileIntegrity )
{
  BOOST_LOG_FUNCTION()

  for ( auto const &mediaSetPaths : configuration.mediaSets )
  {
    // import media set
    auto importer( MediaSetImporter::create() );

    // configure importer
    importer
      ->fileSizeHandler( std::bind_front(
        &MediaSetManagerImpl::fileSizeHandler,
        this,
        mediaSetPaths ) )
      .readFileHandler( std::bind_front(
        &MediaSetManagerImpl::readFileHandler,
        this,
        mediaSetPaths ) )
      .checkFileIntegrity( checkFileIntegrity );

    // import media set
    auto [ impMediaSet, checkValues ]{ (*importer)() };
    assert( impMediaSet );

    std::string partNumber{ impMediaSet->partNumber() };
    // add to media sets information
    mediaSetsInformationV.try_emplace(
      partNumber,
      std::move( impMediaSet ),
      std::move( checkValues ) );
    // add to media sets paths
    mediaSetsPathsV.try_emplace(
      partNumber,
      mediaSetPaths );
  }
}

size_t MediaSetManagerImpl::fileSizeHandler(
  const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
  uint8_t mediumNumber,
  const std::filesystem::path &path ) const
{
  // make structure binding here instead
  const auto &[ mediaSetPath, mediaPaths ]{ mediaSetPaths };

  auto medium{ mediaPaths.find( mediumNumber ) };

  if ( mediaPaths.end() == medium )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Medium not found" } );
  }

  // concatenate file path
  auto filePath{
    absolutePath(
      mediaSetPath / medium->second / path.relative_path() ) };

  return std::filesystem::file_size( filePath );
}

Files::RawFile MediaSetManagerImpl::readFileHandler(
  const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
  uint8_t mediumNumber,
  const std::filesystem::path &path ) const
{
  // make structure binding here instead
  const auto &[ mediaSetPath, mediaPaths ]{ mediaSetPaths };

  auto medium{ mediaPaths.find( mediumNumber ) };

  if ( mediaPaths.end() == medium )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::warning )
      << "Medium not found";

    return {};
  }

  // concatenate file path
  auto filePath{
    absolutePath( mediaSetPath / medium->second / path.relative_path() ) };

  // read file
  Files::RawFile data( std::filesystem::file_size( filePath ) );

  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in };

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" } );
  }

  // read the data to the buffer
  file.read(
    (char*) std::data( data ),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}

std::filesystem::path MediaSetManagerImpl::absolutePath(
  const std::filesystem::path &filePath ) const
{
  return ( basePath / filePath ).lexically_normal();
}

}

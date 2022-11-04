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
  MediaSetManagerConfiguration configuration,
  const bool checkFileIntegrity ) :
  basePath{ std::move( basePath ) },
  configurationV{ std::move( configuration ) }
{
  BOOST_LOG_FUNCTION()
  loadMediaSets( checkFileIntegrity );
}

const MediaSetManagerConfiguration & MediaSetManagerImpl::configuration() const
{
  return configurationV;
}

bool MediaSetManagerImpl::hasMediaSet( std::string_view partNumber ) const
{
  return mediaSetsV.contains( partNumber );
}

std::optional< MediaSetManagerImpl::MediaSetInformation >
MediaSetManagerImpl::mediaSet( std::string_view partNumber ) const
{
  auto mediaSet{ mediaSetsV.find( partNumber ) };

  if ( mediaSet == mediaSetsV.end() )
  {
    return {};
  }

  return mediaSet->second;
}

const MediaSetManagerImpl::MediaSetsInformation&
MediaSetManagerImpl::mediaSets() const
{
  return mediaSetsV;
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
    .readFileHandler(
      std::bind_front(
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

  // add to media sets
  mediaSetsV.try_emplace(
    std::string{ impMediaSet->partNumber() },
    std::move( impMediaSet ),
    std::move( checkValues ) );

  // add to configuration
  configurationV.mediaSets.try_emplace(
    mediaSetPaths.first,
    mediaSetPaths.second );
}

MediaSetManagerConfiguration::MediaSetPaths
MediaSetManagerImpl::deregisterMediaSet( std::string_view partNumber )
{
  const auto foundMediaSet{ mediaSetsV.find( partNumber ) };

  if ( foundMediaSet == mediaSetsV.end() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set not found" } );
  }

  // find media set paths for media set
  const auto mediaSetPath{ mediaSetsPathsV.find( foundMediaSet->second.first ) };

  if ( mediaSetPath == mediaSetsPathsV.end() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set paths not found" } );
  }

  // Remove Media Set from Media Sets List
  mediaSetsV.erase( foundMediaSet );

  // Remove Path Configuration
  auto mediaSetPathConfigIt{
    configurationV.mediaSets.find( mediaSetPath->second.first ) };

  if ( mediaSetPathConfigIt == configurationV.mediaSets.end() )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Media Set paths configuration not found" } );
  }

  // save content of iterator (element is not available after removal from map
  auto mediaSetPathConfig{ *mediaSetPathConfigIt };
  configurationV.mediaSets.erase( mediaSetPathConfigIt );

  // Remove Media Set Path Config
  mediaSetsPathsV.erase( mediaSetPath );

  return mediaSetPathConfig;
}

Media::ConstLoads MediaSetManagerImpl::loads() const
{
  Media::ConstLoads loads{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetsV )
  {
    loads.splice( loads.end(), mediaSet.first->loads() );
  }

  return loads;
}

Media::ConstLoads MediaSetManagerImpl::loads( std::string_view filename ) const
{
  Media::ConstLoads loads{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetsV )
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
    // get all check values for load
    const auto [ checkValueStart, checkValueEnd ]{
      mediaSetInfo->second.equal_range( load ) };

    if ( checkValueStart ==  mediaSetInfo->second.end() )
    {
      // no check value found -> remove from list
      loads.remove( load );
      continue;
    }

    if ( std::none_of(
      checkValueStart,
      checkValueEnd,
      [&checkValue]( const auto &loadCheckValue ){
         return checkValue == loadCheckValue.second;
       } ) )
    {
      // No Check value found -> remove from list
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
    // get all check values for file
    const auto [ checkValueStart, checkValueEnd ]{
      mediaSetInfo->second.equal_range( file ) };

    if ( checkValueStart ==  mediaSetInfo->second.end() )
    {
      // no check value found -> remove from list
      files.remove( file );
      continue;
    }

    if ( std::none_of(
           checkValueStart,
           checkValueEnd,
           [&checkValue]( const auto &loadCheckValue ){
             return checkValue == loadCheckValue.second;
           } ) )
    {
      // No Check value found -> remove from list
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

  auto mediaSetIt{ mediaSetsPathsV.find( file->mediaSet() ) };

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

void MediaSetManagerImpl::loadMediaSets( const bool checkFileIntegrity )
{
  BOOST_LOG_FUNCTION()

  for ( auto const &mediaSetPaths : configurationV.mediaSets )
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

    // Add media set paths
    mediaSetsPathsV.try_emplace( impMediaSet, mediaSetPaths );
    // add media set
    mediaSetsV.try_emplace(
      std::string{ impMediaSet->partNumber() },
      std::move( impMediaSet ),
      std::move( checkValues ) );
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
    absolutePath(
      mediaSetPath / medium->second / path.relative_path() ) };

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

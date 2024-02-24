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
 * @brief Definition of Class Arinc665::Utils::MediaSetManagerImpl.
 **/

#include "MediaSetManagerImpl.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>

#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <helper/Exception.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/exception/all.hpp>

#include <utility>

namespace Arinc665::Utils {

MediaSetManagerImpl::MediaSetManagerImpl(
  std::filesystem::path directory,
  const bool checkFileIntegrity,
  LoadProgressHandler loadProgressHandler ) :
  directoryV{ std::move( directory ) }
{
  BOOST_LOG_FUNCTION()

  const auto configurationFile{ directoryV / ConfigurationFilename };

  if ( !std::filesystem::is_regular_file( configurationFile ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "Media Set Configuration file does not exists" }
      << boost::errinfo_file_name{ configurationFile.string() } );
  }

  boost::property_tree::ptree configurationProperties{};

  boost::property_tree::json_parser::read_json(
    configurationFile.string(),
    configurationProperties );

  auto configuration{
    Arinc665::Utils::MediaSetManagerConfiguration{ configurationProperties } };

  loadMediaSets(
    configuration.mediaSets,
    checkFileIntegrity,
    std::move( loadProgressHandler ) );

  mediaSetDefaultsV = std::move( configuration.defaults );
}

MediaSetManagerImpl::~MediaSetManagerImpl()
{
  BOOST_LOG_FUNCTION()

  try
  {
    saveConfiguration();
  }
  catch ( const Arinc665Exception &e )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::fatal )
      << "Save configuration: " << boost::diagnostic_information( e );
  }
}

const MediaSetDefaults & MediaSetManagerImpl::mediaSetDefaults() const
{
  return mediaSetDefaultsV;
}

MediaSetDefaults & MediaSetManagerImpl::mediaSetDefaults()
{
  return mediaSetDefaultsV;
}

void MediaSetManagerImpl::mediaSetDefaults( MediaSetDefaults mediaSetDefaults )
{
  mediaSetDefaultsV = std::move( mediaSetDefaults );
}

MediaSetManagerConfiguration MediaSetManagerImpl::configuration() const
{
  MediaSetManagerConfiguration configuration{};

  for ( const auto &[ partNumber, mediaSetPaths ] : mediaSetsPathsV )
  {
    configuration.mediaSets.emplace_back( mediaSetPaths );
  }

  configuration.defaults = mediaSetDefaultsV;

  return configuration;
}

void MediaSetManagerImpl::saveConfiguration()
{
  BOOST_LOG_FUNCTION()

  try
  {
    boost::property_tree::write_json(
      ( directoryV / ConfigurationFilename ).string(),
      configuration().toProperties() );
  }
  catch ( const boost::property_tree::json_parser_error &e )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
      << "Save configuration " << e.filename() << " " << e.message();

    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( e.message() )
      << boost::errinfo_file_name( e.filename() ) );
  }
}

const std::filesystem::path& MediaSetManagerImpl::directory() const
{
  return directoryV;
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
  const MediaSetPaths &mediaSetPaths,
  const bool checkFileIntegrity )
{
  auto decompiler( FilesystemMediaSetDecompiler::create() );
  assert( decompiler );

  // configure decompiler
  decompiler
    ->checkFileIntegrity( checkFileIntegrity )
    .mediaPaths( absoluteMediaPaths( mediaSetPaths ) );

  // import media set
  auto [ impMediaSet, checkValues ]{ ( *decompiler )() };
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
  mediaSetsPathsV.try_emplace( partNumber, mediaSetPaths );
}

MediaSetPaths MediaSetManagerImpl::deregisterMediaSet(
  std::string_view partNumber )
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
    loads.splice( loads.end(), mediaSet.first->recursiveLoads() );
  }

  return loads;
}

Media::ConstBatches MediaSetManagerImpl::batches() const
{
  Media::ConstBatches batches{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetsInformationV )
  {
    batches.splice( batches.end(), mediaSet.first->recursiveBatches() );
  }

  return batches;
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

  auto mediumIt{
    mediaSetIt->second.second.find( file->effectiveMediumNumber() ) };

  if ( mediumIt == mediaSetIt->second.second.end() )
  {
    BOOST_LOG_SEV( Arinc665Logger::get(), Helper::Severity::error )
      << "Medium not found";
    return {};
  }

  return ( directoryV / mediaSetIt->second.first / mediumIt->second
    / file->path().relative_path() ).lexically_normal();
}

void MediaSetManagerImpl::loadMediaSets(
  const MediaSetManagerConfiguration::MediaSetsPaths &mediaSetsPaths,
  const bool checkFileIntegrity,
  LoadProgressHandler loadProgressHandler )
{
  BOOST_LOG_FUNCTION()

  for ( size_t mediaSetCounter{ 1U };
    auto const &mediaSetPaths : mediaSetsPaths )
  {
    auto decompiler{ FilesystemMediaSetDecompiler::create() };
    assert( decompiler );

    // configure decompiler
    decompiler
      ->progressHandler( [&](
        std::string_view partNumber,
        std::pair< MediumNumber, MediumNumber > medium )
      {
        if ( loadProgressHandler )
        {
          loadProgressHandler(
            { mediaSetCounter, mediaSetsPaths.size() },
            partNumber,
            medium );
        }
      } )
      .checkFileIntegrity( checkFileIntegrity )
      .mediaPaths( absoluteMediaPaths( mediaSetPaths ) );

    // import media set
    auto [ impMediaSet, checkValues ]{ ( *decompiler )() };
    assert( impMediaSet );

    std::string partNumber{ impMediaSet->partNumber() };

    // add to media sets information
    mediaSetsInformationV.try_emplace(
      partNumber,
      std::move( impMediaSet ),
      std::move( checkValues ) );

    // add to media sets paths
    mediaSetsPathsV.try_emplace( partNumber, mediaSetPaths );

    // increment media set Index Counter
    ++mediaSetCounter;
  }
}

MediaPaths MediaSetManagerImpl::absoluteMediaPaths(
  const MediaSetPaths &mediaSetPaths ) const
{
  MediaPaths absoluteMediaPaths{};
  for ( const auto &[ mediumNumber, mediumPath ] : mediaSetPaths.second )
  {
    absoluteMediaPaths.try_emplace(
      mediumNumber,
      ( directoryV / mediaSetPaths.first / mediumPath ).lexically_normal() );
  }

  return absoluteMediaPaths;
}

}

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

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Arinc665Logger.hpp>

#include <arinc665/utils/MediaSetImporter.hpp>

#include <boost/format.hpp>

#include <fstream>

namespace Arinc665::Utils {

MediaSetManagerImpl::MediaSetManagerImpl(
  const std::filesystem::path &basePath,
  MediaSetConfiguration &config,
  const bool checkFileIntegrity ):
  basePath{ basePath },
  configurationV{ config }
{
  BOOST_LOG_FUNCTION()

  for ( auto const &mediaSet : config.mediaSets )
  {
    // NOTE: structured bindings cannot be passed as lambda capture at the moment
    // https://api.csswg.org/bikeshed/#lambda-captures

    // import media set
    auto importer( MediaSetImporter::create() );

    // the read file handler
    importer->readFileHandler(
      [this,&mediaSet](
        const uint8_t mediumNumber,
        const std::filesystem::path &path )->Files::RawFile
      {
        // make structure binding here instead
        const auto &[mediaSetPath,mediaPaths]{ mediaSet };

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
          (char*) &data.at( 0),
          static_cast< std::streamsize >( data.size() ) );

        // return the buffer
        return data;
      } );

    importer->checkFileIntegrity( checkFileIntegrity );

    // import media set
    auto impMediaSet( (*importer)() );

    // add media set
    mediaSetsV.push_back( impMediaSet );

    // iterate over media
    for ( auto &medium : impMediaSet->media() )
    {
      // add path mapping
      this->mediaPaths.insert( {
        medium.second,
        mediaSet.first / mediaSet.second.at( medium.first ) } ); // should never fail
    }
  }
}

const MediaSetConfiguration& MediaSetManagerImpl::configuration() const
{
  return configurationV;
}

Media::ConstMediaSetPtr MediaSetManagerImpl::mediaSet(
  std::string_view partNumber) const
{
  for ( const auto &mediaSet : mediaSetsV )
  {
    if ( mediaSet->partNumber() == partNumber )
    {
      return mediaSet;
    }
  }

  return {};
}

const MediaSetManagerImpl::MediaSets& MediaSetManagerImpl::mediaSets() const
{
  return mediaSetsV;
}

void MediaSetManagerImpl::add(
  Media::ConstMediaSetPtr mediaSet,
  MediumPathHandler mediumPathHandler )
{
  BOOST_LOG_FUNCTION()

  if ( !mediaSet || !mediumPathHandler )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
        << Helper::AdditionalInfo{ "Invalid Parameters" } );
  }

  // Media Set Base path
  auto mediaSetPath{ mediaSet->partNumber() };
  // List of Medium Paths
  MediaSetConfiguration::MediaPaths mediaSetMediaPaths{};

  // iterate over media
  for ( auto const &[mediumIndex, medium] : mediaSet->media() )
  {
    const auto sourcePath{ mediumPathHandler( medium ) };
    const auto destinationMediumPath{
      (boost::format( "MEDIUM_%03u" ) % (unsigned int)mediumIndex ).str() };

    const auto destinationPath{
      absolutePath( std::filesystem::path{ mediaSetPath } / destinationMediumPath ) };

    std::filesystem::copy(
      sourcePath,
      destinationPath,
      std::filesystem::copy_options::recursive );

    mediaPaths.emplace( medium, destinationPath );
    mediaSetMediaPaths.emplace( mediumIndex, destinationMediumPath );
  }

  mediaSetsV.emplace_back( mediaSet );

  configurationV.mediaSets.emplace_back(
    std::string{ mediaSetPath },
    std::move( mediaSetMediaPaths ) );
}

Media::ConstLoads MediaSetManagerImpl::loads() const
{
  Media::ConstLoads loads{};

  for ( const auto &mediaSet : mediaSetsV )
  {
    auto mediaSetLoads{ mediaSet->loads() };

    loads.insert( loads.end(), mediaSetLoads.begin(), mediaSetLoads.end() );
  }

  return loads;
}

Media::ConstLoads MediaSetManagerImpl::load( std::string_view filename ) const
{
  Media::ConstLoads loads{};

  for ( const auto &mediaSet : mediaSetsV )
  {
    auto mediaSetLoad{ mediaSet->load( filename ) };

    if ( mediaSetLoad )
    {
      loads.push_back( mediaSetLoad );
    }
  }

  return loads;
}

Media::ConstLoadPtr MediaSetManagerImpl::load(
  std::string_view partNumber,
  std::string_view filename ) const
{
  const auto mediaSetFound{ mediaSet( partNumber ) };

  if ( !mediaSetFound )
  {
    return {};
  }

  return mediaSetFound->load( filename );
}

std::filesystem::path MediaSetManagerImpl::filePath(
  Media::ConstFilePtr file ) const
{
  auto mediumIt{ mediaPaths.find( file->parent()->medium() ) };

  if ( mediumIt == mediaPaths.end() )
  {
    return {};
  }

  return absolutePath( mediumIt->second / file->path().relative_path() );
}

std::filesystem::path MediaSetManagerImpl::absolutePath(
  const std::filesystem::path &filePath ) const
{
  return ( configurationV.mediaSetsBase.is_relative() ?
      basePath / configurationV.mediaSetsBase / filePath :
      configurationV.mediaSetsBase / filePath ).lexically_normal();
}

}

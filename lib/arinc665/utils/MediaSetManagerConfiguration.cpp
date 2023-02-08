/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetManagerConfiguration.
 **/

#include "MediaSetManagerConfiguration.hpp"

#include <arinc665/Arinc665Logger.hpp>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Arinc665::Utils {

MediaSetManagerConfiguration::MediaSetManagerConfiguration(
  const boost::property_tree::ptree &properties )
{
  fromProperties( properties );
}

void MediaSetManagerConfiguration::fromProperties(
  const boost::property_tree::ptree &properties )
{
  BOOST_LOG_FUNCTION()

  // iterate over media sets
  for ( const auto &[ mediaSetPtreeName, mediaSetConfig ] : properties )
  {
    auto mediaSetPath{
      mediaSetConfig.get< std::filesystem::path >( "path", {} ) };

    MediaPaths mediaPaths{};

    // iterate over media
    if ( const auto mediaConfigs{ mediaSetConfig.get_child_optional( "media" ) };
         mediaConfigs )
    {
      for ( const auto &[ mediaPtreeName, mediumConfig ] : *mediaConfigs )
      {
        auto mediumNumber{
          mediumConfig.get< unsigned int >( "number" ) };
        auto mediumPath{
          mediumConfig.get< std::filesystem::path >( "path" ) };

        mediaPaths.try_emplace(
          MediumNumber{ static_cast< uint8_t >( mediumNumber ) },
          std::move( mediumPath ) );
      }
    }

    // insert media set configuration
    mediaSets.emplace_back( std::move( mediaSetPath ), std::move( mediaPaths ) );
  }
}

boost::property_tree::ptree MediaSetManagerConfiguration::toProperties() const
{
  boost::property_tree::ptree properties{};

  for ( const auto &[ mediaSetPath, media ] : mediaSets )
  {
    boost::property_tree::ptree mediaSetConfig{};

    mediaSetConfig.add( "path", mediaSetPath.string() );

    boost::property_tree::ptree mediaConfig{};

    for ( const auto &[ mediumNumber, mediumPath ] : media )
    {
      boost::property_tree::ptree mediumConfig{};

      mediumConfig.add(
        "number",
        static_cast< uint8_t >( mediumNumber ) );
      mediumConfig.add( "path", mediumPath.string() );

      mediaConfig.add_child( "medium", mediumConfig );
    }

    mediaSetConfig.add_child( "media", mediaConfig );

    properties.add_child( "media_set", mediaSetConfig );
  }

  return properties;
}

}

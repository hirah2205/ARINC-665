/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetConfiguration.
 **/

#include "MediaSetConfiguration.hpp"

#include <arinc665/Arinc665Logger.hpp>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Arinc665::Utils {

MediaSetConfiguration::MediaSetConfiguration(
  const boost::property_tree::ptree &ptree )
{
  fromProperties( ptree );
}

void MediaSetConfiguration::fromProperties(
  const boost::property_tree::ptree &ptree )
{
  BOOST_LOG_FUNCTION()

  mediaSetsBase = ptree.get< std::filesystem::path >(
    "media_sets_base",
    std::filesystem::current_path() );

  // iterate over media sets
  if ( const auto mediaSetsConfig{ ptree.get_child_optional( "media_sets" ) };
       mediaSetsConfig )
  {
    for ( auto &mediaSetConfig : *mediaSetsConfig )
    {
      auto mediaSetPath{
        mediaSetConfig.second.get< std::filesystem::path >( "path", {} ) };

      MediaPaths mediaPaths{};

      // iterate over media
      if ( const auto mediaConfigs{ mediaSetConfig.second.get_child_optional( "media" ) };
           mediaConfigs )
      {
        for ( auto &mediumConfig : *mediaConfigs )
        {
          auto mediumNumber{
            mediumConfig.second.get< unsigned int >( "number" ) };
          auto mediumPath{
            mediumConfig.second.get< std::filesystem::path >( "path" ) };

          mediaPaths.emplace(
            static_cast< uint8_t >( mediumNumber ),
            std::move( mediumPath ) );
        }
      }

      // insert media set configuration
      mediaSets.emplace_back(
        std::move( mediaSetPath ),
        std::move( mediaPaths ) );
    }
  }
}

boost::property_tree::ptree MediaSetConfiguration::toProperties() const
{
  boost::property_tree::ptree ptree{};

  ptree.add( "media_sets_base", mediaSetsBase);

  boost::property_tree::ptree mediaSetsConfig{};

  for ( const auto &mediaSet : mediaSets )
  {
    boost::property_tree::ptree mediaSetConfig{};

    mediaSetConfig.add( "path", mediaSet.first );

    boost::property_tree::ptree mediaConfig{};

    for ( const auto &[ mediumNumber, mediumPath] : mediaSet.second )
    {
      boost::property_tree::ptree mediumConfig{};

      mediumConfig.add( "number", mediumNumber );
      mediumConfig.add( "path", mediumPath );

      mediaConfig.add_child( "medium", mediumConfig );
    }

    mediaSetConfig.add_child( "media", mediaConfig );

    mediaSetsConfig.add_child( "media_set", mediaSetConfig );
  }

  ptree.add_child( "media_sets", mediaSetsConfig );

  return ptree;
}

}

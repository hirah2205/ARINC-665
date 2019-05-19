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
  const boost::property_tree::ptree &config)
{
  load( config);
}

void MediaSetConfiguration::load( const boost::property_tree::ptree &config)
{
  BOOST_LOG_FUNCTION()

  mediaSetBase = config.get< std::filesystem::path>(
    "media_set_base",
    std::filesystem::current_path());

  // iterate over media sets
  for ( auto &mediaSetConfig : config.get_child( "media_sets"))
  {
    auto mediaSetPartNumber{
      mediaSetConfig.second.get< std::string>( "part_number")};

    MediaPaths mediaPaths;

    // iterate over media
    for ( auto &mediumConfig : mediaSetConfig.second.get_child( "media"))
    {
      auto mediumNumber{ mediumConfig.second.get< unsigned int>( "number")};
      auto mediumPath{ mediumConfig.second.get< std::filesystem::path>( "path")};

      mediaPaths.emplace(
        static_cast< uint8_t>( mediumNumber),
        std::move( mediumPath));
    }

    // insert media set configuration
    mediaSets.emplace( std::move( mediaSetPartNumber), std::move( mediaPaths));
  }
}

}

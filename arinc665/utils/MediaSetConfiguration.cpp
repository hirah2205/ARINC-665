/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Utils::MediaSetConfiguration.
 **/

#include "MediaSetConfiguration.hpp"

#include <arinc665/Arinc665Logger.hpp>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Arinc665::Utils {

MediaSetConfiguration::MediaSetConfiguration( const ptree &config)
{
  load( config);
}

void MediaSetConfiguration::load( const ptree &config)
{
  BOOST_LOG_FUNCTION();

  mediaSetBase = config.get< path>(
    "media_set_base",
    boost::filesystem::current_path());

  // iterate over media sets
  for ( auto &mediaSetConfig : config.get_child( "media_sets"))
  {
    const auto mediaSetPartNumber{
      mediaSetConfig.second.get< std::string>( "part_number")};

    MediaPaths mediaPaths;

    // iterate over media
    for ( auto &mediumConfig : mediaSetConfig.second.get_child( "media"))
    {
      const auto mediumNumber{ mediumConfig.second.get< unsigned int>( "number")};
      const auto mediumPath{ mediumConfig.second.get< path>( "path")};

      mediaPaths.insert(
        { static_cast< uint8_t>( mediumNumber), mediumPath});
    }

    // insert media set configuration
    mediaSets.insert( { std::move( mediaSetPartNumber), std::move( mediaPaths)});
  }
}

}

/*
 * $Date$
 * $Revision$
 */
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

namespace Arinc665 {
namespace Utils {

MediaSetConfiguration::MediaSetConfiguration():
  mediaSetBase( boost::filesystem::current_path())
{
}

MediaSetConfiguration::MediaSetConfiguration( const ptree &config):
  mediaSetBase( config.get< path>( "media_set_base",  boost::filesystem::current_path()))
{
  BOOST_LOG_FUNCTION();

  // iterate over media sets
  for ( auto &mediaSetConfig : config.get_child( "media_sets"))
  {
    auto mediaSetName( mediaSetConfig.second.get< string>( "name"));

    MediaPaths mediaPaths;

    // iterate over media
    for ( auto &mediumConfig : mediaSetConfig.second.get_child( "media"))
    {
      mediaPaths.push_back( mediumConfig.second.get_value< path>());
    }

    // insert media set configuration
    mediaSets.insert( { std::move( mediaSetName), std::move( mediaPaths)});
  }
}

}
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetManager.
 **/

#include "MediaSetManager.hpp"

#include <arinc665/utils/implementation/MediaSetManagerImpl.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Exception.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Utils {

MediaSetManagerPtr MediaSetManager::create( std::filesystem::path directory )
{
  if ( std::filesystem::exists( directory )
    && ( !std::filesystem::is_directory( directory )
      || !std::filesystem::is_empty( directory ) ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "Path exist or is not empty" }
      << boost::errinfo_file_name{ directory.string() } );
  }

  std::filesystem::create_directories( directory );

  Arinc665::Utils::MediaSetManagerConfiguration configuration{};

  boost::property_tree::ptree configurationPTree{ configuration.toProperties() };

  boost::property_tree::write_json(
    ( directory / ConfigurationFilename ).string(),
    configurationPTree );

  return std::make_shared< MediaSetManagerImpl >(
    std::move( directory ),
    false );
}

MediaSetManagerPtr MediaSetManager::load(
  std::filesystem::path directory,
  const bool checkFileIntegrity )
{
  if ( !std::filesystem::exists( directory ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "Media Set Manger Directory must exists" }
      << boost::errinfo_file_name{ directory.string() } );
  }

  return std::make_shared< MediaSetManagerImpl >(
    std::move( directory ),
    checkFileIntegrity );
}

MediaSetManagerPtr MediaSetManager::loadOrCreate(
  std::filesystem::path directory,
  const bool checkFileIntegrity )
{
  if ( !std::filesystem::exists( directory )
    || ( std::filesystem::is_directory( directory )
      && std::filesystem::is_empty( directory ) ) )
  {
    return create( std::move( directory ) );
  }

  return std::make_shared< MediaSetManagerImpl >(
    std::move( directory ),
    checkFileIntegrity );
}

}

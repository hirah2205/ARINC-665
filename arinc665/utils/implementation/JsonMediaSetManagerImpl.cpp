/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#include "JsonMediaSetManagerImpl.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Arinc665::Utils {

void JsonMediaSetManagerImpl::create( const std::filesystem::path &directory )
{
  if ( std::filesystem::exists( directory ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "Media Set Manger Directory must not exists" } );
  }

  std::filesystem::create_directories( directory );

  Arinc665::Utils::MediaSetManagerConfiguration configuration{};

  boost::property_tree::ptree configurationPTree{ configuration.toProperties() };

  boost::property_tree::write_json(
    directory / ConfigurationFilename,
    configurationPTree );
}

JsonMediaSetManagerPtr JsonMediaSetManagerImpl::loadOrCreate(
  const std::filesystem::path &directory,
  const bool checkFileIntegrity )
{
  if ( !std::filesystem::exists( directory ) )
  {
    create( directory );
  }

  return std::make_shared< JsonMediaSetManagerImpl >(
    directory,
    checkFileIntegrity );
}

JsonMediaSetManagerImpl::JsonMediaSetManagerImpl(
  const std::filesystem::path &directory,
  bool checkFileIntegrity ) :
  directoryV{ directory }
{
  const auto configurationFile{ directory / ConfigurationFilename };

  if ( !std::filesystem::is_regular_file( configurationFile ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception{}
      << Helper::AdditionalInfo{ "Media Set Configuration file does not exists" } );
  }

  boost::property_tree::ptree configurationPTree{};

  boost::property_tree::json_parser::read_json(
    configurationFile,
    configurationPTree );

  mediaSetManagerV = Arinc665::Utils::MediaSetManager::instance(
    directory,
    Arinc665::Utils::MediaSetManagerConfiguration{ configurationPTree },
    checkFileIntegrity );
}

MediaSetManagerPtr JsonMediaSetManagerImpl::manager()
{
  assert( mediaSetManagerV );

  return mediaSetManagerV;
}

void JsonMediaSetManagerImpl::saveConfiguration()
{
  assert( mediaSetManagerV );

  boost::property_tree::write_json(
    directoryV / ConfigurationFilename,
    mediaSetManagerV->configuration().toProperties() );
}

const std::filesystem::path& JsonMediaSetManagerImpl::directory()
{
  return directoryV;
}

}

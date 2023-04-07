/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetDefaults.
 **/

#include "MediaSetDefaults.hpp"

#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <arinc665/Arinc665Logger.hpp>
#include <arinc665/SupportedArinc665VersionDescription.hpp>

#include <boost/property_tree/ptree.hpp>

namespace Arinc665::Utils {

MediaSetDefaults::MediaSetDefaults(
  const boost::property_tree::ptree &properties )
{
  fromProperties( properties );
}

void MediaSetDefaults::fromProperties(
  const boost::property_tree::ptree &properties )
{
  BOOST_LOG_FUNCTION()

  checkFileIntegrity =
    properties.get( "check_file_integrity", DefaultCheckFileIntegrity );

  version = properties.get(
    "version",
    SupportedArinc665Version::Supplement345 );

  loadHeaderFileCreationPolicy = properties.get(
    "load_header_file_creation_policy",
    FileCreationPolicy::NoneExisting );
  batchFileCreationPolicy = properties.get(
    "batch_file_creation_policy",
    FileCreationPolicy::NoneExisting );
}

boost::property_tree::ptree MediaSetDefaults::toProperties() const
{
  boost::property_tree::ptree properties{};

  properties.add( "check_file_integrity", checkFileIntegrity );
  properties.add( "version", version );
  properties.add( "header_file_creation_policy", loadHeaderFileCreationPolicy );
  properties.add( "batch_file_creation_policy", batchFileCreationPolicy );

  return properties;
}

}

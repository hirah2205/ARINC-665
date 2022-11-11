/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetManagerConfiguration.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGERCONFIGURATION_HPP
#define ARINC665_UTILS_MEDIASETMANAGERCONFIGURATION_HPP

#include <arinc665/utils/Utils.hpp>

#include <boost/property_tree/ptree_fwd.hpp>

#include <filesystem>
#include <list>
#include <utility>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Manager Configuration.
 **/
class ARINC665_EXPORT MediaSetManagerConfiguration
{
  public:
    //! media-to-path mapping (Medium Number -> Path)
    using MediaPaths = std::map< uint8_t, std::filesystem::path > ;
    //! Media Set Paths (Media Set Base Path + Media Paths)
    using MediaSetPaths = std::pair< std::filesystem::path, MediaPaths >;
    //! Media Sets Paths ( List of Media Set Path, @ref MediaPaths)
    using MediaSetsPaths = std::list< MediaSetPaths >;

    //! Initialises the configuration with default values.
    MediaSetManagerConfiguration() = default;

    /**
     * @brief Loads the configuration via a boost::property_tree::ptree.
     *
     * @param[in] properties
     *   Stored configuration.
     **/
    explicit MediaSetManagerConfiguration(
      const boost::property_tree::ptree &properties );

    /**
     * @brief Loads the media set configuration form the given property tree.
     *
     * @param[in] properties
     *   Stored Media Set Manager Configuration.
     **/
    void fromProperties( const boost::property_tree::ptree &properties );

    /**
     * @brief Converts the configuration values to a
     *   boost::property_tree::ptree.
     *
     * @return Configuration as boost::property_tree::ptree.
     **/
    [[nodiscard]] boost::property_tree::ptree toProperties() const;

    //! Media Sets Paths
    MediaSetsPaths mediaSets;
};

}

#endif

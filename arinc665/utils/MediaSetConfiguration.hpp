/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetConfiguration.
 **/

#ifndef ARINC665_UTILS_MEDIASETCONFIGURATION_HPP
#define ARINC665_UTILS_MEDIASETCONFIGURATION_HPP

#include <arinc665/utils/Utils.hpp>

#include <boost/property_tree/ptree_fwd.hpp>

#include <filesystem>
#include <map>
#include <list>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Configuration.
 **/
class MediaSetConfiguration
{
  public:
    //! media-to-path mapping (Medium Number -> Path)
    using MediaPaths = std::map< uint8_t, std::filesystem::path>;
    //! List of Media Sets (Media Set Path, Media Paths)
    using MediaSets = std::list< std::pair< std::string, MediaPaths > >;

    //! Initialises the configuration with default values.
    MediaSetConfiguration() = default;

    /**
     * @brief Loads the configuration via a boost::property_tree::ptree.
     *
     * @param[in] ptree
     *   The stored configuration.
     **/
    explicit MediaSetConfiguration(
      const boost::property_tree::ptree &ptree );

    /**
     * @brief Loads the media set configuration form the given property tree.
     *
     * @param[in] ptree
     *   The ptree to fromProperties the config from
     **/
    void fromProperties( const boost::property_tree::ptree &ptree );

    /**
     * @brief Converts the configuration values to a
     *   boost::property_tree::ptree.
     *
     * @return The boost::property_tree::ptree.
     **/
    [[nodiscard]] boost::property_tree::ptree toProperties() const;

    //! Base directory for all media sets stored.
    std::filesystem::path mediaSetsBase;
    //! Media Sets
    MediaSets mediaSets;
};

}

#endif

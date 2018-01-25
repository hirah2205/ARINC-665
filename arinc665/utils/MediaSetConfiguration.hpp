/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Utils::MediaSetConfiguration.
 **/

#ifndef ARINC665_UTILS_MEDIASETCONFIGURATION_HPP
#define ARINC665_UTILS_MEDIASETCONFIGURATION_HPP

#include <arinc665/utils/Utils.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <map>

namespace Arinc665 {
namespace Utils {

/**
 * @brief Media Set storage configuration.
 **/
class MediaSetConfiguration
{
  public:
    //! property tree type
    using ptree = boost::property_tree::ptree;
    //! path type
    using path = boost::filesystem::path;
    //! string type
    using string = std::string;

    //! media-to-path mapping (medium number -> path)
    using MediaPaths = std::map< uint8_t, path>;
    //! Mapping of media sets (Media set part number -> media paths)
    using MediaSets = std::map< string, MediaPaths>;

    //! Initialises the configuration with default values.
    MediaSetConfiguration() = default;

    /**
     * @brief Loads the configuration via a boost::property_tree::ptree.
     *
     * @param[in] config
     *   The stored configuration.
     **/
    MediaSetConfiguration( const ptree &config);

    /**
     * @brief Loads the media set configuration form the given property tree.
     *
     * @param[in] config
     *   The ptree to load the config from
     **/
    void load( const ptree &config);

    /**
     * @brief Converts the configuration values to a
     *   boost::property_tree::ptree.
     *
     * @return The boost::property_tree::ptree.
     **/
    ptree toProperties() const;

    //! Base directory for all media sets stored.
    path mediaSetBase;
    //! The list of media sets (name-path-mapping)
    MediaSets mediaSets;
};

}
}

#endif

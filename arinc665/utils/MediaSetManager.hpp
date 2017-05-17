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
 * @brief Declaration of class Arinc665::Utils::MediaSetManager.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGER_HPP
#define ARINC665_UTILS_MEDIASETMANAGER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <list>
#include <string>

namespace Arinc665 {
namespace Utils {

/**
 * @brief This class manages the locally held media sets.
 **/
class MediaSetManager
{
  public:
    using path = boost::filesystem::path;

    using string = std::string;

    using MediaSetList = std::list< Media::MediaSetPtr>;

    //! Default destructor
    virtual ~MediaSetManager() noexcept = default;

    /**
     * @brief Creates the Media Set Manager instance with the given
     *   configuration.
     *
     * @param[in] config
     *   The media set manager configuration.
     * @param[in] mediaSetBase
     *   If media paths are relative, use this as base.
     * @return The media set manager instance.
     **/
    static MediaSetManagerPtr createInstance(
      const boost::property_tree::ptree &config,
      const path &mediaSetBase = {});

    /**
     * @brief Returns the media set with the given part number.
     *
     * @param[in] partNumber
     *   The media set part number.
     *
     * @return The media set with the given part number.
     **/
    virtual Media::MediaSetPtr getMediaSet( const string &partNumber) = 0;

    /**
     * @brief Returns all registered media sets.
     *
     * @return All media sets.
     **/
    virtual const MediaSetList& getMediaSets() const = 0;

    //! @copydoc MediaSetManager::getMediaSets() const
    virtual MediaSetList& getMediaSets() = 0;

    /**
     * @brief Get all available Loads.
     *
     * @return All available loads.
     **/
    virtual Media::ConstLoads getLoads() const = 0;

    /**
     * @brief Returns the path to the given file.
     *
     * @param[in] file
     *   File to request the path from
     *
     * @return The path to the given file.
     **/
    virtual path getFilePath( Media::ConstBaseFilePtr file) const = 0;
};

}
}

#endif

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

#include <filesystem>
#include <list>
#include <string>
#include <functional>

namespace Arinc665::Utils {

/**
 * @brief This class manages the locally held media sets.
 *
 * @par Organisation of Media Set Manager data.
 * * Media sets are stored beneath the [mediaSetBasePath] directory.
 * * Within this directory the media sets each are stored within a directory
 *   named [mediaSetName].
 * * Within the media set directory the media are stored with the corresponding
 *   medium-path mapping.
 **/
class MediaSetManager
{
  public:
    //! Media sets type (list)
    using MediaSets = std::list< Media::MediaSetPtr>;
    //! Handler which returns the path to the given medium number
    using MediumPathHandler =
      std::function< std::filesystem::path( Media::ConstMediumPtr medium)>;

    //! Default destructor
    virtual ~MediaSetManager() noexcept = default;

    /**
     * @brief Creates the Media Set Manager instance with the given
     *   configuration.
     *
     * @param[in] config
     *   The media set manager configuration.
     *
     * @return The media set manager instance.
     **/
    static MediaSetManagerPtr instance(
      const MediaSetConfiguration &config);

    /**
     * @brief Returns the configuration for the media set manager.
     *
     * @return The media set manager configuration.
     **/
    virtual const MediaSetConfiguration& configuration() const = 0;

    /**
     * @brief Returns the media set with the given part number.
     *
     * @param[in] partNumber
     *   The media set part number.
     *
     * @return The media set with the given part number.
     **/
    virtual Media::MediaSetPtr mediaSet( const std::string &partNumber) = 0;

    /**
     * @brief Returns all registered media sets.
     *
     * @return All media sets.
     **/
    virtual const MediaSets& mediaSets() const = 0;

    //! @copydoc MediaSetManager::mediaSets() const
    virtual MediaSets& mediaSets() = 0;

    /**
     * @brief Adds the given media set to the media set manager.
     *
     * Copies the media from the source path to its destination paths.
     * The integrity of the media paths are not checked.
     * To few files will be detected on trying to access them.
     * To much files are not detected.
     *
     * @param[in] mediaSet
     * @param[in] mediumPathHandler
     *
     * @throw Arinc665Exception
     *   If media set with this name already exists.
     * @throw Arinc665Exception
     *   If media/ files cannot be created/ copied.
     **/
    virtual void add(
      Media::ConstMediaSetPtr mediaSet,
      MediumPathHandler mediumPathHandler) = 0;

    /**
     * @brief Get all available Loads.
     *
     * @return All available loads.
     **/
    virtual Media::ConstLoads loads() const = 0;

    /**
     * @brief Returns the loads with the given filename.
     *
     * THis operation returns a list of loads, because the specific load could
     * be localised on more than one medium.
     *
     * @param[in] filename
     *   The filename
     *
     * @return The loads with the given filename.
     **/
    virtual Media::ConstLoads load( const std::string &filename) const = 0;

    /**
     * @brief Returns the path to the given file.
     *
     * @param[in] file
     *   File to request the path from
     *
     * @return The path to the given file.
     **/
    virtual std::filesystem::path filePath( Media::ConstBaseFilePtr file) const = 0;
};

}

#endif

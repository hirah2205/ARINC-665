/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetManager.
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
 * @brief ARINC 665 %Media Set Manager.
 *
 * This class manages the locally held media sets.
 *
 * @par Organisation of Media Set Manager data.
 * - Media sets are stored beneath the [mediaSetBasePath] directory.
 * - Within this directory the media sets each are stored within a directory
 *   named @p mediaSetName.
 * - Within the media set directory the media are stored with the corresponding
 *   medium-path mapping.
 **/
class MediaSetManager
{
  public:
    //! Media Sets (list)
    using MediaSets = std::list< Media::ConstMediaSetPtr >;

    //! Handler which returns the path to the given medium number
    using MediumPathHandler =
      std::function< std::filesystem::path( Media::ConstMediumPtr medium ) >;

    //! Destructor
    virtual ~MediaSetManager() noexcept = default;

    /**
     * @brief Creates the Media Set Manager instance with the given
     *   configuration.
     *
     * @param[in] basePath
     *   Base Path to use, when configured paths are relative, i.e. base of
     *   configuration file.
     * @param[in,out] config
     *   Media Set Manager Configuration.
     *
     * @return Media Set Manager Instance.
     **/
    static MediaSetManagerPtr instance(
      const std::filesystem::path &basePath,
      MediaSetConfiguration &config );

    /**
     * @brief Returns the configuration for the media set manager.
     *
     * @return Media Set Manager Configuration.
     **/
    [[nodiscard]] virtual const MediaSetConfiguration& configuration() const = 0;

    /**
     * @brief Returns the Media Set with the given Part Number.
     *
     * @param[in] partNumber
     *   Media Set Part Number.
     *
     * @return Media Set with the given Part Number.
     **/
    [[nodiscard]] virtual Media::ConstMediaSetPtr mediaSet(
      std::string_view partNumber ) const = 0;

    /**
     * @brief Returns all registered media sets.
     *
     * @return All media sets.
     **/
    [[nodiscard]] virtual const MediaSets& mediaSets() const = 0;

    /**
     * @brief Adds the given media set to the media set manager.
     *
     * Copies the media from the source path to its destination paths.
     * The integrity of the media paths are not checked.
     * Missing files will be detected on trying to access them.
     * To much files are not detected.
     *
     * @param[in] mediaSet
     *   Media Set to add.
     * @param[in] mediumPathHandler
     *   Medium Path Handler for Import.
     *
     * @throw Arinc665Exception
     *   If media set with this name already exists.
     * @throw Arinc665Exception
     *   If media/ files cannot be created/ copied.
     **/
    virtual void add(
      Media::ConstMediaSetPtr mediaSet,
      MediumPathHandler mediumPathHandler ) = 0;

    /**
     * @brief Get all available Loads from all Media Sets.
     *
     * @return All available loads.
     **/
    [[nodiscard]] virtual Media::ConstLoads loads() const = 0;

    /**
     * @brief Returns the loads with the given filename.
     *
     * This operation returns a list of loads, because the specific load could
     * be localised on more than one media set.
     *
     * @param[in] filename
     *   Load Filename
     *
     * @return The loads with the given filename.
     **/
    [[nodiscard]] virtual Media::ConstLoads load(
      std::string_view filename ) const = 0;

    /**
     * @brief Returns the loads with the given filename from the given Media Set.
     *
     * @param[in] partNumber
     *   Media Set Part Number.
     * @param[in] filename
     *   Load Filename
     *
     * @return The loads with the given filename.
     **/
    [[nodiscard]] virtual Media::ConstLoadPtr load(
      std::string_view partNumber,
      std::string_view filename ) const = 0;

    /**
     * @brief Returns the path to the given file.
     *
     * @param[in] file
     *   File to request the path from
     *
     * @return The path to the given file.
     **/
    [[nodiscard]] virtual std::filesystem::path filePath(
      Media::ConstBaseFilePtr file ) const = 0;
};

}

#endif

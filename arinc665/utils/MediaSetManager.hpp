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

#include <arinc665/media/Media.hpp>

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>

#include <arinc645/CheckValue.hpp>

#include <filesystem>
#include <list>
#include <string>
#include <functional>
#include <optional>
#include <map>

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
class ARINC665_EXPORT MediaSetManager
{
  public:
    //! Check Values
    using CheckValues =
      std::map< Media::ConstFilePtr, Arinc645::CheckValue, std::less< > >;
    //! Media Set Information
    using MediaSet =
      std::pair< Media::ConstMediaSetPtr, CheckValues >;
    //! Media Sets Map ( Part Number -> Media Set)
    using MediaSets =
      std::map< std::string, MediaSet, std::less<> >;

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
     * @param[in,out] configuration
     *   Media Set Manager Configuration.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed, when
     *   loading the Media Set Manager.
     *
     * @return Media Set Manager Instance.
     **/
    static MediaSetManagerPtr instance(
      const std::filesystem::path &basePath,
      const MediaSetManagerConfiguration &configuration,
      bool checkFileIntegrity = true );

    /**
     * @brief Returns the configuration for the media set manager.
     *
     * @return Media Set Manager Configuration.
     **/
    [[nodiscard]] virtual const MediaSetManagerConfiguration&
    configuration() const = 0;

    /**
     * @brief Returns the Media Set with the given Part Number.
     *
     * @param[in] partNumber
     *   Media Set Part Number.
     *
     * @return Media Set with the given Part Number.
     * @retval {}
     *   If no Media Set with partNumber exist.
     **/
    [[nodiscard]] virtual std::optional< MediaSet > mediaSet(
      std::string_view partNumber ) const = 0;

    /**
     * @brief Returns all registered media sets.
     *
     * @return All media sets.
     **/
    [[nodiscard]] virtual const MediaSets& mediaSets() const = 0;

    /**
     * @brief Registers the Media Set by the Media Set Manager.
     *
     * Reads the Media Set and imports them to the Media Set Manager.
     *
     * @param[in] mediaSetPaths
     *   Media Set Path Configuration
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed, when
     *   loading the Media Set Manager.
     **/
    virtual void registerMediaSet(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
      bool checkFileIntegrity = true ) = 0;

    /**
     * @brief De-registers the Media Set from the Media Set Manager.
     *
     * The Media Set data is not deleted from the disk.
     *
     * @param[in] partNumber
     *   Part Number of Media Set
     *
     * @return Media Set Path Configuration (for removal)
     **/
    virtual MediaSetManagerConfiguration::MediaSetPaths deregisterMediaSet(
      std::string_view partNumber ) = 0;

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
     * @return Loads with the given filename.
     **/
    [[nodiscard]] virtual Media::ConstLoads loads(
      std::string_view filename ) const = 0;

    /**
     * @brief Returns the loads with the given filename from the given Media Set.
     *
     * @param[in] partNumber
     *   Media Set Part Number.
     * @param[in] filename
     *   Load Filename
     *
     * @return Load with the given filename.
     **/
    [[nodiscard]] virtual Media::ConstLoads loads(
      std::string_view partNumber,
      std::string_view filename ) const = 0;

    /**
     * @brief Returns the path to the given file.
     *
     * @param[in] file
     *   File to request the path from
     *
     * @return Path to the given file.
     **/
    [[nodiscard]] virtual std::filesystem::path filePath(
      const Media::ConstFilePtr &file ) const = 0;
};

}

#endif

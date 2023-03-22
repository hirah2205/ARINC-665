/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetManagerImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP
#define ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetManagerConfiguration.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc645/CheckValue.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of MediaSetManager.
 *
 * Uses the FilesystemMediaSetManager to import the Media Sets from the disk.
 **/
class MediaSetManagerImpl final : public MediaSetManager
{
  public:
    /**
     * @brief Initialises the media set manager.
     *
     * @param[in] basePath
     *   Base Path to use, when configured paths are relative, i.e. base of
     *   configuration file.
     * @param[in] configuration
     *   Media Set Manager Configuration.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    explicit MediaSetManagerImpl(
      std::filesystem::path basePath,
      const MediaSetManagerConfiguration &configuration,
      bool checkFileIntegrity );

    //! @copydoc MediaSetManager::configuration
    [[nodiscard]] MediaSetManagerConfiguration configuration() const override;

    //! @copydoc MediaSetManager::hasMediaSet(std::string_view) const
    [[nodiscard]] bool hasMediaSet(
      std::string_view partNumber ) const override;

    //! @copydoc MediaSetManager::mediaSet(std::string_view) const
    [[nodiscard]] std::optional< MediaSetInformation > mediaSet(
      std::string_view partNumber ) const override;

    //! @copydoc MediaSetManager::mediaSets() const
    [[nodiscard]] const MediaSetsInformation& mediaSets() const override;

    //! @copydoc MediaSetManager::registerMediaSet()
    void registerMediaSet(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
      bool checkFileIntegrity = true ) override;

    //! @copydoc MediaSetManager::deregisterMediaSet()
    [[nodiscard]] MediaSetManagerConfiguration::MediaSetPaths
    deregisterMediaSet( std::string_view partNumber ) override;

    //! @copydoc MediaSetManager::loads() const
    [[nodiscard]] Media::ConstLoads loads() const override;

    //! @copydoc MediaSetManager::filePath
    [[nodiscard]] std::filesystem::path filePath(
      const Media::ConstFilePtr &file ) const override;

  private:
    /**
     * @brief Load Media Sets.
     *
     * @param[in] configuration
     *   Media Set Manager Configuration.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    void loadMediaSets(
      const MediaSetManagerConfiguration &configuration,
      bool checkFileIntegrity );

    /**
     * @brief Converts the given Media Set Paths to absolute Media Paths.
     *
     * Uses the base path, media set path and media paths to convert it to
     * absolute paths.
     *
     * @param[in] mediaSetPaths
     *   Media Set Paths information.
     *
     * @return Absolute Media Paths.
     **/
    [[nodiscard]] MediaPaths absoluteMediaPaths(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths ) const;

    //! Media Set Paths Map ( Part Number -> Media Set Paths )
    using MediaSetsPaths = std::map<
      std::string,
      MediaSetManagerConfiguration::MediaSetPaths,
      std::less< > >;

    //! Base for Relative Paths
    const std::filesystem::path basePathV;
    //! Media Sets Information
    MediaSetsInformation mediaSetsInformationV;
    //! Media Sets Paths
    MediaSetsPaths mediaSetsPathsV;
};

}

#endif

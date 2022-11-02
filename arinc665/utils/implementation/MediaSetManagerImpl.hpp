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

//! Implementation of MediaSetManager
class MediaSetManagerImpl final : public MediaSetManager
{
  public:
    /**
     * @brief Initialises the media set manager.
     *
     * @param[in] basePath
     *   Base Path to use, when configured paths are relative, i.e. base of
     *   configuration file.
     * @param[in,out] configuration
     *   Media Set Manager Configuration.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    explicit MediaSetManagerImpl(
      std::filesystem::path basePath,
      MediaSetManagerConfiguration configuration,
      bool checkFileIntegrity );

    //! @copydoc MediaSetManager::configuration
    [[nodiscard]] const MediaSetManagerConfiguration&
    configuration() const override;

    //! @copydoc MediaSetManager::mediaSet(std::string_view) const
    [[nodiscard]] std::optional< MediaSet > mediaSet(
      std::string_view partNumber ) const override;

    //! @copydoc MediaSetManager::mediaSets() const
    [[nodiscard]] const MediaSets& mediaSets() const override;

    //! @copydoc MediaSetManager::registerMediaSet()
    void registerMediaSet(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
      bool checkFileIntegrity = true ) override;

    //! @copydoc MediaSetManager::deregisterMediaSet()
    [[nodiscard]] MediaSetManagerConfiguration::MediaSetPaths
    deregisterMediaSet(
      std::string_view partNumber ) override;

    //! @copydoc MediaSetManager::loads() const
    [[nodiscard]] Media::ConstLoads loads() const override;

    //! @copydoc MediaSetManager::loads(std::string_view) const
    [[nodiscard]] Media::ConstLoads loads(
      std::string_view filename ) const override;

    //! @copydoc MediaSetManager::loads(std::string_view,std::string_view) const
    [[nodiscard]] Media::ConstLoads loads(
      std::string_view partNumber,
      std::string_view filename ) const override;

    //! @copydoc MediaSetManager::filePath
    [[nodiscard]] std::filesystem::path filePath(
      const Media::ConstFilePtr &file ) const override;

  private:
    /**
     * @brief Load Media Sets.
     *
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    void loadMediaSets( bool checkFileIntegrity );

    /**
     * @brief Handler Called for files sizes
     *
     * Used by the Media Set Importer.
     *
     * @param[in] mediaSetPaths
     *   Media Set Path Configuration.
     * @param[in] mediumNumber
     *   Medium Sequence Number.
     * @param[in] path
     *   File Path.
     *
     * @return File size
     **/
    [[nodiscard]] size_t fileSizeHandler(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
      uint8_t mediumNumber,
      const std::filesystem::path &path ) const;

    /**
     * @brief Handler Called for reading Files
     *
     * Used by the Media Set Importer.
     *
     * @param[in] mediaSetPaths
     *   Media Set Path Configuration.
     * @param[in] mediumNumber
     *   Medium Sequence Number.
     * @param[in] path
     *   File Path.
     *
     * @return Read File Content
     **/
    [[nodiscard]] Files::RawFile readFileHandler(
      const MediaSetManagerConfiguration::MediaSetPaths &mediaSetPaths,
      uint8_t mediumNumber,
      const std::filesystem::path &path ) const;

    /**
     * @brief Makes given @p filePath absolute with respect to media set base
     *   path.
     *
     * @param[in] filePath
     *   Relative file path.
     *
     * @return Absolute path of @p filePath.
     **/
    [[nodiscard]] std::filesystem::path absolutePath(
      const std::filesystem::path &filePath ) const;

    //! Media Set Paths Map
    using MediaSetsPaths = std::map<
      Media::ConstMediaSetPtr,
      MediaSetManagerConfiguration::MediaSetPaths >;

    //! Base for Relative Paths
    const std::filesystem::path basePath;
    //! Media Set Manager Configuration
    MediaSetManagerConfiguration configurationV;
    //! Media Sets
    MediaSets mediaSetsV;
    //! Media Sets Paths
    MediaSetsPaths mediaSetsPathsV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::JsonMediaSetManager
 **/

#ifndef ARINC665_UTILS_JSONMEDIASETMANAGER_HPP
#define ARINC665_UTILS_JSONMEDIASETMANAGER_HPP

#include <arinc665/utils/Utils.hpp>

#include <string_view>
#include <filesystem>

namespace Arinc665::Utils {

/**
 * @brief Media Set Manager JSON Helper
 *
 * This manager holds the media st manager configuration within a JSON file
 * within a separate directory.
 **/
class ARINC665_EXPORT JsonMediaSetManager
{
  public:
    //! Media Set Manager Configuration Filename
    static constexpr std::string_view ConfigurationFilename{
      "MediaSetManager.json" };

    /**
     * @brief Creates an empty Media Set Manager (but don't load it)
     * @param[in] directory
     *   Directory for Media Set Manger.
     *   Must not exist.
     **/
    static void create( const std::filesystem::path &directory );

    /**
     * @brief Checks if a Media Set Manager Configuration is available or creates
     *   it.
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity checks are performed
     *
     * @return Media Set Manager
     **/
    static JsonMediaSetManagerPtr loadOrCreate(
      const std::filesystem::path &directory,
      bool checkFileIntegrity = true );

    /**
     * @brief
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity checks are performed
     *
     * @return JSON Media Set Manager Instance.
     **/
    [[nodiscard]] static JsonMediaSetManagerPtr load(
      const std::filesystem::path &directory,
      bool checkFileIntegrity = true );

    //! Destructor
    virtual ~JsonMediaSetManager() = default;

    /**
     * @brief Returns the Media Set Manager
     *
     * @return Media Set Manager
     **/
    [[nodiscard]] virtual MediaSetManagerPtr manager() = 0;

    /**
     * @brief Persist the Configuration.
     **/
    virtual void saveConfiguration() = 0;

    /**
     * @brief Returns the Media Set Manager Directory
     *
     * @return Media Set Manager Directory.
     **/
    [[nodiscard]] virtual const std::filesystem::path& directory() = 0;
};

}

#endif

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

//! Media Set Manager JSON Helper
class JsonMediaSetManager
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
     * @brief
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     *
     * @return JSON Media Set Manager Instance.
     **/
    [[nodiscard]] static JsonMediaSetManagerPtr load(
      const std::filesystem::path &directory,
      bool checkFileIntegrity = true );

    virtual ~JsonMediaSetManager() = default;

    [[nodiscard]] virtual MediaSetManagerPtr manager() = 0;

    virtual void saveConfiguration() = 0;

    [[nodiscard]] virtual const std::filesystem::path& directory() = 0;
};

}

#endif

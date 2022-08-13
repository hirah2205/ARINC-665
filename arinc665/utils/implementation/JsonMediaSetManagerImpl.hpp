/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::JsonMediaSetManagerImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_JSONMEDIASETMANAGERIMPL_HPP
#define ARINC665_UTILS_IMPLEMENTATION_JSONMEDIASETMANAGERIMPL_HPP

#include <arinc665/utils/JsonMediaSetManager.hpp>

namespace Arinc665::Utils {

//! Implementation of JsonMediaSetManager
class JsonMediaSetManagerImpl : public JsonMediaSetManager
{
  public:
    /**
     * @brief Creates an empty Media Set Manager (but don't load it)
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     **/
    static void create( const std::filesystem::path &directory );

    /**
     * @brief Checks if a Media Set Manager Configuration is available or creates
     *   it.
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     *
     * @return Media Set Manager
     **/
    [[nodiscard]] static JsonMediaSetManagerPtr loadOrCreate(
      const std::filesystem::path &directory,
      bool checkFileIntegrity );

    /**
     * @brief Instantiates the Media Set Manager.
     *
     * Loads the Configuration and instantiates Media Set Manager.
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    JsonMediaSetManagerImpl(
      const std::filesystem::path &directory,
      bool checkFileIntegrity );

    //! @copydoc JsonMediaSetManager::manager()
    [[nodiscard]] MediaSetManagerPtr manager() final;

    //! @copydoc JsonMediaSetManager::saveConfiguration()
    void saveConfiguration() final;

    //! @copydoc JsonMediaSetManager::directory()
    [[nodiscard]] const std::filesystem::path& directory() final;

  private:
    //! Configuration Directory
    const std::filesystem::path directoryV;
    //! Media Set Manager Instance
    MediaSetManagerPtr mediaSetManagerV;
};

}
#endif

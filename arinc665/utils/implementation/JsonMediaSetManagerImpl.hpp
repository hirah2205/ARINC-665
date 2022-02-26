/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_JSONMEDIASETMANAGERIMPL_HPP
#define ARINC665_UTILS_IMPLEMENTATION_JSONMEDIASETMANAGERIMPL_HPP

#include <arinc665/utils/JsonMediaSetManager.hpp>

namespace Arinc665::Utils {

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
     * @brief Instantiates the Media Set Manager.
     *
     * Loads the Configuration and instantiates Media Set Manager.
     *
     * @param[in] directory
     *   Directory for Media Set Manger.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     **/
    explicit JsonMediaSetManagerImpl(
      const std::filesystem::path &directory ,
      bool checkFileIntegrity );

    MediaSetManagerPtr manager() final;

    void saveConfiguration() final;

    [[nodiscard]] const std::filesystem::path& directory() final;

  private:

    //! Configuration Directory
    const std::filesystem::path directoryV;
    //! Media Set Manager Instance
    MediaSetManagerPtr mediaSetManagerV;
};

}
#endif

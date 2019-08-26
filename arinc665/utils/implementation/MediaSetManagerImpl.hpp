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
#include <arinc665/utils/MediaSetConfiguration.hpp>

namespace Arinc665::Utils {

//! Implementation of MediaSetManager
class MediaSetManagerImpl : public MediaSetManager
{
  public:
    /**
     * @brief Initialises the media set manager.
     *
     * @param[in] config
     *   Media set configuration.
     **/
    explicit MediaSetManagerImpl( const MediaSetConfiguration &config);

    //! @copydoc MediaSetManager::configuration
    [[nodiscard]] const MediaSetConfiguration& configuration() const final;

    //! @copydoc MediaSetManager::mediaSet
    Media::MediaSetPtr mediaSet( std::string_view partNumber) final;

    //! @copydoc MediaSetManager::mediaSets() const
    [[nodiscard]] const MediaSets& mediaSets() const final;

    //! @copydoc MediaSetManager::mediaSets()
    MediaSets& mediaSets() final;

    //! @copydoc MediaSetManager::add()
    void add(
      Media::ConstMediaSetPtr mediaSet,
      MediumPathHandler mediumPathHandler) final;

    //! @copydoc MediaSetManager::loads() const
    [[nodiscard]] Media::ConstLoads loads() const final;

    //! @copydoc MediaSetManager::load(std::string_view) const
    [[nodiscard]] Media::ConstLoads load( std::string_view filename) const final;

    //! @copydoc MediaSetManager::filePath
    [[nodiscard]] std::filesystem::path filePath(
      Media::ConstBaseFilePtr file) const final;

  private:
    //! media path map
    using MediaPaths = std::map< Media::ConstMediumPtr, std::filesystem::path>;

    //! media set configuration
    const MediaSetConfiguration config;
    //! media sets
    MediaSets mediaSetsValue;
    //! media paths
    MediaPaths mediaPaths;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Utils::MediaSetManagerImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP
#define ARINC665_UTILS_MEDIASETMANAGERIMPL_HPP

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetConfiguration.hpp>

namespace Arinc665 {
namespace Utils {

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
    MediaSetManagerImpl( const MediaSetConfiguration &config);

    //! @copydoc MediaSetManager::mediaSet
    Media::MediaSetPtr mediaSet( const string &partNumber) final;

    //! @copydoc MediaSetManager::mediaSets() const
    const MediaSets& mediaSets() const  final;

    //! @copydoc MediaSetManager::mediaSets()
    MediaSets& mediaSets() final;

    //! @copydoc MediaSetManager::loads() const
    Media::ConstLoads loads() const final;

    //! @copydoc MediaSetManager::filePath
    path filePath( Media::ConstBaseFilePtr file) const final;

  private:
    //! media path map
    using MediaPaths = std::map< Media::ConstMediumPtr, path>;

    //! media set configuration
    const MediaSetConfiguration config;
    //! media sets
    MediaSets mediaSetsValue;
    //! media paths
    MediaPaths mediaPaths;
};

}
}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Utils::MediaSetManager.
 **/

#ifndef ARINC665_UTILS_MEDIASETMANAGER_HPP
#define ARINC665_UTILS_MEDIASETMANAGER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <boost/filesystem/path.hpp>

#include <list>
#include <string>

namespace Arinc665 {
namespace Utils {

/**
 * @brief This class manages the locally held media sets.
 *
 * @par Organisation of Media Set Manager data.
 * * Media sets are stored beneath the [mediaSetBasePath] directory.
 * * Within this directory the media sets each are stored within a directory
 *   named [mediaSetName].
 * * Within the media set directory the media are stored with the corresponding
 *   medium-path mapping.
 **/
class MediaSetManager
{
  public:
    //! Path type
    using path = boost::filesystem::path;
    //! String type
    using string = std::string;
    //! Media sets type (list)
    using MediaSets = std::list< Media::MediaSetPtr>;

    //! Default destructor
    virtual ~MediaSetManager() noexcept = default;

    /**
     * @brief Creates the Media Set Manager instance with the given
     *   configuration.
     *
     * @param[in] config
     *   The media set manager configuration.
     *
     * @return The media set manager instance.
     **/
    static MediaSetManagerPtr createInstance(
      const MediaSetConfiguration &config);

    /**
     * @brief Returns the media set with the given part number.
     *
     * @param[in] partNumber
     *   The media set part number.
     *
     * @return The media set with the given part number.
     **/
    virtual Media::MediaSetPtr mediaSet( const string &partNumber) = 0;

    /**
     * @brief Returns all registered media sets.
     *
     * @return All media sets.
     **/
    virtual const MediaSets& mediaSets() const = 0;

    //! @copydoc MediaSetManager::mediaSets() const
    virtual MediaSets& mediaSets() = 0;

    /**
     * @brief Get all available Loads.
     *
     * @return All available loads.
     **/
    virtual Media::ConstLoads loads() const = 0;

    /**
     * @brief Returns the path to the given file.
     *
     * @param[in] file
     *   File to request the path from
     *
     * @return The path to the given file.
     **/
    virtual path filePath( Media::ConstBaseFilePtr file) const = 0;
};

}
}

#endif

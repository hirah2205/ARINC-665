/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetCopier.
 **/

#ifndef ARINC665_UTILS_FILESYSTEMMEDIASETCOPIER_HPP
#define ARINC665_UTILS_FILESYSTEMMEDIASETCOPIER_HPP

#include <arinc665/utils/Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Copier.
 **/
class ARINC665_EXPORT FilesystemMediaSetCopier
{
  public:
    /**
     * @brief Creates the ARINC 665 %Media Set Copier Instance.
     *
     * @return ARINC 665 %Media Set Copier Instance
     **/
    static FilesystemMediaSetCopierPtr create();

    //! Destructor
    virtual ~FilesystemMediaSetCopier() = default;

    /**
     * @brief Updates Media Paths.
     *
     * @param[in] mediaPaths
     *   Media Paths of Source Media Set.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetCopier& mediaPaths( MediaPaths mediaPaths ) = 0;

    /**
     * @brief Updates the base directory of the Media Set, where it will be
     *   created.
     *
     * @param[in] mediaSetBasePath
     *   Media Set Base directory.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetCopier& mediaSetBasePath(
      std::filesystem::path mediaSetBasePath ) = 0;

    /**
     * @brief Executes the ARINC 665 Media Set Copier.
     *
     * All parameters must have been set previously.
     *
     * @return Media Paths of Destination %Media Set.
     *
     * @throw Arinc665Exception
     *   When copy operation fails
     **/
    [[nodiscard]] virtual MediaPaths operator()() = 0;
};

}

#endif

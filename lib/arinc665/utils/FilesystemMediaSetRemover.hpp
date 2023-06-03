// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetRemover.
 **/

#ifndef ARINC665_UTILS_FILESYSTEMMEDIASETREMOVER_HPP
#define ARINC665_UTILS_FILESYSTEMMEDIASETREMOVER_HPP

#include <arinc665/utils/Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Remover.
 **/
class ARINC665_EXPORT FilesystemMediaSetRemover
{
  public:
    /**
     * @brief Creates the ARINC 665 %Media Set Remover Instance.
     *
     * @return ARINC 665 %Media Set Remover Instance
     **/
    static FilesystemMediaSetRemoverPtr create();

    //! Destructor
    virtual ~FilesystemMediaSetRemover() = default;

    /**
     * @brief Updates Media Paths.
     *
     * @param[in] mediaSetPaths
     *   Media Set Paths of Media Set.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetRemover& mediaSetPaths(
      MediaSetPaths mediaSetPaths ) = 0;

    /**
     * @brief Executes the ARINC 665 Media Set Remover.
     *
     * All parameters must have been set previously.
     *
     * @throw Arinc665Exception
     *   When copy operation fails
     **/
    virtual void operator()() = 0;
};

}

#endif

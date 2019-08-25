/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::TargetHardwareId.
 **/

#ifndef ARINC665_TARGETHARDWAREWID_HPP
#define ARINC665_TARGETHARDWAREWID_HPP

#include <arinc665/Arinc665.hpp>

#include <string>
#include <string_view>

namespace Arinc665 {

/**
 * @brief Represents a ARINC 665 Target Hardware ID (THW ID).
 **/
class TargetHardwareId
{
  public:
    /**
     * @brief Constructs a target hardware ID with the given string.
     *
     * @param[in] targetHardwareId
     *   The target hardware ID as string.
     **/
    TargetHardwareId( std::string_view targetHardwareId);

    //! @copydoc TargetHardwareId(std::string_view)
    TargetHardwareId( std::string &&targetHardwareId);

    /**
     * @brief Returns the target hardware ID as string.
     *
     * @return The target hardware ID as string.
     **/
    std::string_view get() const noexcept;

    /**
     * @brief Sets the target hardware ID to the given string.
     *
     * @param[in] targetHardwareId
     *   The target hardware ID as string.
     **/
    void set( std::string_view targetHardwareId);

    //! void set(std::string_view)
    void set( std::string &&targetHardwareId);

  private:
    //! Target Hardware ID
    std::string targetHardwareId;
};

}

#endif

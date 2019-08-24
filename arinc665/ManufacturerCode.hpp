/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::ManufacturerCode.
 **/

#ifndef ARINC665_MANUFACTURERCODE_HPP
#define ARINC665_MANUFACTURERCODE_HPP

#include <arinc665/Arinc665.hpp>

#include <string>
#include <string_view>

namespace Arinc665 {

/**
 * @brief Represents a ARINC 665 manufacturer code.
 **/
class ManufacturerCode
{
  public:
    //! Length of manufacturer code
    static constexpr size_t Length = 3U;

    /**
     * @brief Constructs a manufacturer code.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     *
     * @throw Arinc665Exception
     *   When the given string is not an valid manufacturer code.
     *
     * @sa set
     **/
    ManufacturerCode( std::string_view manufacturerCode);

    //! @copydoc ManufacturerCode(std::string_view)
    ManufacturerCode( std::string &&manufacturerCode);

    /**
     * @brief Returns the manufacturer code as string.
     *
     * @return The manufacturer code.
     **/
    std::string_view get() const;

    /**
     * @brief Sets the manufacturer code to the given value.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     *
     * @throw Arinc665Exception
     *   When the given string is not an valid manufacturer code.
     **/
    void set( std::string_view manufacturerCode);

    //! @copydoc set(std::string_view)
    void set( std::string &&manufacturerCode);

  private:
    //! Manufacturer code.
    std::string manufacturerCode;
};

}

#endif

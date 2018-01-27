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
    ManufacturerCode( const std::string &manufacturerCode);

    //! @copydoc ManufacturerCode(const string&)
    ManufacturerCode( std::string &&manufacturerCode);

    /**
     * @brief Returns the manufacturer code as string.
     *
     * @return The manufacturer code.
     **/
    std::string get() const;

    /**
     * @brief Sets the manufacturer code to the given value.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     *
     * @throw Arinc665Exception
     *   When the given string is not an valid manufacturer code.
     **/
    void set( const std::string &manufacturerCode);

    //! @copydoc set(const string&)
    void set( std::string &&manufacturerCode);

  private:
    //! The stored manufacturer code.
    std::string manufacturerCode;
};

}

#endif

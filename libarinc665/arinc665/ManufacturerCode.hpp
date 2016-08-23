/*
 * $Date$
 * $Revision$
 */
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
    using string = std::string;

    //! Length of manufacturer code
    static const size_t Length = 3;

    /**
     * @brief Constructs a manufacturer code.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     *
     * @throw Arinc665Exception
     *   When the given string is not an valid manufacturer code.
     **/
    ManufacturerCode( const string& manufacturerCode);

    /**
     * @brief Returns the manufacturer code as string.
     *
     * @return The manufacturer code.
     **/
    string get( void) const;

    /**
     * @brief Sets the manufacturer code to the given value.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     **/
    void set( const string& manufacturerCode);

  private:
    //! The stored manufacturer code.
    string manufacturerCode;
};

}

#endif

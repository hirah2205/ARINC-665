/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::ProductIdentifier.
 **/

#ifndef ARINC665_PRODUCTIDENTIFIER_HPP
#define ARINC665_PRODUCTIDENTIFIER_HPP

#include <arinc665/Arinc665.hpp>

#include <string>
#include <string_view>

namespace Arinc665 {

/**
 * @brief Represents the product identifier of an ARINC 665 part number.
 **/
class ProductIdentifier
{
  public:
    //! Length of product identifier
    static constexpr unsigned int Length = 8U;

    /**
     * @brief Constructs the product identifier for the given string.
     *
     * @param[in] productIdentifier
     *   The product identifier.
     *
     * @throw Arinc665Exception
     *   If the string does not represents an valid product identifier.
     **/
    explicit ProductIdentifier( std::string_view productIdentifier);

    //! @copydoc ProductIdentifier(std::string_view)
    explicit ProductIdentifier( std::string &&productIdentifier);

    /**
     * @brief Returns the product identifier as string.
     *
     * @return The product identifier
     **/
    std::string_view get() const;

    /**
     * @brief Sets the product identifier.
     *
     * @param[in] productIdentifier
     *   The product identifier.
     **/
    void set( std::string_view productIdentifier);

    //! @copydoc set(std::string_view)
    void set( std::string &&productIdentifier);

  private:
    void check( std::string_view productIdentifier) const;

    //! Product identifier.
    std::string productIdentifier;
};

}

#endif

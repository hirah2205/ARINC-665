/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::PartNumber.
 **/

#ifndef ARINC665_PARTNUMBER_HPP
#define ARINC665_PARTNUMBER_HPP

#include <arinc665/Arinc665.hpp>
#include <arinc665/ManufacturerCode.hpp>
#include <arinc665/ProductIdentifier.hpp>
#include <arinc665/CheckCode.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <string>
#include <cstdint>

namespace Arinc665 {

/**
 * @brief Represents a ARINC665 Part Number.
 *
 * An ARINC 665 part number is as follows:
 *
 * MMMCC-SSSS-SSSS
 *
 * whereby:
 * @li MMM is a unique, upper-case alphanumeric identifier that is assigned to
 * each software supplier
 * @li CC is a two "check characters" generated from the other characters in
 * the part number
 * @li SSSSSSSS is a software supplier defined unique product identifier
 * consisting of upper-case alphanumeric characters except for alpha characters
 * "I", "O", "Q" and "Z".
 **/
class PartNumber
{
  public:
    //! The length of an ARINC 665 part number
    static constexpr size_t Length = ManufacturerCode::Length
      + CheckCode::Length + ProductIdentifier::Length;

    /**
     * @brief Constructs an ARINC 665 part number based on manufacturer code
     * and product identifier.
     *
     * @param[in] manufacturerCode
     *   Manufacturer code
     * @param[in] productIdentifier
     *   Product identifier
     **/
    PartNumber(
      const ManufacturerCode &manufacturerCode,
      const ProductIdentifier &productIdentifier);

    /**
     * @brief Constructs an ARINC 665 part number based on a part number
     *   string.
     *
     * @param[in] partNumber
     *   The part number as string
     **/
    PartNumber( const std::string& partNumber);

    /**
     * @brief Returns the manufacturer code.
     *
     * @return The manufacturer code.
     **/
    ManufacturerCode manufacturerCode() const;

    /**
     * @brief Returns the manufacturer code.
     *
     * @return The manufacturer code.
     **/
    ManufacturerCode& manufacturerCode();

    /**
     * @brief Sets the manufacturer code.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     **/
    void manufacturerCode( const ManufacturerCode& manufacturerCode);

    /**
     * @brief Returns the product identifier.
     *
     * @return The product identifier.
     **/
    ProductIdentifier productIdentifier() const;

    /**
     * @brief Returns the product identifier.
     *
     * @return The product identifier.
     **/
    ProductIdentifier& productIdentifier();

    /**
     * @brief Sets the product identifier.
     *
     * @param[in] productIdentifier
     *   The product identifier.
     **/
    void productIdentifier( const ProductIdentifier& productIdentifier);

    /**
     * @brief Calculates and return the check code
     *
     * @return The calculated check code
     **/
    CheckCode checkCode() const;

    /**
     * @brief Returns the part number as string
     *
     * @return The part number as string
     **/
    std::string partNumber() const;

  private:
    //! The manufacture code
    ManufacturerCode manufacturerCodeValue;
    //! the product identifier
    ProductIdentifier productIdentifierValue;
};

}

#endif

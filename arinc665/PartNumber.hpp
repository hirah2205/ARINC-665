/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::PartNumber.
 **/

#ifndef ARINC665_PARTNUMBER_HPP
#define ARINC665_PARTNUMBER_HPP

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <string>
#include <string_view>
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
    //! Length of Manufacturer Code
    static constexpr size_t ManufacturerCodeLength = 3U;

    //! Length of Product Identifier
    static constexpr size_t ProductIdentifierLength = 8U;

    //! The length of the check code when represented as string
    static constexpr size_t CheckCodeLength = 2U;

    //! The length of an ARINC 665 part number
    static constexpr size_t Length = ManufacturerCodeLength
      + CheckCodeLength + ProductIdentifierLength;

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
      std::string_view manufacturerCode,
      std::string_view productIdentifier);

    /**
     * @brief Constructs an ARINC 665 part number based on a part number
     *   string.
     *
     * @param[in] partNumber
     *   The part number as string
     **/
    PartNumber( std::string_view partNumber);

    /**
     * @brief Returns the manufacturer code.
     *
     * @return The manufacturer code.
     **/
    [[nodiscard]] std::string_view manufacturerCode() const;

    /**
     * @brief Sets the manufacturer code.
     *
     * @param[in] manufacturerCode
     *   The manufacturer code.
     **/
    void manufacturerCode( std::string_view manufacturerCode);

    /**
     * @brief Returns the product identifier.
     *
     * @return The product identifier.
     **/
    [[nodiscard]] std::string_view productIdentifier() const;

    /**
     * @brief Sets the product identifier.
     *
     * @param[in] productIdentifier
     *   The product identifier.
     **/
    void productIdentifier( std::string_view productIdentifier);

    /**
     * @brief Calculates and return the check code
     *
     * @return The calculated check code
     **/
    [[nodiscard]] std::string checkCode() const;

    /**
     * @brief Returns the part number as string
     *
     * @return The part number as string
     **/
    [[nodiscard]] std::string partNumber() const;

  private:
    /**
     * @brief Checks the Manufacturer Code.
     *
     * @param[in] manufacturerCode
     *   Manufacturer Code to check
     **/
    void checkManufacturerCode( std::string_view manufacturerCode) const;

    /**
     * @brief Checks the Product Identifier.
     *
     * @param[in] productIdentifier
     *   Product Identifier to check
     **/
    void checkProductIdentifier( std::string_view productIdentifier) const;

    /**
     * @brief Checks the ManuCheckfacturer Code.
     *
     * @param[in] checkCode
     *   Check  Code to check
     **/
    void checkCheckCode( std::string_view checkCode) const;

    //! Manufacture Code
    std::string manufacturerCodeValue;
    //! Product Identifier
    std::string productIdentifierValue;
};

}

#endif

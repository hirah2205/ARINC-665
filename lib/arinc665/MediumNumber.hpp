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
 * @brief Declaration of Class Arinc665::MediumNumber.
 **/

#ifndef ARINC665_MEDIUMNUMBER_HPP
#define ARINC665_MEDIUMNUMBER_HPP

#include <arinc665/Arinc665.hpp>

#include <cstdint>
#include <iosfwd>

namespace Arinc665 {

/**
 * @brief ARINC 665 Medium Number
 *
 * The Medium Number is used to provide a Medium Number @p [0..255].
 **/
class ARINC665_EXPORT MediumNumber
{
  public:
    /**
     * @brief Default Constructor.
     *
     * Initialises the Medium Number to @p 1.
     **/
    explicit MediumNumber() noexcept = default;

    /**
     * @brief Constructs the Medium Number
     *
     * @param[in] mediumNumber
     *   Medium Number
     **/
    explicit MediumNumber( uint8_t mediumNumber ) noexcept;

    /**
     * @brief Binary Plus Operator with @p uin8_t.
     *
     * @param[in] increment
     *   Increment Value.
     *
     * @return *this
     **/
    MediumNumber operator+( uint8_t increment );

    /**
     * @brief Prefix increment operator (@p ++mediumNumber).
     *
     * @return *this
     **/
    MediumNumber operator++();

    /**
     * @brief Postfix increment operator (@p mediumNumber++).
     *
     * @return *this before increment
     **/
    MediumNumber operator++( int );

    /**
     * @brief Explicit cast to @p uint8_t.
     *
     * @return Medium Number @p uint8_t
     **/
    explicit operator uint8_t() const;

    /**
     * @brief Explicit cast to @p std::string.
     *
     * @return Medium Number as @p std::string.
     **/
    explicit operator std::string() const;

    /**
     * @brief Assigns @p uint8_t to medium number.
     *
     * @param[in] mediumNumber
     *   Medium Number
     *
     * @return *this
     **/
    MediumNumber& operator =( uint8_t mediumNumber );

    /**
     * @brief 3-way comparison of Medium Number.
     *
     * @param[in] rhs
     *   Compares to.
     *
     * @return Comparison.
     **/
    auto operator<=>( const MediumNumber &rhs ) const = default;

  private:
    //! Medium Number
    uint8_t mediumNumberV{ 1U };
};

/**
 * @brief Medium Number @p std::ostream output operator.
 *
 * @param[in,out] ostream
 *   Output stream.
 * @param[in] mediumNumber
 *   Medium number.
 *
 * @return Output stream
 *
 * @sa @ref MediumNumber
 **/
ARINC665_EXPORT std::ostream& operator<<(
  std::ostream &ostream,
  const MediumNumber &mediumNumber );

}

#endif

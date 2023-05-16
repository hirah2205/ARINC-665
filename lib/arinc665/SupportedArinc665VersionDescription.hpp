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
 * @brief Declaration of Class Arinc665::SupportedArinc665VersionDescription.
 **/

#ifndef ARINC665_SUPPORTEDARINC665VERSIONDESCRIPTION_HPP
#define ARINC665_SUPPORTEDARINC665VERSIONDESCRIPTION_HPP

#include <arinc665/Arinc665.hpp>

#include <helper/Description.hpp>

#include <iosfwd>

namespace Arinc665 {

//! Description for SupportedArinc665Version Enumeration.
class ARINC665_EXPORT SupportedArinc665VersionDescription :
  public Helper::Description<
    SupportedArinc665VersionDescription,
    SupportedArinc665Version >
{
  public:
    //! Initialises Description Values
    SupportedArinc665VersionDescription();
};

/**
 * @brief Supported ARINC 665 %Version @p std::ostream output operator.
 *
 * @param[in,out] stream
 *   Output Stream
 * @param[in] version
 *   ARINC 665 version.
 *
 * @return @p stream for chaining.
 **/
ARINC665_EXPORT std::ostream& operator<<(
  std::ostream &stream,
  SupportedArinc665Version version );

/**
 * @brief Supported ARINC 665 %Version @p std::istream input operator.
 *
 * @param[in,out] stream
 *   Input Stream
 * @param[out] version
 *   Decoded ARINC 665 Version
 *
 * @return @p stream for chaining.
 **/
ARINC665_EXPORT std::istream& operator>>(
  std::istream& stream,
  SupportedArinc665Version &version );

}

#endif

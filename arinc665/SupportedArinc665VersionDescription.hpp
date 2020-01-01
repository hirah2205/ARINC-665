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

//! Description for SupportedArinc665Version enumeration.
class SupportedArinc665VersionDescription : public Helper::Description<
  SupportedArinc665VersionDescription,
  SupportedArinc665Version,
  SupportedArinc665Version::Invalid>
{
  public:
    //! Initialises Description Values
    SupportedArinc665VersionDescription();
};

/**
 * @brief Streaming operator to output ARINC 665 Version as string
 *
 * @param[in,out] stream
 *   The output stream
 * @param[in] version
 *   The ARINC 665 version.
 *
 * @return The stream for chaining.
 **/
std::ostream& operator<<( std::ostream &stream, SupportedArinc665Version version);

/**
 * @brief Streaming operator for parsing a version string as ARINC 665 version
 *   type.
 *
 * @param[in,out] stream
 *   The input stream
 * @param[out] version
 *   The decoded ARINC 665 Version
 *
 * @return The stream for chaining.
 **/
std::istream& operator>>(
  std::istream& stream,
  SupportedArinc665Version &version);

}

#endif

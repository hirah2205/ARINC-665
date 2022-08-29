/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of String Utility Functions.
 **/

#ifndef ARINC665_FILES_STRINGUTILS_HPP
#define ARINC665_FILES_STRINGUTILS_HPP

#include <arinc665/files/Files.hpp>

#include <string>
#include <string_view>
#include <list>
#include <set>

namespace Arinc665::Files {

/**
 * @name ARINC 665 String Handling
 *
 * @par String Encoding
 *
 * Offset | Field   | Length   | Description
 * -------|---------|----------|------------------------------------------------
 * +0     | Length  | 2 Bytes  | Length of String
 * +2     | Value   | Variable | String (`Length` Bytes)
 * +x     | Padding | 0..1     | If string length is odd, add a `0` fill char
 *
 * @{
 **/

/**
 * @brief Decodes the ARINC 665 string from the stream.
 *
 * @param[in] it
 *   Current position, where the raw string start.
 * @param[out] str
 *   Will contain the decoded string.
 *
 * @return New iterator position.
 **/
ConstRawFileSpan::iterator StringUtils_decodeString(
  ConstRawFileSpan::iterator it,
  std::string &str );

/**
 * @brief Encodes the ARINC 665 string to the stream.
 *
 * @param[in] str
 *  String to encode.
 *
 * @return Encoded raw string.
 **/
RawFile StringUtils_encodeString( std::string_view str );

/** @} **/

/**
 * @name ARINC 665 String List Handling.
 *
 * Offset | Field          | Length   | Description
 * -------|----------------|----------|-----------------------------------------
 * +0     | No. of Strings | 2 Bytes  | Number of Strings following this filed
 * +x     | String 1       | Variable | Encoded string
 * +y     | String x       | Variable | Encoded string
 *
 * @{
 **/

/**
 * @brief Decodes the ARINC 665 String List from the stream.
 *
 * @param[in] it
 *   current position, where the raw string list starts.
 * @param[out] strings
 *   Decoded Strings.
 *
 * @return New iterator position.
 **/
ConstRawFileSpan::iterator StringUtils_decodeStrings(
  ConstRawFileSpan::iterator it,
  std::list< std::string > &strings );

//! @copydoc StringUtils_decodeStrings(ConstRawFileSpan::iterator,std::list<std::string>&)
ConstRawFileSpan::iterator StringUtils_decodeStrings(
  ConstRawFileSpan::iterator it,
  std::set< std::string, std::less<> > &strings );

/**
 * @brief Encodes the ARINC 665 String List to the Stream.
 *
 * @param[in] strings
 *   String List
 *
 * @return Encoded raw string list.
 **/
RawFile StringUtils_encodeStrings( const std::list< std::string > &strings );

//! @copydoc StringUtils_encodeStrings(const std::list<std::string>&)
RawFile StringUtils_encodeStrings(
  const std::set< std::string, std::less<> > &strings );

/** @} **/

}

#endif

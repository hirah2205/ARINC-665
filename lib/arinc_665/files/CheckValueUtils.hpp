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
 * @brief Declaration of Check Value Utility Functions.
 **/

#ifndef ARINC_665_FILES_CHECKVALUEUTILS_HPP
#define ARINC_665_FILES_CHECKVALUEUTILS_HPP

#include <arinc_665/files/Files.hpp>

#include <arinc_645/Arinc645.hpp>

namespace Arinc665::Files {

/**
 * @name Check Value
 *
 * Encoding/ decoding of Check Values.
 *
 * @par Check Value Encoding
 *
 * Offset | Field  | Length  | Description
 * -------|--------|---------|--------------------------------------------------
 * +0     | Length | 2 Bytes | Length of Check Value Including Length Field
 * +2     | Type   | 2 Bytes | Check Value Type
 * +4     | Value  | n * 2 B | Check Value (always even number of bytes)
 *
 * @par No Check Value Encoding
 * There are two alternatives for encoding "No Check Value":
 *  1. Set `Length` field to `0` and omit `Type` and `Value` field.
 *  2. Set `Length` field to `4`, set `Type` field to `0` and omit `Value`
 *     field.
 *
 * This implementation uses the first option.
 *
 * @{
 **/

/**
 * @brief Calculates the Size of the Encoded Check Value.
 *
 * @param[in] type
 *   Check Value Type to encode.
 *
 * @return Size of raw representation of @p checkValue.
 **/
[[nodiscard]] size_t ARINC_665_EXPORT CheckValueUtils_size(
  Arinc645::CheckValueType type );

/**
 * @brief Encodes the given Check Value.
 *
 * @param[in] checkValue
 *   Check Value to encode.
 *
 * @return Check Value as raw data.
 **/
[[nodiscard]] RawFile ARINC_665_EXPORT CheckValueUtils_encode(
  const Arinc645::CheckValue &checkValue );

/**
 * @brief Decodes the given data as Check Value.
 *
 * @param[in] rawFile
 *   Raw file data starting with raw check value.
 *
 * @return Decoded Check Value.
 *
 * @throw Arinc665Exception
 *   When Check Value is Invalid
 *   - Invalid Length
 *   - Invalid Type
 *   - Check Value Type / Length mismatch
 **/
[[nodiscard]] Arinc645::CheckValue ARINC_665_EXPORT CheckValueUtils_decode(
  ConstRawFileSpan rawFile );

/** @} **/

}

#endif

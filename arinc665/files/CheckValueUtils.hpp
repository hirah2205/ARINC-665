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

#ifndef ARINC665_FILES_CHECKVALUEUTILS_HPP
#define ARINC665_FILES_CHECKVALUEUTILS_HPP

#include <arinc665/files/Files.hpp>

#include <arinc645/Arinc645.hpp>

namespace Arinc665::Files {

/**
 * @name Check Value
 *
 * Encoding/ decoding of Check Values.
 *
 * @par Check Value Encoding
 *
 * Field  | Length of Field | Description
 * -------|-----------------|---------------------------------------------------
 * Length | 2 Bytes         | Length of Check Value Including Length Field
 * Type   | 2 Bytes         | Check Value Type
 * Value  | Variable        | Check Value
 *
 * @{
 **/

/**
 * @brief Encodes the given Check Value.
 *
 * There are two alternatives for encoding "No Check Value".
 * Setting the Length Field to @p 0, or set Length Field to @p 4 and the
 * Check Value Type to @p 0 (No Check Value).
 * This implenetation uses the first option.
 *
 * @param[in] checkValue
 *   Check Value to encode.
 *
 * @return Check Value as raw data.
 **/
RawFile ARINC665_EXPORT CheckValueUtils_encode(
  const Arinc645::CheckValue &checkValue );

/**
 * @brief Decodes the given data as Check Value.
 *
 * @param[in] rawFile
 *   Raw file data.
 *
 * @return Decoded Check Value.
 *
 * @throw Arinc665Exception
 *   When Check Value is Invalid
 *   - Invalid Length
 *   - Invalid Type
 *   - Check Value Type / Length mismatch
 **/
Arinc645::CheckValue ARINC665_EXPORT CheckValueUtils_decode(
  const ConstRawFileSpan &rawFile );

/** @} **/

}

#endif

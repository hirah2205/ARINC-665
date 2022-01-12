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

#include <optional>

namespace Arinc665::Files {

/**
 * @brief Encodes the given Check Value.
 *
 * @param[in] checkValue
 *   Check Value to encode.
 *
 * @return Check Value as raw data.
 **/
RawFile ARINC665_EXPORT CheckValueUtils_encode(
  const std::optional< CheckValue > &checkValue );

/**
 * @brief Decodes the given data as Check Value.
 *
 * @param[in] rawFile
 *   Raw file data.
 * @param offset
 *   Offset where to start decoding.
 *
 * @return Decoded Check Value.
 **/
std::optional< CheckValue > ARINC665_EXPORT CheckValueUtils_decode(
  const ConstRawFileSpan &rawFile,
  ptrdiff_t offset );

}

#endif

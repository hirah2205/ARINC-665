/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Module Arinc665::Utils Printer.
 *
 * This module provides operations to print the content of ARINC 665 Media Sets
 * and ARINC 665 files.
 **/

#ifndef ARINC665_UTILS_PRINTER_HPP
#define ARINC665_UTILS_PRINTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <iosfwd>
#include <iostream>
#include <string_view>

namespace Arinc665::Utils {

/**
 * @brief Decodes an Prints the Content of the Media Set.
 *
 * - Prints files
 * - Prints loads
 * - Prints batches
 *
 * @param[in] mediaSet
 *   Media Set to print.
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void ARINC665_EXPORT printMediaSet(
  const Media::ConstMediaSetPtr &mediaSet,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = " " );

/**
 * @brief Decodes an Prints the Content of the Media Set File.
 *
 * - File Name
 * - File Path
 *
 * @param[in] file
 *   File to print.
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 **/
void ARINC665_EXPORT printFile(
  const Media::ConstFilePtr &file,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {} );

/**
 * @brief Decodes an Prints the Content of the Load.
 *
 * - Load Name
 * - Load Part Number
 * - Data Files
 * - Support Files
 *
 * @param[in] load
 *   Media Set to print.
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void ARINC665_EXPORT printLoad(
  const Media::ConstLoadPtr &load,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = " " );

/**
 * @brief Decodes an Prints the Content of the Batch.
 *
 * - Batch Name
 * - Batch Part Number
 * - Loads
 *
 * @param[in] batch
 *   Batch to print.
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void ARINC665_EXPORT printBatch(
  const Media::ConstBatchPtr &batch,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = " " );

}

#endif

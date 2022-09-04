/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Module Arinc665::Utils FilePrinter.
 **/

#ifndef ARINC665_UTILS_FILEPRINTER_HPP
#define ARINC665_UTILS_FILEPRINTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/files/Files.hpp>

#include <iosfwd>
#include <iostream>
#include <string_view>

namespace Arinc665::Utils {

/**
 * @brief Prints the Content of the given File List File.
 *
 * @param[in] fileListFile
 *   File List File
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void ARINC665_EXPORT FilePrinter_print(
  const Arinc665::Files::FileListFile &fileListFile,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = " " );

/**
 * @brief Prints the Content of the given File List File.
 *
 * @param[in] loadListFile
 *   Load List File
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void ARINC665_EXPORT FilePrinter_print(
  const Arinc665::Files::LoadListFile &loadListFile,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = " " );

/**
 * @brief Prints the Content of the given File List File.
 *
 * @param[in] batchListFile
 *   Batch List File
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void FilePrinter_print(
  const Arinc665::Files::BatchListFile &batchListFile,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = "" );

/**
 * @brief Prints the Content of the given Load Header File.
 *
 * @param[in] loadHeaderFile
 *   Load Header File
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void FilePrinter_print(
  const Arinc665::Files::LoadHeaderFile &loadHeaderFile,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = "" );

/**
 * @brief Prints the Content of the given Batch File.
 *
 * @param[in] batchFile
 *   Batch File
 * @param[in,out] outS
 *   Output Stream
 * @param[in] initialIndent
 *   Initial Indention prepended before each output.
 * @param[in] indent
 *   Indent for sub-information
 **/
void FilePrinter_print(
  const Arinc665::Files::BatchFile &batchFile,
  std::ostream &outS = std::cout,
  std::string_view initialIndent = {},
  std::string_view indent = "" );

}

#endif

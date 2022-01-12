/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::LoadFileInfo.
 **/

#ifndef ARINC665_FILES_LOADFILEINFO_HPP
#define ARINC665_FILES_LOADFILEINFO_HPP

#include <arinc665/files/Files.hpp>

#include <string>
#include <optional>
#include <cstdint>

namespace Arinc665::Files {

/**
 * @brief ARINC 665 Load %File Information.
 *
 * File Information stored within the file list of the ARINC 665 Load Header
 * File.
 *
 * The file size is always interpreted in number of bytes (octets).
 *
 * @sa LoadHeaderFile
 **/
struct ARINC665_EXPORT LoadFileInfo
{
  //! Filename
  std::string filename;
  //! File Part Number
  std::string partNumber;
  //! File Length (Always in bytes)
  uint64_t length;
  //! File CRC
  uint16_t crc;
  //! Check Value (since ARINC 665-3)
  std::optional< CheckValue> checkValue;
};

}

#endif

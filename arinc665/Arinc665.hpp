/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Namespace Arinc665.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665.
 **/

#ifndef ARINC665_ARINC665_HPP
#define ARINC665_ARINC665_HPP

#include <string_view>
#include <tuple>
#include <vector>
#include <cstdint>

/**
 * @brief ARINC 665 Specification.
 *
 * This library implements the ARINC 665 specification.
 * This library currently supports following supplements of ARINC 665:
 * - Supplement 2 (Full)
 * - Supplement 3/4 (Initially)
 *
 * @par Referenced Documents
 * - ARINC Report 665-1: Loadable Software Standards; 2001-01-12
 * - ARINC Report 665-2: Loadable Software Standards; 2002-08-30
 * - ARINC Report 665-3: Loadable Software Standards; 2005-08-12
 * - ARINC Report 665-4: Loadable Software Standards; 2016-07-14
 **/
namespace Arinc665 {

//! Enumeration of all Defined ARINC 665 Versions.
enum class Arinc665Version
{
  //! ARINC 665 Supplement 1
  Supplement1,
  //! ARINC 665 Supplement 2
  Supplement2,
  //! ARINC 665 Supplement 3
  Supplement3,
  //! ARINC 665 Supplement 4
  Supplement4,

  Invalid
};

//! Enumeration used for Feature Selection of this Library.
enum class SupportedArinc665Version
{
  //! ARINC 665-2 Compatible
  Supplement2,
  //! ARINC 665-3/4/5 Compatible
  Supplement345,

  Invalid
};

//! Enumeration of file types.
enum class FileClassType
{
  //! ARINC 665 Load file
  LoadFile,
  //! ARINC 665 Batch file
  BatchFile,
  //! ARINC 665 Media files (List files)
  MediaFile,

  //! None ARINC 665 File (Not specified through standard)
  Invalid
};

/**
 * @brief Enumeration of load file versions.
 *
 * Used for load upload header files
 **/
enum class LoadFileFormatVersion : uint16_t
{
  //! ARINC 665-1 version of load file
  Version1 = 0x8002U,
  //! ARINC 665-2 version of load file
  Version2 = 0x8003U,
  //! ARINC 665-3/4/5 version of load file
  Version345 = 0x8004U,

  //! invalid value
  Invalid  = 0xFFFFU
};

/**
 * @brief Enumeration of batch file versions.
 *
 * Used for batch files
 **/
enum class BatchFileFormatVersion : uint16_t
{
  //! ARINC 665-1 version of batch file
  Version1 = 0x8002U,
  //! ARINC 665-2 version of batch file
  Version2 = 0x9003U,
  //! ARINC 665-3/4/5 version of batch file
  Version345 = 0x9004U,

  //! invalid value
  Invalid  = 0xFFFFU
};

/**
 * @brief Enumeration of media file versions.
 *
 * Used for:
 * - list of batch file,
 * - list of loads file, and
 * - list of files file
 **/
enum class MediaFileFormatVersion : uint16_t
{
  //! ARINC 665-1 Version of Media File
  Version1 = 0x8002U,
  //! ARINC 665-2 Version of Media File
  Version2 = 0xA003U,
  //! ARINC 665-3/4/5 Version of Media File
  Version345 = 0xA004U,

  //! invalid value
  Invalid  = 0xFFFFU
};

//! All ARINC 665 file types combined
enum class Arinc665FileFormatVersion : uint16_t
{
  LoadFileVersion2   = static_cast< uint16_t>( LoadFileFormatVersion::Version2 ),
  LoadFileVersion345 = static_cast< uint16_t>( LoadFileFormatVersion::Version345 ),

  BatchFileVersion2   = static_cast< uint16_t>( BatchFileFormatVersion::Version2 ),
  BatchFileVersion345 = static_cast< uint16_t>( BatchFileFormatVersion::Version345 ),

  MediaFileVersion2   = static_cast< uint16_t>( MediaFileFormatVersion::Version2 ),
  MediaFileVersion345 = static_cast< uint16_t>( MediaFileFormatVersion::Version345 ),

  Invalid = 0xFFFFU  //!< invalid value
};

//! Check Value Type Enumeration
enum class CheckValueType : uint16_t
{
  //! Not to be used
  NotUsed = 0u,
  //! 8-bit CRC (deprecated)
  Crc8 = 1u,
  //! 16-bit CRC (deprecated)
  Crc16 = 2u,
  //! 32-bit CRC
  Crc32 = 3u,
  //! MD5
  Md5 = 4u,
  //! SHA1
  Sha1 = 5u,
  //! SHA-256
  Sha256 = 6u,
  //! SHA-512
  Sha512 = 7u,
  //! 64-bit CRC
  Crc64 = 8u,

  Invalid = 0xFFFFU
};

//! Check Value
using CheckValue = std::tuple< CheckValueType, std::vector< uint8_t>>;

//! @brief ARINC 665 File Types
enum class FileType
{
  BatchFile,        //!< ARINC 665 Batch File
  LoadUploadHeader, //!< ARINC 665 Load Upload Header File
  LoadList,         //!< ARINC 665 Load List File
  BatchList,        //!< ARINC 665 Batch List File
  FileList,         //!< ARINC 665 File List File

  Invalid           //!< invalid value
};

//! Default Filename of the List of Files: "FILES.LUM"
extern const std::string_view ListOfFilesName;

//! Default Filename of the List of Loads: "LOADS.LUM"
extern const std::string_view ListOfLoadsName;

//! Default Filename of the List of Batches: "BATCHES.LUM"
extern const std::string_view ListOfBatchesName;

//! Default Extension of a Load Upload Header: ".LUH"
extern const std::string_view LoadUploadHeaderExtension;

//! Default Extension of a Batch File: ".LUB"
extern const std::string_view BatchFileExtension;

//! CRC 8bit Polynom
constexpr uint8_t Crc8Polynom{ 0x80U };
//! CRC 8bit initialisation value
constexpr uint8_t Crc8Init{ 0x00U };
//! CRC 8bit final XOR value
constexpr uint8_t Crc8FinalXor{ 0x00U };
//! CRC 8bit reflect in
constexpr bool Crc8ReflectIn{ false };
//! CRC 8bit reflect out
constexpr bool Crc8ReflectOut{ false };

//! CRC 16bit Polynom
constexpr uint16_t Crc16Polynom{ 0x1021U };
//! CRC 16bit initialisation value
constexpr uint16_t Crc16Init{ 0xFFFFU };
//! CRC 16bit final XOR value
constexpr uint16_t Crc16FinalXor{ 0x0000U };
//! CRC 16bit reflect in
constexpr bool Crc16ReflectIn{ false };
//! CRC 16bit reflect out
constexpr bool Crc16ReflectOut{ false };

//! CRC 32bit Polynom
constexpr uint32_t Crc32Polynom{ 0x04C1'1DB7U };
//! CRC 32bit initialisation value
constexpr uint32_t Crc32Init{ 0xFFFF'FFFFU };
//! CRC 32bit final XOR value
constexpr uint32_t Crc32FinalXor{ 0xFFFF'FFFFU };
//! CRC 32bit reflect in
constexpr bool Crc32ReflectIn{ false };
//! CRC 32bit reflect out
constexpr bool Crc32ReflectOut{ false };

//! CRC 64bit Polynom
constexpr uint64_t Crc64Polynom{ 0x42F0'E1EB'A9EA'3693U };
//! CRC 64bit initialisation value
constexpr uint64_t Crc64Init{ 0xFFFF'FFFF'FFFF'FFFFU };
//! CRC 64bit final XOR value
constexpr uint64_t Crc64FinalXor{ 0xFFFF'FFFF'FFFF'FFFFU };
//! CRC 64bit reflect in
//! @todo According to ARINC 665-4 Reflect In/Out should be set to true - but then the tests fail!
constexpr bool Crc64ReflectIn{ false };
//! CRC 64bit reflect out
constexpr bool Crc64ReflectOut{ false };

class PartNumber;

}

#endif

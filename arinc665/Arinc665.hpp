/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of namespace Arinc665.
 **/

#ifndef ARINC665_ARINC665_HPP
#define ARINC665_ARINC665_HPP

#include <string>
#include <cstdint>

/**
 * @brief ARINC 665 specification implementation.
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

//! Enumeration of ARINC 665 versions.
enum class Arinc665Version
{
  //! ARINC 665 Supplement 1
  ARINC_665_1,
  //! ARINC 665 Supplement 2
  ARINC_665_2,
  //! ARINC 665 Supplement 3
  ARINC_665_3,
  //! ARINC 665 Supplement 4
  ARINC_665_4,

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
  //! ARINC 665-3 version of load file
  Version34 = 0x8004U,

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
  //! ARINC 665-3 version of batch file
  Version34 = 0x9004U,

  //! invalid value
  Invalid  = 0xFFFFU
};

/**
 * @brief Enumeration of media file versions.
 *
 * Used for:
 * @li list of batch file,
 * @li list of loads file, and
 * @li list of files file
 **/
enum class MediaFileFormatVersion : uint16_t
{
  //! ARINC 665-1 version of media file
  Version1 = 0x8002U,
  //! ARINC 665-2 version of media file
  Version2 = 0xA003U,
  //! ARINC 665-3 version of media file
  Version34 = 0xA004U,

  //! invalid value
  Invalid  = 0xFFFFU
};

//! All ARINC 665 file types combined
enum class Arinc665FileFormatVersion : uint16_t
{
  LOAD_FILE_VERSION_2 = static_cast< uint16_t>( LoadFileFormatVersion::Version2),
  LOAD_FILE_VERSION_34 = static_cast< uint16_t>( LoadFileFormatVersion::Version34),

  BATCH_FILE_VERSION_2 = static_cast< uint16_t>( BatchFileFormatVersion::Version2),
  BATCH_FILE_VERSION_34 = static_cast< uint16_t>( BatchFileFormatVersion::Version34),

  MEDIA_FILE_VERSION_2 = static_cast< uint16_t>( MediaFileFormatVersion::Version2),
  MEDIA_FILE_VERSION_34 = static_cast< uint16_t>( MediaFileFormatVersion::Version34),

  Invalid = 0xFFFFU  //!< invalid value
};

//! @brief ARINC 665 file types
enum class FileType
{
  BatchFile,        //!< ARINC 665 batch file
  LoadUploadHeader, //!< ARINC 665 load upload header file
  LoadList,         //!< ARINC 665 load list file
  BatchList,        //!< ARINC 665 batch list file
  FileList,         //!< ARINC 665 file list file

  Invalid           //!< invalid value
};

//! The default filename of the list of files: "FILES.LUM"
extern const std::string ListOfFilesName;

//! The default filename of the list of loads: "LOADS.LUM"
extern const std::string ListOfLoadsName;

//! The default filename of the list of batches: "BATCHES.LUM"
extern const std::string ListOfBatchesName;

//! The default extension of a load upload header: ".LUH"
extern const std::string LoadUploadHeaderExtension;

//! The default extension of a batch file: ".LUB"
extern const std::string BatchFileExtension;

//! CRC 8bit polynom
constexpr uint8_t Crc8Polynom = 0x80U;
//! CRC 8bit initialisation value
constexpr uint8_t Crc8Init = 0x00U;
//! CRC 8bit final XOR value
constexpr uint8_t Crc8FinalXor = 0x00U;
//! CRC 8bit reflect in
constexpr bool Crc8ReflectIn = false;
//! CRC 8bit reflect out
constexpr bool Crc8ReflectOut = false;

//! CRC 16bit polynom
constexpr uint16_t Crc16Polynom = 0x1021U;
//! CRC 16bit initialisation value
constexpr uint16_t Crc16Init = 0xFFFFU;
//! CRC 16bit final XOR value
constexpr uint16_t Crc16FinalXor = 0x0000U;
//! CRC 16bit reflect in
constexpr bool Crc16ReflectIn = false;
//! CRC 16bit reflect out
constexpr bool Crc16ReflectOut = false;

//! CRC 32bit polynom
constexpr uint32_t Crc32Polynom = 0x04C1'1DB7UL;
//! CRC 32bit initialisation value
constexpr uint32_t Crc32Init = 0xFFFFFFFFUL;
//! CRC 32bit final XOR value
constexpr uint32_t Crc32FinalXor = 0xFFFFFFFFUL;
//! CRC 32bit reflect in
constexpr bool Crc32ReflectIn = false;
//! CRC 32bit reflect out
constexpr bool Crc32ReflectOut = false;

//! CRC 64bit polynom
constexpr uint64_t Crc64Polynom = 0x42F0'E1EB'A9EA'3693ULL;
//! CRC 64bit initialisation value
constexpr uint64_t Crc64Init = 0xFFFFFFFFFFFFFFFFULL;
//! CRC 64bit final XOR value
constexpr uint64_t Crc64FinalXor = 0xFFFFFFFFFFFFFFFFULL;
//! CRC 64bit reflect in
//! @todo According to ARINC 665-4 Reflect In/Out should be set to true - but then the tests fail!
constexpr bool Crc64ReflectIn = false;
//! CRC 64bit reflect out
constexpr bool Crc64ReflectOut = false;

class CheckCode;
class ManufacturerCode;
class PartNumber;
class ProductIdentifier;
class TargetHardwareId;

}

#endif

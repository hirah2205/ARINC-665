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

#ifndef ARINC665_ARINC665_HPP
#define ARINC665_ARINC665_HPP

#include <arinc665/arinc665_export.h>

#include <string_view>
#include <vector>
#include <cstdint>

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665.
 **/

/**
 * @brief ARINC 665 Specification.
 *
 * This library implements the ARINC 665 specification.
 * This library currently supports following supplements of ARINC 665:
 * - Supplement 2 (Full)
 * - Supplement 3/4/5 (Initially)
 *
 * @par Referenced Documents
 * - ARINC Report 665-1: Loadable Software Standards; 2001-01-12
 * - ARINC Report 665-2: Loadable Software Standards; 2002-08-30
 * - ARINC Report 665-3: Loadable Software Standards; 2005-08-12
 * - ARINC Report 665-4: Loadable Software Standards; 2016-07-14
 * - ARINC Report 665-5: Loadable Software Standards; 2019-08-13
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
  //! ARINC 665 Supplement 5
  Supplement5,

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

//! Default Filename of the List of Files.
constexpr std::string_view ListOfFilesName{ "FILES.LUM" };

//! Default Filename of the List of Loads.
constexpr std::string_view ListOfLoadsName{ "LOADS.LUM" };

//! Default Filename of the List of Batches.
constexpr std::string_view ListOfBatchesName{ "BATCHES.LUM" };

//! Default Extension of a Load Upload Header.
constexpr std::string_view LoadUploadHeaderExtension{ ".LUH" };

//! Default Extension of a Batch File.
constexpr std::string_view BatchFileExtension{ ".LUB" };

class PartNumber;

}

#endif

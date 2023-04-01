/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of namespace Arinc665::Utils.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665::Utils.
 **/

#ifndef ARINC665_UTILS_UTILS_HPP
#define ARINC665_UTILS_UTILS_HPP

#include <arinc665/Arinc665.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc645/Arinc645.hpp>

#include <memory>
#include <optional>
#include <filesystem>
#include <map>
#include <set>

/**
 * @brief ARINC 665 Utilities.
 *
 * This namespace provides utilities for handling ARINC 665 %Media Sets:
 *  - Importing and Exporting Media Sets
 *  - XML Handling
 *  - %Media Set Manager
 **/
namespace Arinc665::Utils {

//! Media Paths
using MediaPaths =
  std::map< Arinc665::MediumNumber, std::filesystem::path >;

/**
 * @name Media Set Exporter
 *
 * @{
 **/

//! File creation policy of the exporter for load headers/ batch file
enum class FileCreationPolicy
{
  //! No file (load header/ batch file) is created by the exporter itself.
  None,
  //! Only non-existing files are created by the exporter itself.
  NoneExisting,
  //! All files are created by the exporter itself - even if already existing in source.
  All
};

//! File (Source) Path Mapping (File to path)
using FilePathMapping = std::map< Media::ConstFilePtr, std::filesystem::path >;

class MediaSetExporter;
//! ARINC 665 %Media Set Exporter Instance.
using MediaSetExporterPtr = std::unique_ptr< MediaSetExporter >;

class FilesystemMediaSetExporter;
//! Filesystem ARINC 665 %Media Set Exporter Instance.
using FilesystemMediaSetExporterPtr =
  std::unique_ptr< FilesystemMediaSetExporter >;

/** @} **/

/**
 * @name Media Set Importer
 *
 * @{
 **/

//! Media Set Import Result Type
using MediaSetImportResult =
  std::pair< Media::MediaSetPtr, Media::CheckValues >;

class MediaSetImporter;
//! ARINC 665 %Media Set Importer Instance Pointer.
using MediaSetImporterPtr = std::unique_ptr< MediaSetImporter >;

class FilesystemMediaSetImporter;
//! Filesystem ARINC 665 %Media Set Importer Instance Pointer.
using FilesystemMediaSetImporterPtr =
  std::unique_ptr< FilesystemMediaSetImporter >;

/** @} **/

/**
 * @name Media Set Validator
 *
 * @{
 **/

class MediaSetValidator;
//! ARINC 665 %Media Set Validator Instance Pointer.
using MediaSetValidatorPtr = std::unique_ptr< MediaSetValidator >;

/** @} **/

/**
 * @name Media Set Manager
 *
 * @{
 **/

class MediaSetManagerConfiguration;
class MediaSetManager;
//! ARINC 665 %Media Set Manager Instance Pointer.
using MediaSetManagerPtr = std::shared_ptr< MediaSetManager >;

/** @} **/

/**
 * @brief Tries to decode the given directory as ARINC 665 Medium.
 *
 * Tries to access the `List of Files` within @p directory and decodes it.
 * If the decoding is successful, the Medium Information is returned.
 *
 * @param[in] directory
 *   Medium directory
 *
 * @return Medium Information for @p directory.
 * @retval {}
 *   If @p directory is not a valid directory or ARINC 665 Medium
 **/
ARINC665_EXPORT std::optional< Files::MediaSetInformation >
getMediumInformation( const std::filesystem::path &directory );

}

#endif

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
 * @brief Declaration of namespace Arinc665::Utils.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665::Utils.
 **/

#ifndef ARINC_665_UTILS_UTILS_HPP
#define ARINC_665_UTILS_UTILS_HPP

#include <arinc_665/Arinc665.hpp>

#include <arinc_665/media/Media.hpp>

#include <arinc_665/files/Files.hpp>

#include <arinc_665/MediumNumber.hpp>

#include <arinc_645/Arinc645.hpp>

#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>

/**
 * @brief ARINC 665 Utilities.
 *
 * This namespace provides utilities for handling ARINC 665 %Media Sets:
 *  - Decompiling and Compiling %Media Sets
 *  - XML Handling
 *  - %Media Set Manager
 **/
namespace Arinc665::Utils {

//! %Media Paths (Medium Number -> Medium Path)
using MediaPaths =
  std::map< Arinc665::MediumNumber, std::filesystem::path >;

//! %Media Set Paths (%Media Set Base Path + %Media Paths)
using MediaSetPaths = std::pair< std::filesystem::path, MediaPaths >;

/**
 * @name Media Set Compiler
 *
 * @{
 **/

//! File creation policy of the compiler for load headers/ batch files
enum class FileCreationPolicy
{
  //! No file (load header/ batch file) is created by the compiler itself.
  None,
  //! Only non-existing files are created by the compiler itself.
  NoneExisting,
  //! All files are created by the compiler itself - even if already existing in source.
  All
};

//! File (Source) Path Mapping (File to path)
using FilePathMapping = std::map< Media::ConstFilePtr, std::filesystem::path >;

class MediaSetCompiler;
//! ARINC 665 %Media Set Compiler Instance.
using MediaSetCompilerPtr = std::unique_ptr< MediaSetCompiler >;

class FilesystemMediaSetCompiler;
//! Filesystem ARINC 665 %Media Set Compiler Instance.
using FilesystemMediaSetCompilerPtr =
  std::unique_ptr< FilesystemMediaSetCompiler >;

/** @} **/

/**
 * @name Media Set Decompiler
 *
 * @{
 **/

//! %Media Set Decompiler Result Type
using MediaSetDecompilerResult =
  std::pair< Media::MediaSetPtr, Media::CheckValues >;

class MediaSetDecompiler;
//! ARINC 665 %Media Set Decompiler Instance.
using MediaSetDecompilerPtr = std::unique_ptr< MediaSetDecompiler >;

class FilesystemMediaSetDecompiler;
//! Filesystem ARINC 665 %Media Set Decompiler Instance.
using FilesystemMediaSetDecompilerPtr =
  std::unique_ptr< FilesystemMediaSetDecompiler >;

/** @} **/

class FilesystemMediaSetCopier;
//! Filesystem ARINC 665 %Media Set Copier Instance.
using FilesystemMediaSetCopierPtr =
  std::unique_ptr< FilesystemMediaSetCopier >;

class FilesystemMediaSetRemover;
//! Filesystem ARINC 665 %Media Set Remover Instance.
using FilesystemMediaSetRemoverPtr =
  std::unique_ptr< FilesystemMediaSetRemover >;

/**
 * @name Media Set Validator
 *
 * @{
 **/

class MediaSetValidator;
//! ARINC 665 %Media Set Validator Instance.
using MediaSetValidatorPtr = std::unique_ptr< MediaSetValidator >;

/** @} **/

/**
 * @name Media Set Manager
 *
 * @{
 **/

class MediaSetDefaults;
class MediaSetManagerConfiguration;
class MediaSetManager;
//! ARINC 665 %Media Set Manager Instance Pointer.
using MediaSetManagerPtr = std::shared_ptr< MediaSetManager >;

/** @} **/

/**
 * @brief Tries to decode the given directory as ARINC 665 Medium.
 *
 * Tries to access the *List of %Files* within @p directory and decodes it.
 * If the decoding is successful, the medium information is returned.
 *
 * @param[in] directory
 *   Medium directory
 *
 * @return Medium Information for @p directory.
 * @retval {}
 *   If @p directory is not a valid directory or ARINC 665 Medium
 **/
[[nodiscard]] ARINC_665_EXPORT std::optional< Files::MediaSetInformation >
getMediumInformation( const std::filesystem::path &directory );

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Namespace Arinc665::Files.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of namespace Arinc665::Files.
 **/

#ifndef ARINC665_FILES_FILES_HPP
#define ARINC665_FILES_FILES_HPP

#include <arinc665/Arinc665.hpp>

#include <vector>
#include <span>
#include <list>
#include <string>
#include <cstdint>

/**
 * @brief ARINC 665 %Files.
 *
 * This namespace contains the implementation of the following ARINC 665
 * protocol files:
 * - List files:
 *   - List of %Files: FileListFile
 *   - List of Loads: LoadListFile
 *   - List of Batches: BatchListFile
 * - Load Header File: LoadHeaderFile
 * - Batch File: BatchFile
 *
 * The implementation supports the file formats of the following ARINC 665
 * supplements:
 * - Supplement 2
 * - Supplement 3
 * - Supplement 4 (Which actually does not refine the file formats)
 * - Supplement 5 (Which actually does not refine the file formats)
 **/
namespace Arinc665::Files {

//! Raw File
using RawFile = std::vector< uint8_t>;
//! Const Raw File Span
using ConstRawFileSpan = std::span< const uint8_t>;
//! Raw File Span
using RawFileSpan = std::span< uint8_t>;

class Arinc665File;

struct LoadFileInfo;
//! Load Files Information.
using LoadFilesInfo = std::list< LoadFileInfo >;
class LoadHeaderFile;

struct BatchTargetInfo;
//! Batch Targets Information.
using BatchTargetsInfo = std::list< BatchTargetInfo >;
struct BatchLoadInfo;
//! Batch Loads Information.
using BatchLoadsInfo = std::list< BatchLoadInfo >;
class BatchFile;

struct BatchInfo;
//! Batches Information.
using BatchesInfo = std::list< BatchInfo >;
class BatchListFile;

struct LoadInfo;
//! Loads Information.
using LoadsInfo = std::list< LoadInfo >;
class LoadListFile;

struct FileInfo;
//! Files Information.
using FilesInfo = std::vector< FileInfo >;
class FileListFile;

}

#endif

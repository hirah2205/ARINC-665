/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Namespace Arinc665::File.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665::File.
 **/

#ifndef ARINC665_FILE_FILE_HPP
#define ARINC665_FILE_FILE_HPP

#include <arinc665/Arinc665.hpp>

#include <vector>
#include <list>
#include <cstdint>

/**
 * @brief ARINC 665 Files.
 *
 * This namespace contains the implementation of the following ARINC 665
 * protocol files:
 * * List files:
 *   * List of Files: FileListFile
 *   * List of Loads: LoadListFile
 *   * List of Batches: BatchListFile
 * * Load Header File: LoadHeaderFile
 * * Batch File: BatchFile
 *
 * The implementation supports the file formats of the following ARINC 665
 * supplements:
 * * Supplement 2
 * * Supplement 3
 * * Supplement 4 (Which actually does not refines the file formats)
 **/
namespace Arinc665::File {

//! Raw File
using RawFile = std::vector< uint8_t>;

class Arinc665File;

class LoadFileInfo;
//! Load Files Information (list).
using LoadFilesInfo = std::list< LoadFileInfo>;
class LoadHeaderFile;

class BatchTargetInfo;
//! Batch Targets Information (list).
using BatchTargetsInfo = std::list< BatchTargetInfo>;
class BatchLoadInfo;
//! Batch Loads Information (list).
using BatchLoadsInfo = std::list< BatchLoadInfo>;
class BatchFile;

class BatchInfo;
//! Batches Information (list).
using BatchesInfo = std::list< BatchInfo>;
class BatchListFile;

class LoadInfo;
//! Loads Information (list).
using LoadsInfo = std::list< LoadInfo>;
class LoadListFile;

class FileInfo;
//! Files Information (list).
using FilesInfo = std::vector< FileInfo>;
class FileListFile;

}

#endif

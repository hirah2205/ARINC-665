/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of the namespace Arinc665::File.
 **/

#ifndef ARINC665_FILE_FILE_HPP
#define ARINC665_FILE_FILE_HPP

#include <arinc665/Arinc665.hpp>

#include <vector>
#include <list>
#include <cstdint>

namespace Arinc665 {

/**
 * @brief Namespace, which holds the decoding/ encoding of the ARINC 665 files.
 **/
namespace File {
//! Raw file type definition.
using RawFile = std::vector< uint8_t>;

class FileFactory;

class Arinc665File;

class LoadFileInfo;
using LoadFileInfos = std::list< LoadFileInfo>;
class LoadHeaderFile;

class BatchTargetInfo;
using BatchTargetsInfo = std::list< BatchTargetInfo>;
class BatchLoadInfo;
using BatchLoadsInfo = std::list< BatchLoadInfo>;
class BatchFile;

class BatchInfo;
using BatchInfos = std::list< BatchInfo>;
class BatchListFile;

class LoadInfo;
using LoadInfos = std::list< LoadInfo>;
class LoadListFile;

class FileInfo;
using FileInfos = std::vector< FileInfo>;
class FileListFile;

}
}

#endif

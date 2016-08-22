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
 * @brief Definition of namespace Arinc665::Media.
 **/

#ifndef ARINC665_MEDIA_MEDIA_HPP
#define ARINC665_MEDIA_MEDIA_HPP

#include <arinc665/Arinc665.hpp>

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace Arinc665 {

/**
 *
 **/
namespace Media {

class MediaSet;
using ConstMediaSetPtr = std::shared_ptr< const MediaSet>;
using MediaSetPtr = std::shared_ptr< MediaSet>;
using WeakMediaSetPtr = std::weak_ptr< MediaSet>;

class ContainerEntity;
using ConstContainerEntityPtr = std::shared_ptr< const ContainerEntity>;
using ContainerEntityPtr = std::shared_ptr< ContainerEntity>;
using WeakContainerEntityPtr = std::weak_ptr< ContainerEntity>;

class Medium;
using ConstMediumPtr = std::shared_ptr< const Medium>;
using MediumPtr = std::shared_ptr< Medium>;
using Media = std::vector< MediumPtr>;

class Directory;
using ConstDirectoryPtr = std::shared_ptr< const Directory>;
using DirectoryPtr = std::shared_ptr< Directory>;
using WeakDirectoryPtr = std::weak_ptr< Directory>;

using Directories = std::vector< DirectoryPtr>;
using ConstDirectories = std::vector< ConstDirectoryPtr>;

class PartNumberEntity;
class BaseFile;
class File;
class Load;
class Batch;

typedef std::shared_ptr< const BaseFile> ConstBaseFilePtr;
typedef std::shared_ptr< BaseFile> BaseFilePtr;
typedef std::weak_ptr< BaseFile> WeakBaseFilePtr;

using Files = std::vector< BaseFilePtr>;
using ConstFiles = std::vector< ConstBaseFilePtr>;

using ConstFilePtr = std::shared_ptr< const BaseFile> ;
using FilePtr = std::shared_ptr< BaseFile>;
using WeakFilePtr = std::weak_ptr< BaseFile>;

using ConstLoadPtr = std::shared_ptr< const Load>;
using LoadPtr = std::shared_ptr< Load>;
using WeakLoadPtr = std::weak_ptr< Load>;
using Loads = std::vector< LoadPtr>;
using ConstLoads = std::vector< ConstLoadPtr>;

using ConstBatchPtr = std::shared_ptr< const Batch> ;
using BatchPtr = std::shared_ptr< Batch> ;
using Batches = std::vector< BatchPtr>;
using ConstBatches = std::vector< ConstBatchPtr>;

using ConstDirectoryFileTuple  = std::tuple< const Directory&, ConstFilePtr>;
using DirectoryFileTuple = std::tuple< Directory&, FilePtr>;

}
}

#endif

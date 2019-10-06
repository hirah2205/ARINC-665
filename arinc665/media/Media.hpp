/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Namespace Arinc665::Media.
 **/

/**
 * @dir
 * @brief Declaration/ Definition of Namespace Arinc665::Media.
 **/

#ifndef ARINC665_MEDIA_MEDIA_HPP
#define ARINC665_MEDIA_MEDIA_HPP

#include <arinc665/Arinc665.hpp>

#include <map>
#include <vector>
#include <list>
#include <memory>

/**
 * @brief ARINC 665 %Media (Set).
 *
 * This namespace provides classes to represent a ARINC 665 Media Set and their
 * parts like Loads, Batches nad Management Files.
 **/
namespace Arinc665::Media {

class Base;
//! Constant Base Pointer
using ConstBasePtr = std::shared_ptr< const Base>;
//! Base Pointer
using BasePtr = std::shared_ptr< Base>;

class MediaSet;
//! Constant media set Pointer
using ConstMediaSetPtr = std::shared_ptr< const MediaSet>;
//! Media set Pointer
using MediaSetPtr = std::shared_ptr< MediaSet>;
//! Weak Media Set Pointer
using WeakMediaSetPtr = std::weak_ptr< MediaSet>;

class ContainerEntity;
//! constant container entity Pointer
using ConstContainerEntityPtr = std::shared_ptr< const ContainerEntity>;
//! container entity Pointer
using ContainerEntityPtr = std::shared_ptr< ContainerEntity>;
//! Weak container entity Pointer
using WeakContainerEntityPtr = std::weak_ptr< ContainerEntity>;

class Medium;
//! Constant Medium Pointer
using ConstMediumPtr = std::shared_ptr< const Medium>;
//! Medium Pointer
using MediumPtr = std::shared_ptr< Medium>;
//! Media
using Media = std::map< uint8_t, MediumPtr>;
//! Const Media
using ConstMedia = std::map< uint8_t, ConstMediumPtr>;

class Directory;
//! constant directory Pointer
using ConstDirectoryPtr = std::shared_ptr< const Directory>;
//! directory Pointer
using DirectoryPtr = std::shared_ptr< Directory>;
//! weak directory Pointer
using WeakDirectoryPtr = std::weak_ptr< Directory>;
//! directories list
using Directories = std::list< DirectoryPtr>;
//! constant directories list
using ConstDirectories = std::list< ConstDirectoryPtr>;

class PartNumberEntity;
class BaseFile;
class File;
class Load;
class Batch;

//! Pointer to const BaseFile
using ConstBaseFilePtr = std::shared_ptr< const BaseFile>;
//! Pointer to BaseFile
using BaseFilePtr = std::shared_ptr< BaseFile>;
//! Weak pointer to BaseFile
using WeakBaseFilePtr = std::weak_ptr< BaseFile>;
//! files list
using Files = std::list< BaseFilePtr>;
//! constant files list
using ConstFiles = std::list< ConstBaseFilePtr>;

//! Pointer to const BaseFile
using ConstFilePtr = std::shared_ptr< const BaseFile> ;
//! Pointer to BaseFile
using FilePtr = std::shared_ptr< BaseFile>;
//! Weak pointer to BaseFile
using WeakFilePtr = std::weak_ptr< BaseFile>;

//! constant load Pointer
using ConstLoadPtr = std::shared_ptr< const Load>;
//! load pointer
using LoadPtr = std::shared_ptr< Load>;
//! weak load Pointer
using WeakLoadPtr = std::weak_ptr< Load>;
//! constant weak load Pointer
using ConstWeakLoadPtr = std::weak_ptr< const Load>;
//! loads list
using Loads = std::list< LoadPtr>;
//! constant loads list
using ConstLoads = std::list< ConstLoadPtr>;
//! weak loads list
using WeakLoads = std::list< WeakLoadPtr>;
//! constant weak loads list
using ConstWeakLoads = std::list< ConstWeakLoadPtr>;

//! constant batch Pointer
using ConstBatchPtr = std::shared_ptr< const Batch> ;
//! batch pointer
using BatchPtr = std::shared_ptr< Batch> ;
//! batches list
using Batches = std::list< BatchPtr>;
//! constant batches list
using ConstBatches = std::list< ConstBatchPtr>;

//! Tuple of directory an const file
using ConstDirectoryFileTuple  = std::tuple< const Directory&, ConstFilePtr>;
//! Tuple of directory an file
using DirectoryFileTuple = std::tuple< Directory&, FilePtr>;

}

#endif

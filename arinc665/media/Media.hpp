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

class ContainerEntity;
//! Constant Container Entity Pointer
using ConstContainerEntityPtr = std::shared_ptr< const ContainerEntity>;
//! Container Entity Pointer
using ContainerEntityPtr = std::shared_ptr< ContainerEntity>;

class Medium;
//! Constant Medium Pointer
using ConstMediumPtr = std::shared_ptr< const Medium >;
//! Medium Pointer
using MediumPtr = std::shared_ptr< Medium >;
//! Media
using Media = std::map< uint8_t, MediumPtr >;
//! Const Media
using ConstMedia = std::map< uint8_t, ConstMediumPtr >;

class Directory;
//! Constant Directory Pointer
using ConstDirectoryPtr = std::shared_ptr< const Directory >;
//! Directory Pointer
using DirectoryPtr = std::shared_ptr< Directory >;
//! Constant Directories (List)
using ConstDirectories = std::list< ConstDirectoryPtr >;
//! Directories (List)
using Directories = std::list< DirectoryPtr >;

class File;
class RegularFile;
class Load;
class Batch;

//! Pointer to const File
using ConstFilePtr = std::shared_ptr< const File >;
//! Pointer to File
using FilePtr = std::shared_ptr< File >;
//! Files (List)
using Files = std::list< FilePtr >;
//! Constant Files (List)
using ConstFiles = std::list< ConstFilePtr>;

//! Pointer to Const Regular File
using ConstRegularFilePtr = std::shared_ptr< const RegularFile > ;
//! Pointer to regular File
using RegularFilePtr = std::shared_ptr< RegularFile >;
//! Regular Files (List)
using RegularFiles = std::list< RegularFilePtr >;
//! Constant Regular Files (List)
using ConstRegularFiles = std::list< ConstRegularFilePtr>;

//! Constant Load Pointer
using ConstLoadPtr = std::shared_ptr< const Load >;
//! Load Pointer
using LoadPtr = std::shared_ptr< Load >;
//! Loads List
using Loads = std::list< LoadPtr >;
//! Constant Loads List
using ConstLoads = std::list< ConstLoadPtr >;
//! Weak Loads List
using WeakLoads = std::list< LoadPtr::weak_type >;

//! Constant Batch Pointer
using ConstBatchPtr = std::shared_ptr< const Batch > ;
//! Batch Pointer
using BatchPtr = std::shared_ptr< Batch > ;
//! Batches List
using Batches = std::list< BatchPtr >;
//! Constant Batches List
using ConstBatches = std::list< ConstBatchPtr >;

//! Tuple of directory an const file
using ConstDirectoryFileTuple  = std::tuple< const Directory&, ConstFilePtr>;
//! Tuple of directory an file
using DirectoryFileTuple = std::tuple< Directory&, FilePtr>;

//! Load File List ( File, Part Number). Used for Data and Support Files of Load
using LoadFiles = std::list< std::pair< FilePtr, std::string > >;
//! Weak Load File List ( File, Part Number). Used for Data and Support Files of Load
using WeakLoadFiles = std::list< std::pair< FilePtr::weak_type, std::string > >;

}

#endif

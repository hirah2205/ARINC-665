/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of namespace and forward declarations.
 **/

#ifndef ARINC665_MEDIA_MEDIA_HPP
#define ARINC665_MEDIA_MEDIA_HPP

#include <arinc665/Arinc665.hpp>

#include <string>
#include <map>
#include <list>
#include <memory>

namespace Arinc665 {

/**
 *
 **/
namespace Media {

class MediaSet;
typedef std::shared_ptr< MediaSet> MediaSetPtr;

class Medium;
typedef std::shared_ptr< const Medium> ConstMediumPtr;
typedef std::shared_ptr< Medium> MediumPtr;
typedef std::map< unsigned int, MediumPtr> MediaMap;

class Directory;
typedef std::shared_ptr< const Directory> ConstDirectoryPtr;
typedef std::shared_ptr< Directory> DirectoryPtr;
typedef std::weak_ptr< Directory> WeakDirectoryPtr;
typedef std::map< std::string, DirectoryPtr> DirectoryMap;
typedef std::map< std::string, ConstDirectoryPtr> ConstDirectoryMap;

class PartNumberEntity;
class File;
class Load;
class Batch;

typedef std::shared_ptr< const File> ConstFilePtr;
typedef std::shared_ptr< File> FilePtr;
typedef std::weak_ptr< File> WeakFilePtr;

typedef std::shared_ptr< const Load> ConstLoadPtr;
typedef std::shared_ptr< Load> LoadPtr;

typedef std::shared_ptr< const Batch> ConstBatchPtr;
typedef std::weak_ptr< Load> WeakLoadPtr;
typedef std::shared_ptr< Batch> BatchPtr;

typedef std::map< std::string, FilePtr> FileMap;
typedef std::map< std::string, LoadPtr> LoadMap;
typedef std::map< std::string, BatchPtr> BatchMap;

typedef std::map< std::string, ConstFilePtr> ConstFileMap;
typedef std::map< std::string, ConstLoadPtr> ConstLoadMap;
typedef std::map< std::string, ConstBatchPtr> ConstBatchMap;

typedef std::tuple< const Directory&, ConstFilePtr> ConstDirectoryFileTuple;
typedef std::tuple< Directory&, FilePtr> DirectoryFileTuple;
typedef std::tuple< const Medium&, const Directory&, ConstFilePtr> ConstMediumDirectoryFileTuple;
typedef std::tuple< Medium&, Directory&, FilePtr> MediumDirectoryFileTuple;

}
}

#endif

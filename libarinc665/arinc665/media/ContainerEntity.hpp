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
 * @brief Declaration of class Arinc665::ContainerEntity.
 **/

#ifndef ARINC665_MEDIA_CONTAINERENTITY_HPP
#define ARINC665_MEDIA_CONTAINERENTITY_HPP

#include <arinc665/media/Media.hpp>

#include <arinc665/media/Base.hpp>

#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <string>

namespace Arinc665 {
namespace Media {

/**
 *
 **/
class ContainerEntity :
  public Base,
  public std::enable_shared_from_this< ContainerEntity>
{
  public:
    using string = std::string;

    ContainerEntity( void) = default;

    bool hasChildren( void) const;

    size_t getNumberOfSubDirectories( void) const;

    ConstDirectories getSubDirectories( void) const;

    Directories getSubDirectories( void);

    ConstDirectoryPtr getSubDirectory( const string &name) const;

    DirectoryPtr getSubDirectory( const string &name);

    DirectoryPtr addSubDirectory( const string &name);

    void removeSubDirectory( const string &name);

    void removeSubDirectory( DirectoryPtr subDirectory);

    /**
     * @brief Returns the number of files within this directory and
     *   sub-directories.
     *
     * @return The number of files within this directory.
     **/
    size_t getNumberOfFiles( bool recursive = false) const;

    ConstFiles getFiles( bool recursive = false) const;

    Files getFiles( bool recursive = false);

    ConstFilePtr getFile( const string &filename, bool recursive = false) const;

    FilePtr getFile( const string &filename, bool recursive = false);

    /**
     * @brief Adds a file into this directory.
     *
     * @param filename
     *
     * @return
     **/
    FilePtr addFile( const string &filename);

    void removeFile( const string &filename);

    void removeFile( ConstFilePtr file);


    size_t getNumberOfLoads( bool recursive = false) const;

    ConstLoads getLoads( bool recursive = false) const;

    Loads getLoads( bool recursive = false);

    ConstLoadPtr getLoad( const string &filename, bool recursive = false) const;

    LoadPtr getLoad( const string &filename, bool recursive = false);

    LoadPtr addLoad( const string &filename);

    void removeLoad( const string &filename);

    void removeLoad( ConstLoadPtr load);


    size_t getNumberOfBatches( bool recursive = false) const;

    ConstBatches getBatches( bool recursive = false) const;

    Batches getBatches( bool recursive = false);

    ConstBatchPtr getBatch( const string &filename, bool recursive = false) const;

    BatchPtr getBatch( const string &filename, bool recursive = false);

    BatchPtr addBatch( const string &filename);

    void removeBatch( const string &filename);

    void removeBatch( ConstBatchPtr batch);

    ContainerEntityPtr getParent( void);

    ConstContainerEntityPtr getParent( void) const;

  protected:
    ContainerEntity( ContainerEntityPtr parent);

    ConstFiles getFiles( BaseFile::FileType fileType) const;

    Files getFiles( BaseFile::FileType fileType);

    void setParent( ContainerEntityPtr parent);

  private:
    Directories subDirectories;
    Files files;
    WeakContainerEntityPtr parent;
};
}
}

#endif

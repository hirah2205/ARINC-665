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
 * @brief Declaration of class Arinc665::Media::ContainerEntity.
 **/

#ifndef ARINC665_MEDIA_CONTAINERENTITY_HPP
#define ARINC665_MEDIA_CONTAINERENTITY_HPP

#include <arinc665/media/Media.hpp>

#include <arinc665/media/Base.hpp>

#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <boost/filesystem/path.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief Container base class.
 *
 * Contains common operations needed for containers.
 * Direct children are:
 * @li Media, and
 * @li Directory
 **/
class ContainerEntity :
  public Base,
  public std::enable_shared_from_this< ContainerEntity>
{
  public:
    //! Path type
    using path = boost::filesystem::path;

    //! Default constructor
    ContainerEntity() = default;

    /**
     * @brief Returns the path up to the medium root.
     *
     * @return The path up to the medium root.
     **/
    virtual path getPath() const = 0;

    /**
     * @brief Indicates, if the container has child elements.
     *
     * @return if there are sub-directories or files.
     **/
    bool hasChildren() const;

    /**
     * @brief Return the number of sub-directories.
     *
     * @return The number of sub-directories.
     **/
    size_t getNumberOfSubDirectories() const;

    /**
     * @brief Returns all sub-directories within the current container.
     *
     * @return All sub-directories contained in the current container.
     **/
    ConstDirectories getSubDirectories() const;

    //! @copydoc getSubDirectories() const
    Directories getSubDirectories();

    /**
     * @brief Returns the sub-directory with the given name.
     *
     * @param[in] name
     *   The name of the requested sub-directory.
     *
     * @return The sub-directory with the given name.
     * @retval {}
     *   If no such sub-directory exists.
     **/
    ConstDirectoryPtr getSubDirectory( const string &name) const;

    //! @copydoc getSubDirectory(const string&) const
    DirectoryPtr getSubDirectory( const string &name);

    /**
     * @brief Adds a sub-directory with the given name.
     *
     * It is a failure to try to create an already existing directory.
     *
     * @param[in] name
     *   The name of the requested sub-directory.
     *
     * @return The created sub-directory.
     **/
    DirectoryPtr addSubDirectory( const string &name);

    /**
     * @brief Removes the sub-directory with the given name.
     *
     * It is a failure to try to delete a non-existing directory.
     *
     * @param[in] name
     *   The name of the requested sub-directory to be deleted.
     **/
    void removeSubDirectory( const string &name);

    /**
     * @brief Removes the given sub-directory.
     *
     * It is a failure to try to delete a non-existing directory.
     *
     * @param[in] subDirectory
     *   The sub-directory to be deleted,
     **/
    void removeSubDirectory( DirectoryPtr subDirectory);

    /**
     * @brief Returns the number of files within this directory and
     *   sub-directories.
     *
     * @param[in] recursive
     *   If set to true includes files in sub-directories.
     *
     * @return The number of files within this directory.
     **/
    size_t getNumberOfFiles( bool recursive = false) const;

    /**
     * @brief Returns all files present in the given container.
     *
     * @param[in] recursive
     *   If set to true recursive scan all sub-directories.
     *
     * @return All files of the current container.
     **/
    ConstFiles getFiles( bool recursive = false) const;

    //! @copydoc getFiles(bool) const
    Files getFiles( bool recursive = false);

    /**
     * @brief Returns the file with the given name.
     *
     * The file type is not relevant (file can be load header file, batch file,
     * or other file).
     *
     * @note
     * If a file with the same name exists in multiple sub-directories exists,
     * only the first one is returned (which is the first is not specified).
     *
     * @param[in] filename
     *   The name of the requested file.
     * @param[in] recursive
     *   If set to true scans all sub-directories for the file.
     *
     * @return The file with the given name.
     * @retval {}
     *   If no such file exists.
     **/
    ConstFilePtr getFile( const string &filename, bool recursive = false) const;

    //! @copydoc getFile(const string&,bool) const
    FilePtr getFile( const string &filename, bool recursive = false);

    /**
     * @brief Adds a file into this directory.
     *
     * @param[in] filename
     *   The filename of the file to be created.
     *
     * @return The created file.
     **/
    FilePtr addFile( const string &filename);

    /**
     * @brief Removes the file with the given name.
     *
     * A non-existing file is handled as failure.
     *
     * @param[in] filename
     *   The filename of the file to be deleted.
     **/
    void removeFile( const string &filename);

    /**
     * @brief Removes the given file.
     *
     * A non-existing file (in this directory) is handled as failure.
     *
     * @param[in] file
     *   The file to be deleted.
     **/
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

    /**
     * @brief Returns the parent.
     *
     * @return The parent of this container.
     **/
    ConstContainerEntityPtr getParent() const;

    //! @copydoc getParent() const
    ContainerEntityPtr getParent();

    /**
     * @brief Returns the medium where this container is located.
     *
     * The returned medium could be the container itself.
     *
     * @return The medium where this container is located.
     **/
    ConstMediumPtr getMedium() const;

    //! @copydoc getMedium() const
    MediumPtr getMedium();

  protected:
    ContainerEntity( ContainerEntityPtr parent);

    ConstFiles getFiles( BaseFile::FileType fileType) const;

    //! @copydoc getFiles( BaseFile::FileType) const
    Files getFiles( BaseFile::FileType fileType);

    void setParent( ContainerEntityPtr parent);

  private:
    //! sub-directories
    Directories subDirectories;
    //! files
    Files files;
    //! the parent
    WeakContainerEntityPtr parent;
};

}
}

#endif

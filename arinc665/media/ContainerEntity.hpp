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
    using fpath = boost::filesystem::path;

    //! Default constructor
    ContainerEntity() = default;

    /**
     * @brief Returns the path up to the medium root.
     *
     * @return The path up to the medium root.
     **/
    virtual fpath path() const = 0;

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
    size_t numberOfSubDirectories() const;

    /**
     * @brief Returns all sub-directories within the current container.
     *
     * @return All sub-directories contained in the current container.
     **/
    ConstDirectories subDirectories() const;

    //! @copydoc subDirectories() const
    Directories subDirectories();

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
    ConstDirectoryPtr subDirectory( const string &name) const;

    //! @copydoc subDirectory(const string&) const
    DirectoryPtr subDirectory( const string &name);

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
    size_t numberOfFiles( bool recursive = false) const;

    /**
     * @brief Returns all files present in the given container.
     *
     * @param[in] recursive
     *   If set to true recursive scan all sub-directories.
     *
     * @return All files of the current container.
     **/
    ConstFiles files( bool recursive = false) const;

    //! @copydoc files(bool) const
    Files files( bool recursive = false);

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
    ConstFilePtr file( const string &filename, bool recursive = false) const;

    //! @copydoc file(const string&,bool) const
    FilePtr file( const string &filename, bool recursive = false);

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

    /**
     * @brief Return the number of loads.
     *
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return The number of loads.
     **/
    size_t numberOfLoads( bool recursive = false) const;

    /**
     * @brief Return loads.
     *
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return Loads contained within this container.
     **/
    ConstLoads loads( bool recursive = false) const;

    //! @copydoc loads(bool) const
    Loads loads( bool recursive = false);

    /**
     * @brief Returns the load with the given filename.
     *
     * @param[in] filename
     *   Load filename
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return The load with the given filename.
     * @retval {}
     *   If load does not exists.
     **/
    ConstLoadPtr load( const string &filename, bool recursive = false) const;

    //! @copydoc load(const string&,bool) const
    LoadPtr load( const string &filename, bool recursive = false);

    /**
     * @brief Creates a load with the given filename.
     *
     * @param[in] filename
     *   Load filename.
     *
     * @return Created load
     **/
    LoadPtr addLoad( const string &filename);

    /**
     * @brief Removes the load with the given filename.
     *
     * @param[in] filename
     *   Load filename.
     **/
    void removeLoad( const string &filename);

    /**
     * @brief Removes the given load.
     *
     * @param[in] load
     *   Load to be deleted.
     **/
    void removeLoad( ConstLoadPtr load);


    /**
     * @brief Return the number of batches.
     *
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return The number of loads.
     **/
    size_t numberOfBatches( bool recursive = false) const;

    /**
     * @brief Return batches.
     *
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return Batches contained within this container.
     **/
    ConstBatches batches( bool recursive = false) const;

    //! @copydoc batches(bool) const
    Batches batches( bool recursive = false);

    /**
     * @brief Returns the batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename
     * @param[in] recursive
     *   If set to true also iterates over sub-directories.
     *
     * @return The batch with the given filename.
     * @retval {}
     *   If batch does not exists.
     **/
    ConstBatchPtr batch( const string &filename, bool recursive = false) const;

    //! @copydoc batch(const string&,bool) const
    BatchPtr batch( const string &filename, bool recursive = false);

    /**
     * @brief Creates a batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename.
     *
     * @return Created batch.
     **/
    BatchPtr addBatch( const string &filename);

    /**
     * @brief Removes the batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename.
     **/
    void removeBatch( const string &filename);

    /**
     * @brief Removes the given batch.
     *
     * @param[in] batch
     *   Batch to be deleted.
     **/
    void removeBatch( ConstBatchPtr batch);

    /**
     * @brief Returns the parent.
     *
     * @return The parent of this container.
     **/
    ConstContainerEntityPtr parent() const;

    //! @copydoc parent() const
    ContainerEntityPtr parent();

    /**
     * @brief Returns the medium where this container is located.
     *
     * The returned medium could be the container itself.
     *
     * @return The medium where this container is located.
     **/
    ConstMediumPtr medium() const;

    //! @copydoc medium() const
    MediumPtr medium();

  protected:
    //! File type
    using FileType = BaseFile::FileType;

    /**
     * @brief Initialises the container with the given parameter.
     *
     * @param[in] parent
     *   Parent of this container.
     **/
    ContainerEntity( ContainerEntityPtr parent);

    /**
     * @brief Return the files (real file, load, batch) with the specified file
     *   type.
     *
     * @param[in] fileType
     *   File type to search for.
     *
     * @return The files (real file, load, batch) with the specified file.
     **/
    ConstFiles files( FileType fileType) const;

    //! @copydoc files(FileType) const
    Files files( FileType fileType);

    /**
     * @brief Updates the parent.
     *
     * @param[in] parent
     *   New parent.
     **/
    void parent( ContainerEntityPtr parent);

  private:
    //! sub-directories
    Directories subDirectoriesValue;
    //! files
    Files filesValue;
    //! the parent
    WeakContainerEntityPtr parentValue;
};

}
}

#endif

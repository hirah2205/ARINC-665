/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::ContainerEntity.
 **/

#ifndef ARINC665_MEDIA_CONTAINERENTITY_HPP
#define ARINC665_MEDIA_CONTAINERENTITY_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/Base.hpp>

#include <filesystem>
#include <string>
#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Media Set Container.
 *
 * Contains common operations needed for containers.
 * Direct children are:
 * - Media, and
 * - Directory.
 **/
class ARINC665_EXPORT ContainerEntity : public Base
{
  public:
    //! Constructor
    ContainerEntity() = default;

    /**
     * @brief Returns the path up to the medium root.
     *
     * @return The path up to the medium root.
     **/
    virtual std::filesystem::path path() const = 0;

    /**
     * @brief Indicates, if the container has child elements.
     *
     * @return if there are subdirectories or files.
     **/
    bool hasChildren() const;

    /**
     * @name Subdirectories
     * @{
     **/

    /**
     * @brief Return the number of subdirectories.
     *
     * @return Number of subdirectories.
     **/
    size_t numberOfSubdirectories() const;

    /**
     * @brief Returns all subdirectories within the current container.
     *
     * @return All subdirectories contained in the current container.
     **/
    ConstDirectories subdirectories() const;

    //! @copydoc subdirectories() const
    Directories subdirectories();

    /**
     * @brief Returns the subdirectory with the given name.
     *
     * @param[in] name
     *   Name of the requested subdirectory.
     *
     * @return Subdirectory with the given name.
     * @retval {}
     *   If no such subdirectory exists.
     **/
    ConstDirectoryPtr subdirectory( std::string_view name ) const;

    //! @copydoc subdirectory(std::string_view) const
    DirectoryPtr subdirectory( std::string_view name );

    /**
     * @brief Adds a subdirectory with the given name.
     *
     * It is a failure to try to create an already existing directory.
     *
     * @param[in] name
     *   Name of the requested subdirectory.
     *
     * @return Created subdirectory.
     *
     * @throw Arinc665Exception()
     *   If directory or name with this name already exists.
     **/
    DirectoryPtr addSubdirectory( std::string name );

    /**
     * @brief Removes the subdirectory with the given name within this entity.
     *
     * It is a failure to try to delete a non-existing directory.
     *
     * @param[in] name
     *   name of the requested subdirectory to be deleted.
     *
     * @throw Arinc665Exception()
     *   If directory does not exist.
     **/
    void removeSubdirectory( std::string_view name );

    /**
     * @brief Removes the given subdirectory within this entity.
     *
     * It is a failure to try to delete a non-existing directory.
     *
     * @param[in] subDirectory
     *   Subdirectory to be deleted.
     *
     * @throw Arinc665Exception()
     *   if directory does not exist.
     **/
    void removeSubdirectory( const DirectoryPtr& subDirectory );

    /** @} **/

    /**
     * @name Files
     * All files, including regular files, loads, and batches.
     * @{
     **/

    /**
     * @brief Returns the number of files within this directory.
     *
     * @return Number of files within this directory.
     **/
    size_t numberOfFiles() const;

    /**
     * @brief Recursively returns number of files.
     *
     * @return Number of files in container and its subdirectories.
     **/
    [[nodiscard]] size_t recursiveNumberOfFiles() const;

    /**
     * @brief Returns all files present in the given container.
     *
     * @return All files of the current container.
     **/
    ConstFiles files() const;

    //! @copydoc files() const
    Files files();

    /**
     * @brief Recursively returns all files.
     *
     * @return All files in @p container and its subdirectories.
     **/
    [[nodiscard]] ConstFiles recursiveFiles() const;

    //! @copydoc recursiveFiles() const
    [[nodiscard]] Files recursiveFiles();

    /**
     * @brief Returns the files with the given name.
     *
     * The file type is not relevant (file can be load header file, batch file,
     * or other file).
     *
     * @param[in] filename
     *   Name of the requested file.
     *
     * @return Files with the given name.
     * @retval {}
     *   If no such files exists.
     **/
    [[nodiscard]] ConstFiles recursiveFiles( std::string_view filename ) const;

    //! @copydoc recursiveFiles(std::string_view) const
    [[nodiscard]] Files recursiveFiles( std::string_view filename );

    /**
     * @brief Returns the file with the given name.
     *
     * The file type is not relevant (file can be load header file, batch file,
     * or other file).
     *
     * @param[in] filename
     *   Name of the requested file.
     *
     * @return The file with the given name.
     * @retval {}
     *   If no such file exists.
     **/
    [[nodiscard]] ConstFilePtr file( std::string_view filename ) const;

    //! @copydoc file(const std::string_view) const
    [[nodiscard]] FilePtr file( std::string_view filename );

    /**
     * @brief Removes the file with the given name.
     *
     * A non-existing file is handled as failure.
     *
     * @param[in] filename
     *   Filename of the file to be deleted.
     *
     * @throw Arinc665Exception() if file does not exists.
     **/
    void removeFile( std::string_view filename );

    /**
     * @brief Removes the given file.
     *
     * A non-existing file (in this directory) is handled as failure.
     *
     * @param[in] file
     *   File to be deleted.
     *
     * @throw Arinc665Exception() if file does not exists.
     **/
    void removeFile( const ConstFilePtr &file );

    /** @} **/

    /**
     * @name Regular Files
     * @{
     **/

    /**
     * @brief Return the number of Regular Files.
     *
     * @return Number of Regular Files.
     **/
    [[nodiscard]] size_t numberOfRegularFiles() const;

    /**
     * @brief Recursively returns number of regular files.
     *
     * @return Number of regular files in container and its subdirectories.
     **/
    [[nodiscard]] size_t recursiveNumberOfRegularFiles() const;

    /**
     * @brief Return Regular Files.
     *
     * @return Regular Files contained within this container.
     **/
    [[nodiscard]] ConstRegularFiles regularFiles() const;

    //! @copydoc regularFiles() const
    [[nodiscard]] RegularFiles regularFiles();

    /**
     * @brief Recursively returns all regular files.
     *
     * @return All regular files in @p container and its subdirectories.
     **/
    [[nodiscard]] ConstRegularFiles recursiveRegularFiles() const;

    //! @copydoc recursiveRegularFiles() const
    [[nodiscard]] RegularFiles recursiveRegularFiles();

    /**
     * @brief Returns the regular files with the given name.
     *
     * @param[in] filename
     *   Name of the requested regular file.
     *
     * @return The regular files with the given name.
     * @retval {}
     *   If no such files exists.
     **/
    [[nodiscard]] ConstRegularFiles recursiveRegularFiles(
      std::string_view filename ) const;

    //! @copydoc recursiveRegularFiles(std::string_view) const
    [[nodiscard]] RegularFiles recursiveRegularFiles(
      std::string_view filename );

    /**
     * @brief Returns the Regular File with the given filename.
     *
     * @param[in] filename
     *   Load filename
     *
     * @return Load with the given filename.
     * @retval {}
     *   If load does not exists.
     **/
    [[nodiscard]] ConstRegularFilePtr regularFile(
      std::string_view filename ) const;

    //! @copydoc regularFile(std::string_view) const
    [[nodiscard]] RegularFilePtr regularFile( std::string_view filename );

    /**
     * @brief Adds a Regular File into this container.
     *
     * @param[in] filename
     *   Filename of the file to be created.
     *
     * @return Created file.
     *
     * @throw Arinc665Exception
     *   When file already exist.
     **/
    RegularFilePtr addRegularFile( std::string filename );

    /** @} **/

    /**
     * @name Loads
     * @{
     **/

    /**
     * @brief Return the number of Loads.
     *
     * @return Number of Loads.
     **/
    [[nodiscard]] size_t numberOfLoads() const;

    /**
     * @brief Recursively returns number of Loads.
     *
     * @return Number of Loads in container and its subdirectories.
     **/
    [[nodiscard]] size_t recursiveNumberOfLoads() const;

    /**
     * @brief Return loads.
     *
     * @return Loads contained within this container.
     **/
    [[nodiscard]] ConstLoads loads() const;

    //! @copydoc loads() const
    [[nodiscard]] Loads loads();

    /**
     * @brief Recursively returns all Loads.
     *
     * @return All Loads in container and its subdirectories.
     **/
    [[nodiscard]] ConstLoads recursiveLoads() const;

    //! @copydoc recursiveLoads() const
    [[nodiscard]] Loads recursiveLoads();

    /**
     * @brief Returns the Loads with the given name.
     *
     * @param[in] filename
     *   Name of the requested Load.
     *
     * @return Loads with the given name.
     * @retval {}
     *   If no such loads exists.
     **/
    [[nodiscard]] ConstLoads recursiveLoads(
      std::string_view filename ) const;

    //! @copydoc recursiveLoads(std::string_view) const
    [[nodiscard]] Loads recursiveLoads( std::string_view filename );

    /**
     * @brief Returns the load with the given filename.
     *
     * @param[in] filename
     *   Load filename
     *
     * @return The load with the given filename.
     * @retval {}
     *   If load does not exists.
     **/
    [[nodiscard]] ConstLoadPtr load( std::string_view filename ) const;

    //! @copydoc load(std::string_view) const
    [[nodiscard]] LoadPtr load( std::string_view filename );

    /**
     * @brief Creates a load with the given filename.
     *
     * @param[in] filename
     *   Load filename.
     *
     * @return Created load.
     *
     * @throw Arinc665Exception
     *   When a file with given filename already exist.
     **/
    [[nodiscard]] LoadPtr addLoad( std::string filename );

    /** @} **/

    /**
     * @name Batches
     * @{
     **/

    /**
     * @brief Return the number of Batches.
     *
     * @return Number of Batches.
     **/
    [[nodiscard]] size_t numberOfBatches() const;

    /**
     * @brief Recursively returns number of Batches.
     *
     * @return Number of Batches in container and its subdirectories.
     **/
    [[nodiscard]] size_t recursiveNumberOfBatches() const;

    /**
     * @brief Return batches.
     *
     * @return Batches contained within this container.
     **/
    [[nodiscard]] ConstBatches batches() const;

    //! @copydoc batches() const
    [[nodiscard]] Batches batches();

    /**
     * @brief Recursively returns all Batches.
     *
     * @return All Batches in container and its subdirectories.
     **/
    [[nodiscard]] ConstBatches recursiveBatches() const;

    //! @copydoc recursiveBatches() const
    [[nodiscard]] Batches recursiveBatches();

    /**
     * @brief Returns the Batches with the given name.
     *
     * @param[in] filename
     *   Name of the requested Batch.
     *
     * @return Batches with the given name.
     * @retval {}
     *   If no such batches exists.
     **/
    [[nodiscard]] ConstBatches recursiveBatches(
      std::string_view filename ) const;

    //! @copydoc recursiveBatches(std::string_view) const
    [[nodiscard]] Batches recursiveBatches( std::string_view filename );

    /**
     * @brief Returns the batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename
     *
     * @return The batch with the given filename.
     * @retval {}
     *   If batch does not exist.
     **/
    [[nodiscard]] ConstBatchPtr batch( std::string_view filename ) const;

    //! @copydoc batch(std::string_view) const
    [[nodiscard]] BatchPtr batch( std::string_view filename );

    /**
     * @brief Creates a batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename.
     *
     * @return Created batch.
     *
     * @throw Arinc665Exception
     *   When a file with given filename already exist.
     **/
    [[nodiscard]] BatchPtr addBatch( std::string filename );

    /** @} **/

    /**
     * @brief Returns the parent.
     *
     * @return Parent of this container.
     **/
    [[nodiscard]] virtual ConstContainerEntityPtr parent() const = 0;

    //! @copydoc parent() const
    [[nodiscard]] virtual ContainerEntityPtr parent() = 0;

    /**
     * @brief Returns the medium where this container is located.
     *
     * The returned medium could be the container itself.
     *
     * @return The medium where this container is located.
     **/
    [[nodiscard]] virtual ConstMediumPtr medium() const = 0;

    //! @copydoc medium() const
    [[nodiscard]] virtual MediumPtr medium() = 0;

  protected:
    /**
     * @brief Return the number of files (real file, load, batch) with the
     *   specified file type.
     *
     * @param[in] fileType
     *   File type to search for.
     *
     * @return Number of files of specific type.
     **/
    [[nodiscard]] size_t numberOfFiles( FileType fileType ) const;

    /**
     * @brief Return the files (real file, load, batch) with the specified file
     *   type.
     *
     * @tparam FilesT
     *   Files List Type
     * @tparam fileType
     *   File type to search for.
     *
     * @return Files (real file, load, batch) with the specified file.
     **/
    template< typename FilesT, FileType fileType >
    [[nodiscard]] FilesT filesPerType() const;

    //! @copydoc filesPerType() const
    template< typename FilesT, FileType fileType >
    [[nodiscard]] FilesT filesPerType();

    /**
     * @brief Return the file (real file, load, batch) with the specified file
     *   type.
     *
     * @tparam FilesT
     *   Files List Type
     * @tparam fileType
     *   File type to search for.
     *
     * @param[in] filename
     *   Filename
     *
     * @return File (real file, load, batch) with the given filename.
     **/
    template< typename FilesT, FileType fileType >
    [[nodiscard]] FilesT filePerType( std::string_view filename ) const;

    //! @copydoc filePerType(std::string_view) const
    template< typename FilesT, FileType fileType >
    [[nodiscard]] FilesT filePerType( std::string_view filename );

  private:
    //! Subdirectories
    Directories subdirectoriesV;
    //! Files
    Files filesV;
};

}

#endif

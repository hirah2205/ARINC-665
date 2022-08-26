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

#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

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
    DirectoryPtr addSubdirectory( std::string_view name );

    /**
     * @brief Removes the subdirectory with the given name.
     *
     * It is a failure to try to delete a non-existing directory.
     *
     * @param[in] name
     *   The name of the requested subdirectory to be deleted.
     *
     * @throw Arinc665Exception()
     *   If directory does not exist.
     **/
    void removeSubdirectory( std::string_view name );

    /**
     * @brief Removes the given subdirectory.
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
     * @brief Returns all files present in the given container.
     *
     * @return All files of the current container.
     **/
    ConstFiles files() const;

    //! @copydoc files() const
    Files files();

    /**
     * @brief Returns the file with the given name.
     *
     * The file type is not relevant (file can be load header file, batch file,
     * or other file).
     *
     * @note
     * If a file with the same name exists in multiple subdirectories exists,
     * only the first one is returned (which is the first is not specified).
     *
     * @param[in] filename
     *   The name of the requested file.
     * @param[in] recursive
     *   If set to true scans all subdirectories for the file.
     *
     * @return The file with the given name.
     * @retval {}
     *   If no such file exists.
     **/
    [[nodiscard]] ConstFilePtr file(
      std::string_view filename,
      bool recursive = false ) const;

    //! @copydoc file(const std::string_view,bool) const
    [[nodiscard]] FilePtr file(
      std::string_view filename,
      bool recursive = false );

    /**
     * @brief Removes the file with the given name.
     *
     * A non-existing file is handled as failure.
     *
     * @param[in] filename
     *   The filename of the file to be deleted.
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
    void removeFile( const ConstFilePtr& file );

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
     * @brief Return Regular Files.
     *
     * @return Regular Files contained within this container.
     **/
    [[nodiscard]] ConstRegularFiles regularFiles() const;

    //! @copydoc regularFiles() const
    [[nodiscard]] RegularFiles regularFiles();

    /**
     * @brief Returns the Regular File with the given filename.
     *
     * @param[in] filename
     *   Load filename
     * @param[in] recursive
     *   If set to true also iterates over subdirectories.
     *
     * @return The load with the given filename.
     * @retval {}
     *   If load does not exists.
     **/
    [[nodiscard]] ConstRegularFilePtr regularFile(
      std::string_view filename,
      bool recursive = false ) const;

    //! @copydoc regularFile(std::string_view,bool) const
    [[nodiscard]] RegularFilePtr regularFile(
      std::string_view filename,
      bool recursive = false );

    /**
     * @brief Adds a regular file into this directory.
     *
     * @param[in] filename
     *   Filename of the file to be created.
     *
     * @return Created file.
     *
     * @throw Arinc665Exception
     *   When file already exist.
     **/
    RegularFilePtr addRegularFile( std::string_view filename );

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
     * @brief Return loads.
     *
     * @return Loads contained within this container.
     **/
    [[nodiscard]] ConstLoads loads() const;

    //! @copydoc loads() const
    [[nodiscard]] Loads loads();

    /**
     * @brief Returns the load with the given filename.
     *
     * @param[in] filename
     *   Load filename
     * @param[in] recursive
     *   If set to true also iterates over subdirectories.
     *
     * @return The load with the given filename.
     * @retval {}
     *   If load does not exists.
     **/
    [[nodiscard]] ConstLoadPtr load(
      std::string_view filename,
      bool recursive = false ) const;

    //! @copydoc load(std::string_view,bool) const
    [[nodiscard]] LoadPtr load(
      std::string_view filename,
      bool recursive = false );

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
    [[nodiscard]] LoadPtr addLoad( std::string_view filename );

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
     * @brief Return batches.
     *
     * @return Batches contained within this container.
     **/
    [[nodiscard]] ConstBatches batches() const;

    //! @copydoc batches() const
    [[nodiscard]] Batches batches();

    /**
     * @brief Returns the batch with the given filename.
     *
     * @param[in] filename
     *   Batch filename
     * @param[in] recursive
     *   If set to true also iterates over subdirectories.
     *
     * @return The batch with the given filename.
     * @retval {}
     *   If batch does not exist.
     **/
    [[nodiscard]] ConstBatchPtr batch(
      std::string_view filename,
      bool recursive = false ) const;

    //! @copydoc batch(std::string_view,bool) const
    [[nodiscard]] BatchPtr batch(
      std::string_view filename,
      bool recursive = false );

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
    [[nodiscard]] BatchPtr addBatch( std::string_view filename );

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
    //! File type
    using FileType = File::FileType;

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
     * @param[in] fileType
     *   File type to search for.
     *
     * @return Files (real file, load, batch) with the specified file.
     **/
    [[nodiscard]] ConstFiles files( FileType fileType ) const;

    //! @copydoc files(FileType) const
    [[nodiscard]] Files files( FileType fileType );

  private:
    //! Subdirectories
    Directories subdirectoriesV;
    //! Files
    Files filesV;
};

}

#endif

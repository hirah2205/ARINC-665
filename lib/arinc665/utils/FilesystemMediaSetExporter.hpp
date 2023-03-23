/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetExporter.
 **/

#ifndef ARINC665_UTILS_FILESYSTEMMEDIASETEXPORTER_HPP
#define ARINC665_UTILS_FILESYSTEMMEDIASETEXPORTER_HPP

#include <arinc665/utils/Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Set Exporter using local Filesystem.
 *
 * This exporter expects that each medium is contained as directory on the local
 * filesystem.
 * Normal filesystem access routines are used to access the media and files.
 *
 * @sa @ref MediaSetExporter
 **/
class ARINC665_EXPORT FilesystemMediaSetExporter
{
  public:
    /**
     * @brief Creates the ARINC 665 %Media Set Exporter Instance.
     *
     * @return ARINC 665 %Media Set Exporter Instance
     **/
    static FilesystemMediaSetExporterPtr create();

    //! Destructor
    virtual ~FilesystemMediaSetExporter() = default;

    /**
     * @brief Sets the Media Set to export.
     *
     * @param[in] mediaSet
     *   Media Set, which shall be exported.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetExporter& mediaSet(
      Media::ConstMediaSetPtr mediaSet ) = 0;

    /**
     * @brief Sets the ARINC 665 Version Flag.
     *
     * @param[in] version
     *   ARINC 665 version used for exporting.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetExporter& arinc665Version(
      SupportedArinc665Version version ) = 0;

    /**
     * @brief Sets the Create Batch Files Flag.
     *
     * @param[in] createBatchFiles
     *   Defines, if Batch Files are created by exporter or pre-existing ones
     *   are used.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetExporter& createBatchFiles(
      FileCreationPolicy createBatchFiles ) = 0;

    /**
     * @brief Sets the Create Load Header Files Flag.
     *
     * @param[in] createLoadHeaderFiles
     *   Defines, if Load Header Files are created by exporter or pre-existing
     *   ones are used.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetExporter& createLoadHeaderFiles(
      FileCreationPolicy createLoadHeaderFiles ) = 0;

    /**
     * @brief Updates the base directory of the Media Set, where it will be
     *   created.
     *
     * @param[in] mediaSetBasePath
     *   Media Set Base directory.
     *
     * @return *this for chaining.
     **/
    virtual FilesystemMediaSetExporter& mediaSetBasePath(
      std::filesystem::path mediaSetBasePath ) = 0;

    /**
     * @brief Updates the base directory for source files, if the path within
     *   the file mapping table is relative.
     *
     * @param[in] sourceBasePath
     *   Sources Base directory.
     *
     * @return *this for chaining.
     *
     * @sa @ref FilesystemMediaSetExporter::filePathMapping
     **/
    virtual FilesystemMediaSetExporter& sourceBasePath(
      std::filesystem::path sourceBasePath ) = 0;

    /**
     * @brief Updates the File Mapping Table.
     *
     * This file mapping table set the paths for the Media Set Files.
     *
     * @param[in] filePathMapping
     *   File mapping table
     *
     * @return *this for chaining.
     *
     * @sa @ref FilesystemMediaSetExporter::filePathMapping
     **/
    virtual FilesystemMediaSetExporter& filePathMapping(
      FilePathMapping filePathMapping ) = 0;

    /**
     * @brief Executes the ARINC 665 Media Set Exporter.
     *
     * All parameters must have been set previously.
     *
     * @return Media Paths relative to Media Set Base Path.
     *
     * @throw Arinc665Exception
     *   When export fails
     **/
    virtual MediaPaths operator()() = 0;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetExporterImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_
#define ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_

#include <arinc665/utils/Arinc665Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a Media Set Exporter.
 **/
class MediaSetExporterImpl final
{
  public:
    /**
     * @brief Initialises the ARINC 665 Media Set Importer
     *
     * @param[in] mediaSet
     *   The media set, which shall be exported.
     * @param[in] createMediumHandler
     *   Called to create the given medium.
     * @param[in] createDirectoryHandler
     *   Called to create the given directory.
     * @param[in] checkFileExistenceHandler
     *   Called to check if desired file exists
     * @param[in] createFileHandler
     *   When a file needs to be generated, this handler is called.
     * @param[in] writeFileHandler
     *   Writes a given file to the output media set.
     * @param[in] readFileHandler
     *   Reads a given file from the output media set.
     *   Used for CRC calculation.
     * @param[in] arinc665Version
     *   The ARINC 665 version used for exporting
     * @param[in] createBatchFiles
     *   Defines, if Batch Files are created by exporter or pre-existing ones
     *   are used.
     * @param[in] createLoadHeaderFiles
     *   Defines, if Load Header Files are created by exporter or pre-existing
     *   ones are used.
     **/
    MediaSetExporterImpl(
      Media::ConstMediaSetPtr mediaSet,
      Arinc665Utils::CreateMediumHandler createMediumHandler,
      Arinc665Utils::CreateDirectoryHandler createDirectoryHandler,
      Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler,
      Arinc665Utils::CreateFileHandler createFileHandler,
      Arinc665Utils::WriteFileHandler writeFileHandler,
      Arinc665Utils::ReadFileHandler readFileHandler,
      SupportedArinc665Version arinc665Version,
      FileCreationPolicy createBatchFiles,
      FileCreationPolicy createLoadHeaderFiles );

    /**
     * @brief Entry-point of the ARINC 665 media set exporter.
     ***/
    void operator()();

  private:
    /**
     * @brief Called to export the given medium.
     *
     * @param[in] medium
     *   Medium, which is exported.
     **/
    void exportMedium( const Media::ConstMediumPtr &medium );

    /**
     * @brief Called to export the given directory.
     *
     * @param[in] directory
     *   Directory, which is exported.
     **/
    void exportDirectory( const Media::ConstDirectoryPtr &directory );

    /**
     * @brief Called to Export the Given File.
     *
     * @param[in] file
     *   File, which is exported.
     **/
    void exportFile( const Media::ConstFilePtr &file ) const;

    /**
     * @brief Creates the Load Header File.
     *
     * @param[in] loadFile
     *   Load header file.
     **/
    void createLoadHeaderFile( const Media::ConstFilePtr &loadFile ) const;

    /**
     * @brief Creates the Batch File.
     *
     * @param[in] file
     *   Batch file.
     **/
    void createBatchFile( const Media::ConstFilePtr &file ) const;

    //! ARINC 665 Version used for exporting
    const SupportedArinc665Version arinc665Version;
    //! Indicates if batch files shall be created by Media set Exporter
    const FileCreationPolicy createBatchFiles;
    //! Indicates if load header files shall be created by Media Set Exporter
    const FileCreationPolicy createLoadHeaderFiles;

    //! Media Set
    Media::ConstMediaSetPtr mediaSet;
    //! Create Medium Handler
    Arinc665Utils::CreateMediumHandler createMediumHandler;
    //! Create Directory Handler
    Arinc665Utils::CreateDirectoryHandler createDirectoryHandler;
    //! Check File existence Handler
    Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler;
    //! Create File Handler
    Arinc665Utils::CreateFileHandler createFileHandler;
    //! Write File Handler
    Arinc665Utils::WriteFileHandler writeFileHandler;
    //! Read File Handler
    Arinc665Utils::ReadFileHandler readFileHandler;
};

}

#endif

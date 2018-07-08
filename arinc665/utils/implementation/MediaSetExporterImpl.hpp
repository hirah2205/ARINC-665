/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Utils::MediaSetExporterImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_
#define ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP_

#include <arinc665/utils/Arinc665Utils.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a media set exporter.
 **/
class MediaSetExporterImpl
{
  public:
    //! Path type
    using path = Arinc665Utils::path;

    /**
     * @brief Initialises the ARINC 665 media set importer
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
     *   If set to true, Batch Files are created by exporter.
     * @param[in] createLoadHeaderFiles
     *   If set to true, Load Header Files are created by exporter.
     **/
    MediaSetExporterImpl(
      Media::ConstMediaSetPtr mediaSet,
      Arinc665Utils::CreateMediumHandler createMediumHandler,
      Arinc665Utils::CreateDirectoryHandler createDirectoryHandler,
      Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler,
      Arinc665Utils::CreateFileHandler createFileHandler,
      Arinc665Utils::WriteFileHandler writeFileHandler,
      Arinc665Utils::ReadFileHandler readFileHandler,
      Arinc665Version arinc665Version,
      FileCreationPolicy createBatchFiles,
      FileCreationPolicy createLoadHeaderFiles);

    /**
     * @brief Entry-point of the ARINC 665 media set exporter.
     ***/
    void operator()();

  private:
    /**
     * @brief Called to export the given medium.
     *
     * @param[in] medium
     *   The medium, which is exported.
     **/
    void exportMedium( Media::ConstMediumPtr medium);

    /**
     * @brief Called to export the given directory.
     *
     * @param[in] directory
     *   The directory, which is exported.
     **/
    void exportDirectory( Media::ConstDirectoryPtr directory);

    /**
     * @brief Called to export the given file.
     *
     * @param[in] file
     *   The file, which is exported.
     **/
    void exportFile( Media::ConstFilePtr file);

    /**
     * @brief Creates the load header file.
     *
     * @param[in] file
     *   The load header file.
     **/
    void createLoadHeaderFile( Media::ConstFilePtr file);

    /**
     * @brief Creates the batch file.
     *
     * @param[in] file
     *   The batch file.
     **/
    void createBatchFile( Media::ConstFilePtr file);

    //! The ARINC 665 version used for exporting
    const Arinc665Version arinc665Version;
    //! indicates if batch files shall be created by media set exporter
    const FileCreationPolicy createBatchFiles;
    //! indicates if load header files shall be created by media set exporter
    const FileCreationPolicy createLoadHeaderFiles;

    //! The Media Set
    Media::ConstMediaSetPtr mediaSet;
    //! Create medium handler
    Arinc665Utils::CreateMediumHandler createMediumHandler;
    //! Create directory handler
    Arinc665Utils::CreateDirectoryHandler createDirectoryHandler;
    //! Check file existence handler
    Arinc665Utils::CheckFileExistenceHandler checkFileExistenceHandler;
    //! Create file handler
    Arinc665Utils::CreateFileHandler createFileHandler;
    //! Write file handler
    Arinc665Utils::WriteFileHandler writeFileHandler;
    //! Read file handler
    Arinc665Utils::ReadFileHandler readFileHandler;
};

}

#endif

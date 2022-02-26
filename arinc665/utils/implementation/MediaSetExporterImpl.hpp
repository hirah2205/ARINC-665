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

#include <arinc665/utils/MediaSetExporter.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a Media Set Exporter.
 **/
class MediaSetExporterImpl final : public MediaSetExporter
{
  public:
    /**
     * @brief Initialises the ARINC 665 Media Set Importer
     **/
    MediaSetExporterImpl();

    //! @copydoc MediaSetExporter::mediaSet()
    MediaSetExporter& mediaSet( Media::ConstMediaSetPtr mediaSet ) final;

    //! @copydoc MediaSetExporter::createMediumHandler()
    MediaSetExporter& createMediumHandler(
      CreateMediumHandler createMediumHandler ) final;

    //! @copydoc MediaSetExporter::createDirectoryHandler()
    MediaSetExporter& createDirectoryHandler(
      CreateDirectoryHandler createDirectoryHandler ) final;

    //! @copydoc MediaSetExporter::checkFileExistenceHandler()
    MediaSetExporter& checkFileExistenceHandler(
      CheckFileExistenceHandler checkFileExistenceHandler ) final;

    //! @copydoc MediaSetExporter::createFileHandler()
    MediaSetExporter& createFileHandler(
      CreateFileHandler createFileHandler ) final;

    //! @copydoc MediaSetExporter::writeFileHandler()
    MediaSetExporter& writeFileHandler(
      WriteFileHandler writeFileHandler ) final;

    //! @copydoc MediaSetExporter::readFileHandler()
    MediaSetExporter& readFileHandler(
      ReadFileHandler readFileHandler ) final;

    //! @copydoc MediaSetExporter::version()
    MediaSetExporter& arinc665Version( SupportedArinc665Version version ) final;

    //! @copydoc MediaSetExporter::createBatchFiles()
    MediaSetExporter& createBatchFiles(
      FileCreationPolicy createBatchFiles ) final;

    //! @copydoc MediaSetExporter::createLoadHeaderFiles()
    MediaSetExporter& createLoadHeaderFiles(
      FileCreationPolicy createLoadHeaderFiles ) final;

    /**
     * @brief Entry-point of the ARINC 665 Media Set Exporter.
     ***/
    void operator()() final;

  private:
    /**
     * @brief Called to Export the Given Medium.
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
    SupportedArinc665Version arinc665VersionV;
    //! Indicates if batch files shall be created by Media set Exporter
    FileCreationPolicy createBatchFilesV;
    //! Indicates if load header files shall be created by Media Set Exporter
    FileCreationPolicy createLoadHeaderFilesV;

    //! Media Set
    Media::ConstMediaSetPtr mediaSetV;
    //! Create Medium Handler
    CreateMediumHandler createMediumHandlerV;
    //! Create Directory Handler
    CreateDirectoryHandler createDirectoryHandlerV;
    //! Check File existence Handler
    CheckFileExistenceHandler checkFileExistenceHandlerV;
    //! Create File Handler
    CreateFileHandler createFileHandlerV;
    //! Write File Handler
    WriteFileHandler writeFileHandlerV;
    //! Read File Handler
    ReadFileHandler readFileHandlerV;
};

}

#endif

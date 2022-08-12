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

#ifndef ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP
#define ARINC665_UTILS_IMPLEMENTATION_MEDIASETEXPORTERIMPL_HPP

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
    MediaSetExporterImpl() = default;

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

    //! @copydoc MediaSetExporter::arinc665Version()
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
     * Exports all directories, files, loads, and batches.
     * Create Index files.
     *
     * @param[in] medium
     *   Medium, which is exported.
     **/
    void exportMedium( const Media::ConstMediumPtr &medium );

    /**
     * @brief Called to export the given Directory.
     *
     * Exports all sub-directories, files, loads and batches.
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
     * @brief Exports List of Loads File.
     *
     * @param[in] medium
     *   Medium Information.
     **/
    void exportListOfLoads( const Media::ConstMediumPtr &medium ) const;

    /**
     * @brief Exports List of Batches File.
     *
     * @param[in] medium
     *   Medium Information.
     **/
    void exportListOfBatches( const Media::ConstMediumPtr &medium ) const;

    /**
     * @brief Exports List of Files File.
     *
     * @param[in] medium
     *   Medium Information.
     **/
    void exportListOfFiles( const Media::ConstMediumPtr &medium ) const;

    /**
     * @brief Creates the Load Header File.
     *
     * @param[in] loadFile
     *   Load header file.
     **/
    void createLoadHeaderFile( const Media::ConstFilePtr &loadFile ) const;

    /**
     * @brief Calculate Load file CRC, Check Value and Returns File Information.
     *
     * This operation is used to fill in the data and support file information
     * within the load header.
     *
     * @param[in] loadFile
     *   Load File.
     *
     * @return Processed Load File Information
     **/
    [[nodiscard]] Files::LoadFileInfo loadFileInformation(
      const Media::ConstLoadFile &loadFile ) const;

    /**
     * @brief Creates the Batch File.
     *
     * @param[in] file
     *   Batch file.
     **/
    void createBatchFile( const Media::ConstFilePtr &file ) const;

    /**
     * @brief Calculates CRC16 and Check Value of given file.
     *
     * @param[in] medium
     *   Medium Information
     * @param[in] checkValueType
     *   Desired Check Value Type.
     * @param[in] filename
     *   Filename.
     *
     * @return CRC16 and Check Value as std::tuple.
     **/
    [[nodiscard]] std::tuple< uint16_t, Arinc645::CheckValue > fileCrcCheckValue(
      const Media::ConstMediumPtr &medium,
      std::optional< Arinc645::CheckValueType > checkValueType,
      const std::filesystem::path &filename ) const;

    //! ARINC 665 Version used for exporting
    SupportedArinc665Version arinc665VersionV{
      SupportedArinc665Version::Supplement2 };
    //! Indicates if batch files shall be created by Media set Exporter
    FileCreationPolicy createBatchFilesV{ FileCreationPolicy::None };
    //! Indicates if load header files shall be created by Media Set Exporter
    FileCreationPolicy createLoadHeaderFilesV{ FileCreationPolicy::None };

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

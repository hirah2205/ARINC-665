/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetExporterImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_FILESYSTEMMEDIASETEXPORTERIMPL_HPP
#define ARINC665_UTILS_IMPLEMENTATION_FILESYSTEMMEDIASETEXPORTERIMPL_HPP

#include <arinc665/utils/FilesystemMediaSetExporter.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a Filesystem ARINC 665 Media Set Exporter.
 **/
class FilesystemMediaSetExporterImpl final : public FilesystemMediaSetExporter
{
  public:
    /**
     * @brief Initialises the ARINC 665 Media Set Exporter.
     **/
    FilesystemMediaSetExporterImpl();

    ~FilesystemMediaSetExporterImpl() override;

    //! @copydoc FilesystemMediaSetExporter::mediaSet()
    FilesystemMediaSetExporter& mediaSet(
      Media::ConstMediaSetPtr mediaSet ) override;

    //! @copydoc FilesystemMediaSetExporter::arinc665Version()
    FilesystemMediaSetExporter& arinc665Version(
      SupportedArinc665Version version ) override;

    //! @copydoc FilesystemMediaSetExporter::createBatchFiles()
    FilesystemMediaSetExporter& createBatchFiles(
      FileCreationPolicy createBatchFiles ) override;

    //! @copydoc FilesystemMediaSetExporter::createLoadHeaderFiles()
    FilesystemMediaSetExporter& createLoadHeaderFiles(
      FileCreationPolicy createLoadHeaderFiles ) override;

    //! @copydoc FilesystemMediaSetExporter::mediaSetBasePath()
    FilesystemMediaSetExporter& mediaSetBasePath(
      std::filesystem::path mediaSetBasePath ) override;

    //! @copydoc FilesystemMediaSetExporter::sourceBasePath()
    FilesystemMediaSetExporter& sourceBasePath(
      std::filesystem::path sourceBasePath ) override;

    //! @copydoc FilesystemMediaSetExporter::filePathMapping()
    FilesystemMediaSetExporter& filePathMapping(
      FilePathMapping filePathMapping ) override;

    /**
     * @brief Entry-point of the Filesystem ARINC 665 Media Set Exporter.
     ***/
    MediaPaths operator()() override;

  private:
    /**
     * @brief Returns the medium path.
     *
     * @param[in] mediumNumber
     *   Medium Number.
     *
     * @return Medium Path.
     **/
    [[nodiscard]] std::filesystem::path mediumPath(
      const Arinc665::MediumNumber &mediumNumber ) const;

    /**
     * @brief Creates the Directory for the given Medium.
     *
     * @param[in] mediumNumber
     *   Medium to Create.
     **/
    void createMedium( const Arinc665::MediumNumber &mediumNumber );

    /**
     * @brief Creates the given Directory.
     *
     * @param[in] mediumNumber
     *   Medium Number, where to create the directory in.
     * @param[in] directory
     *   Directory to Create.
     **/
    void createDirectory(
      const Arinc665::MediumNumber &mediumNumber,
      const Arinc665::Media::ConstDirectoryPtr &directory );

    /**
     * @brief Check File Existence Handler.
     *
     * @param[in] file
     *   File to Check
     *
     * @return If file exists
     **/
    [[nodiscard]] bool checkFileExistence(
      const Arinc665::Media::ConstFilePtr &file );

    /**
     * @brief Create File Handler.
     *
     * Copies the file from source to destination.
     *
     * @param[in] file
     *   File to Create
     **/
    void createFile( const Arinc665::Media::ConstFilePtr &file );

    /**
     * @brief Write File Handler
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   File Path on %Medium
     * @param[in] file
     *   File Content
     **/
    void writeFile(
      const Arinc665::MediumNumber &mediumNumber,
      const std::filesystem::path &path,
      const Arinc665::Files::ConstRawFileSpan &file );

    /**
     * @brief Read File Handler
     *
     * @param[in] mediumNumber
     *   Medium number.
     * @param[in] path
     *   File Path
     *
     * @return File Content
     **/
    [[nodiscard]] Arinc665::Files::RawFile readFile(
      const Arinc665::MediumNumber &mediumNumber,
      const std::filesystem::path &path );

    //! Media Set Exporter
    MediaSetExporterPtr mediaSetExporterV;
    //! Media Set Base Path
    std::filesystem::path mediaSetBasePathV;
    //! Source Base Path
    std::filesystem::path sourceBasePathV;
    //! File Path Mapping
    FilePathMapping filePathMappingV;
    //! Generated Media Paths
    MediaPaths mediaPathsV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::FilesystemMediaSetImporterImpl.
 **/

#ifndef ARINC665_UTILS_IMPLEMENTATION_FILESYSTEMMEDIASETIMPORTERIMPL_HPP
#define ARINC665_UTILS_IMPLEMENTATION_FILESYSTEMMEDIASETIMPORTERIMPL_HPP

#include <arinc665/utils/FilesystemMediaSetImporter.hpp>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a Filesystem ARINC 665 Media Set Importer.
 **/
class FilesystemMediaSetImporterImpl final : public FilesystemMediaSetImporter
{
  public:
    /**
     * @brief Initialises the ARINC 665 Media Set Importer
     **/
    explicit FilesystemMediaSetImporterImpl();

    ~FilesystemMediaSetImporterImpl() override;

    //! @copydoc FilesystemMediaSetImporter::checkFileIntegrity()
    FilesystemMediaSetImporter& checkFileIntegrity(
      bool checkFileIntegrity ) noexcept override;

    //! @copydoc FilesystemMediaSetImporter::mediaPaths()
    FilesystemMediaSetImporter& mediaPaths( MediaPaths mediaPaths ) override;

    /**
     * @brief Entry-point of the ARINC 665 Media Set Importer.
     *
     * @return Created Media Set Instance.
     **/
    [[nodiscard]] MediaSetImportResult operator()() override;

  private:
    /**
     * @brief Returns File Size of given File.
     *
     * @param[in] mediumNumber
     *   Medium number.
     * @param[in] path
     *   Path of file on Medium.
     *
     * @return File Size
     **/
    [[nodiscard]] size_t getFileSize(
      const Arinc665::MediumNumber &mediumNumber,
      const std::filesystem::path &path );

    /**
     * @brief Reads the give file and returns the data.
     *
     * @param[in] mediumNumber
     *   Medium number.
     * @param[in] path
     *   Path of file on Medium.
     *
     * @return Read file data.
     *
     * @throw Arinc665Exception
     *   If file does not exist or cannot be read.
     **/
    [[nodiscard]] Files::RawFile readFile(
      const Arinc665::MediumNumber &mediumNumber,
      const std::filesystem::path &path );

    //! Media Set Importer
    MediaSetImporterPtr mediaSetImporter;
    //! Media Paths
    MediaPaths mediaPathsV;
};

}

#endif

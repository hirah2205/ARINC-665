/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::Arinc665Utils.
 **/

#ifndef ARINC665_UTILS_ARINC665IMPORTER_HPP
#define ARINC665_UTILS_ARINC665IMPORTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/file/File.hpp>

#include <filesystem>
#include <functional>
#include <string>
#include <string_view>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Utilities.
 *
 * This class provides an interface ARINC 665 Utilities like:
 * * Media set importer, and
 * * Media set exporter
 **/
class Arinc665Utils
{
  public:
    //! Handler, which is called to generate the given medium.
    using CreateMediumHandler =
      std::function< void( Media::ConstMediumPtr medium)>;

    //! Handler, which is called to generate the given directory.
    using CreateDirectoryHandler =
      std::function< void( Media::ConstDirectoryPtr directory)>;

    //! Handler, which checks the existence of a file within the source
    using CheckFileExistenceHandler =
      std::function< bool( Media::ConstFilePtr file)>;

    //! Handler, which is called to generate the given file at the requested position.
    using CreateFileHandler = std::function< void( Media::ConstFilePtr file)>;

    /**
     * @brief Handler, which is called to read a file form a medium.
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Relative Path on Medium.
     *
     * @return File Data (Read as binary).
     **/
    using ReadFileHandler =
      std::function< File::RawFile( uint8_t mediumNumber, const std::filesystem::path &path)>;

    //! Handler, which is called to write the given file at the requested position.
    using WriteFileHandler =
      std::function< void( uint8_t mediumNumber, const std::filesystem::path &path, const File::RawFile &file)>;

    //! Handler which is called for Validation Information.
    using ValidatorInformationHandler =
      std::function< void( std::string_view information)>;

    /**
     * @brief The ARINC 665 Media Set Importer.
     * Returns the MediaSet
     **/
    using Arinc665Importer = std::function< Media::MediaSetPtr()>;

    /**
     * @brief ARINC 665 Media Set Validator
     * Validates the integrity and consistency of a ARINC 665 Media Set
     **/
    using Arinc665Validator = std::function< bool()>;

    /**
     * The ARINC 665 Media Set exporter.
     **/
    using Arinc665Exporter = std::function< void()>;

    /**
     * @brief Create a ARINC 665 Media Set importer.
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     *
     * @return The ARINC 665 Media Set importer.
     **/
    static Arinc665Importer arinc665Importer(
      ReadFileHandler readFileHandler);

    /**
     * @brief Creates a ARINC 665 Validator Instance.
     *
     * @param[in] readFileHandler
     *   Handler for reading files.
     * @param[in] informationHandler
     *   Handler for validation information.
     *
     * @return The ARINC 665 Validator.
     **/
    static Arinc665Validator arinc665Validator(
      ReadFileHandler readFileHandler,
      ValidatorInformationHandler informationHandler);

    /**
     * @brief Creates a ARINC 665 Media Set exporter.
     *
     * @param[in] mediaSet
     *   The media set, which shall be exported.
     * @param[in] createMediumHandler
     *   Called to create the given medium.
     * @param[in] createDirectoryHandler
     *   Called to create the given directory.
     * @param[in] checkFileExistenceHandler
     *   Called to check if file exists.
     * @param[in] createFileHandler
     *   When a file needs to be generated, this handler is called.
     * @param[in] writeFileHandler
     *   Writes a given file to the output media set.
     * @param[in] readFileHandler
     *   Reads a given file from the output media set.
     *   Used for CRC calculation.
     * @param[in] arinc665Version
     *   The ARINC 665 version used for exporting.
     * @param[in] createBatchFiles
     *   If set to true, Batch Files are created by exporter.
     * @param[in] createLoadHeaderFiles
     *   If set to true, Load Header Files are created by exporter.
     *
     * @return The ARINC 665 Media Set exporter.
     **/
    static Arinc665Exporter arinc665Exporter(
      Media::ConstMediaSetPtr mediaSet,
      CreateMediumHandler createMediumHandler,
      CreateDirectoryHandler createDirectoryHandler,
      CheckFileExistenceHandler checkFileExistenceHandler,
      CreateFileHandler createFileHandler,
      WriteFileHandler writeFileHandler,
      ReadFileHandler readFileHandler,
      SupportedArinc665Version arinc665Version = SupportedArinc665Version::Supplement2,
      FileCreationPolicy createBatchFiles = FileCreationPolicy::None,
      FileCreationPolicy createLoadHeaderFiles = FileCreationPolicy::None );
};

}

#endif

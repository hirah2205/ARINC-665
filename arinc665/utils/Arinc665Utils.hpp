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
 * - ARINC 665 Media Set importer, and
 * - ARINC 665 Media Set exporter
 **/
class Arinc665Utils
{
  public:

    /**
     * @brief Handler, which is called to read a file form a medium.
     *
     * This handler is also used to read files, which are not represented by
     * Arinc665::Media classes.
     * Therefore a basic representation is used.
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Relative Path on Medium.
     *
     * @return File Data (Read as binary).
     **/
    using ReadFileHandler =
      std::function< File::RawFile(
        uint8_t mediumNumber,
        const std::filesystem::path &path ) >;


    //! Handler which is called for Validation Information.
    using ValidatorInformationHandler =
      std::function< void( std::string_view information ) >;

    /**
     * @brief The ARINC 665 Media Set Importer.
     * Returns the MediaSet
     **/
    using Arinc665Importer = std::function< Media::MediaSetPtr() >;

    /**
     * @brief ARINC 665 Media Set Validator
     * Validates the integrity and consistency of a ARINC 665 Media Set
     **/
    using Arinc665Validator = std::function< bool() >;

    /**
     * @brief Create a ARINC 665 Media Set Importer.
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     *
     * @return The ARINC 665 Media Set importer.
     **/
    static Arinc665Importer arinc665Importer(
      ReadFileHandler readFileHandler,
      bool checkFileIntegrity = true );

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
      ValidatorInformationHandler informationHandler );
};

}

#endif

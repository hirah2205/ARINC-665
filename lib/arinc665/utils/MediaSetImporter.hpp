/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetImporter.
 **/

#ifndef ARINC665_ARINC665_UTILS_MEDIASETIMPORTER_HPP
#define ARINC665_ARINC665_UTILS_MEDIASETIMPORTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc645/Arinc645.hpp>

#include <filesystem>
#include <functional>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Importer
 *
 * Imports the Media Set given by the provided properties.
 **/
class ARINC665_EXPORT MediaSetImporter
{
  public:
    //! Return Type of Operation
    using Result = std::pair< Media::MediaSetPtr, CheckValues >;

    /**
     * @brief Handler, which is called to obtain the file size.
     *
     * This handler is also used to access files, which are not represented by
     * Arinc665::Media classes (i.e. List of Files, Loads, and Batches).
     * Therefore, a basic representation is used.
     *
     * This Handler shall throw, when the file does not exist.
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Relative Path on Medium.
     *
     * @return File size in Bytes.
     **/
    using FileSizeHandler = std::function< size_t(
      const MediumNumber &mediumNumber,
      const std::filesystem::path &path ) >;

    /**
     * @brief Handler, which is called to read a file from a medium.
     *
     * This handler is also used to read files, which are not represented by
     * Arinc665::Media classes.
     * Therefore, a basic representation is used.
     *
     * This Handler shall throw, when the file does not exist.
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Relative Path on Medium.
     *
     * @return File Data (Read as binary).
     **/
    using ReadFileHandler = std::function< Files::RawFile(
      const MediumNumber &mediumNumber,
      const std::filesystem::path &path ) >;

    /**
     * @brief Creates the ARINC 665 Media Set Importer Instance.
     *
     * @return ARINC 665 Importer Instance
     **/
    static MediaSetImporterPtr create();

    //! Destructor
    virtual ~MediaSetImporter() = default;

    /**
     * @brief Sets the File Size Handler.
     *
     * @param[in] fileSizeHandler
     *   Handler which is called to obtain the requested file size the medium.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetImporter& fileSizeHandler(
      FileSizeHandler fileSizeHandler ) = 0;

    /**
     * @brief Sets the Read File Handler.
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetImporter& readFileHandler(
      ReadFileHandler readFileHandler ) = 0;

    /**
     * @brief Sets the Check File Integrity Flag.
     *
     * @param[in] checkFileIntegrity
     *   If set to true additional file integrity steps are performed
     *
     * @return *this for chaining.
     **/
    virtual MediaSetImporter& checkFileIntegrity( bool checkFileIntegrity ) = 0;

    /**
     * @brief Executes the ARINC 665 Media Set Importer.
     *
     * All parameters must have been set previously.
     *
     * @return Imported Media Set
     *
     * @throw Arinc665Exception
     **/
    virtual Result operator()() = 0;
};

}

#endif

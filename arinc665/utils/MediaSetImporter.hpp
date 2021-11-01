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

#include <arinc665/file/File.hpp>

#include <filesystem>
#include <functional>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Set Importer
 *
 * Imports the Media Set given by the provided properties..
 **/
class MediaSetImporter
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

  /**
   * @brief Creates the ARINC 665 Media Set Exporter Instance.
   *
   * @return ARINC 665 Exporter Instance
   **/
  static MediaSetImporterPtr create();

  //! Destructor
  virtual ~MediaSetImporter() = default;

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
  virtual MediaSetImporter& checkFileIntegrity(
    bool checkFileIntegrity ) = 0;

  /**
   * @brief Executes the ARINC 665 Media Set Importer.
   *
   * All parameters must have been set previously.
   *
   * @return The imported Media Set
   *
   * @throw Arinc665Exception
   **/
  virtual Media::MediaSetPtr operator()() = 0;
};

}

#endif

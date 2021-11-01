/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetExporter.
 **/

#ifndef ARINC665_ARINC665_UTILS_MEDIASETEXPORTER_HPP
#define ARINC665_ARINC665_UTILS_MEDIASETEXPORTER_HPP

#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/file/File.hpp>

#include <filesystem>
#include <functional>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 Media Set Exporter
 *
 * This exporter uses a Media Set and exports all files and structures.
 **/
class MediaSetExporter
{
public:
  //! Handler, which is called to generate the given medium.
  using CreateMediumHandler =
    std::function< void( Media::ConstMediumPtr medium )>;

  //! Handler, which is called to generate the given directory.
  using CreateDirectoryHandler =
    std::function< void( Media::ConstDirectoryPtr directory )>;

  //! Handler, which checks the existence of a file within the source
  using CheckFileExistenceHandler =
    std::function< bool( Media::ConstFilePtr file )>;

  //! Handler, which is called to generate the given file at the requested position.
  using CreateFileHandler = std::function< void( Media::ConstFilePtr file )>;

  //! Destructor
  virtual ~MediaSetExporter() = default;

  /**
   * @brief Handler, which is called to write the given file at the requested
   *   position.
   *
   * This handler is also used to write files, which are not represented by
   * Arinc665::Media classes.
   * Therefore a basic representation is used.
   *
   * @param[in] mediumNumber
   *   Medium Number
   * @param[in] path
   *   Relative Path on Medium.
   * @param[in] file
   *   File Data (binary).
   **/
  using WriteFileHandler =
    std::function< void(
      uint8_t mediumNumber,
      const std::filesystem::path &path,
      const File::ConstRawFileSpan &file )>;

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
  static MediaSetExporterPtr create();

  /**
   * @brief Sets the Media Set to export.
   *
   * @param[in] mediaSet
   *   Media Set, which shall be exported.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& mediaSet( Media::ConstMediaSetPtr mediaSet) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] createMediumHandler
   *   Called to create the given medium.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& createMediumHandler(
    CreateMediumHandler createMediumHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] createDirectoryHandler
   *   Called to create the given directory.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& createDirectoryHandler(
    CreateDirectoryHandler createDirectoryHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] checkFileExistenceHandler
   *   Called to check if file exists.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& checkFileExistenceHandler(
    CheckFileExistenceHandler checkFileExistenceHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] createFileHandler
   *   When a file needs to be generated, this handler is called.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& createFileHandler(
    CreateFileHandler createFileHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] writeFileHandler
   *   Writes a given file to the output media set.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& writeFileHandler(
    WriteFileHandler writeFileHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] readFileHandler
   *   Reads a given file from the output media set.
   *   Used for CRC calculation.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& readFileHandler(
    ReadFileHandler readFileHandler ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] arinc665Version
   *   ARINC 665 version used for exporting.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& arinc665Version(
    SupportedArinc665Version arinc665Version ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] createBatchFiles
   *   Defines, if Batch Files are created by exporter or pre-existing ones
   *   are used.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& createBatchFiles(
    FileCreationPolicy createBatchFiles ) = 0;

  /**
   * @brief Sets the .
   *
   * @param[in] createLoadHeaderFiles
   *   Defines, if Load Header Files are created by exporter or pre-existing
   *   ones are used.
   *
   * @return *this for chaining.
   **/
  virtual MediaSetExporter& createLoadHeaderFiles(
    FileCreationPolicy createLoadHeaderFiles ) = 0;

  /**
   * @brief Executes the ARINC 665 Media Set Exporter.
   *
   * All parameters must have been set previously.
   *
   * @throw Arinc665Exception
   **/
  virtual void operator()() = 0;
};

}

#endif

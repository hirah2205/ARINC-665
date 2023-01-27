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

#include <arinc665/files/Files.hpp>

#include <filesystem>
#include <functional>

namespace Arinc665::Utils {

/**
 * @brief ARINC 665 %Media Set Exporter
 *
 * This exporter uses a Media Set and exports all files and structures.
 **/
class ARINC665_EXPORT MediaSetExporter
{
  public:
    /**
     * @brief Handler, which is called to generate the given Medium.
     *
     * @param[in] medium
     *   Medium to Create.
     **/
    using CreateMediumHandler =
      std::function< void( const Media::ConstMediumPtr &medium ) >;

    /**
     * @brief Handler, which is called to generate the given Directory.
     *
     * @param[in] directory
     *   Directory to create.
     **/
    using CreateDirectoryHandler =
      std::function< void( const Media::ConstDirectoryPtr &directory ) >;

    /**
     * @brief Handler, which checks the existence of a file within the Source.
     *
     * This callback is used, to check if a file exist on sources.
     * This is used for Load Header and Batch Files.
     *
     * @param[in] file
     *   File to check existence for.
     *
     * @return if the File exist on sources.
     **/
    using CheckFileExistenceHandler =
      std::function< bool( const Media::ConstFilePtr &file ) >;

    /**
     * @brief Handler, which is called to generate the given File from Source.
     *
     * How the file is generated is not known to the exporter itself.
     *
     * @param[in] file
     *   File to be created.
     **/
    using CreateFileHandler =
      std::function< void( const Media::ConstFilePtr &file ) >;

    /**
     * @brief Handler, which is called to write the given File on the Target.
     *
     * This handler is also used to write files, which are not represented by
     * Arinc665::Media classes.
     * Therefore, a basic representation is used.
     *
     * @param[in] mediumNumber
     *   Medium number
     * @param[in] path
     *   Relative path on Medium.
     * @param[in] file
     *   File data (binary).
     **/
    using WriteFileHandler =
      std::function< void(
        uint8_t mediumNumber,
        const std::filesystem::path &path,
        const Files::ConstRawFileSpan &file ) >;

    /**
     * @brief Handler, which is called to read a File from the Target.
     *
     * This handler is also used to read files, which are not represented by
     * Arinc665::Media classes.
     * Therefore, a basic representation is used.
     * This operation is used for checksum and check value calculation.
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Relative Path on Medium.
     *
     * @return File Data (Read as binary).
     **/
    using ReadFileHandler =
      std::function< Files::RawFile(
        uint8_t mediumNumber,
        const std::filesystem::path &path ) >;

    /**
     * @brief Creates the ARINC 665 Media Set Exporter Instance.
     *
     * @return ARINC 665 Exporter Instance
     **/
    static MediaSetExporterPtr create();

    //! Destructor
    virtual ~MediaSetExporter() = default;

    /**
     * @brief Sets the Media Set to export.
     *
     * @param[in] mediaSet
     *   Media Set, which shall be exported.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& mediaSet( Media::ConstMediaSetPtr mediaSet ) = 0;

    /**
     * @brief Sets the Create Medium Handler.
     *
     * @param[in] createMediumHandler
     *   Called to create the given medium.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& createMediumHandler(
      CreateMediumHandler createMediumHandler ) = 0;

    /**
     * @brief Sets the Create Directory Handler.
     *
     * @param[in] createDirectoryHandler
     *   Called to create the given directory.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& createDirectoryHandler(
      CreateDirectoryHandler createDirectoryHandler ) = 0;

    /**
     * @brief Sets the Check File Existence Handler.
     *
     * @param[in] checkFileExistenceHandler
     *   Called to check if file exists.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& checkFileExistenceHandler(
      CheckFileExistenceHandler checkFileExistenceHandler ) = 0;

    /**
     * @brief Sets the Create File Handler.
     *
     * @param[in] createFileHandler
     *   When a file needs to be generated, this handler is called.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& createFileHandler(
      CreateFileHandler createFileHandler ) = 0;

    /**
     * @brief Sets the Write File Handler.
     *
     * @param[in] writeFileHandler
     *   Writes a given file to the output media set.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& writeFileHandler(
      WriteFileHandler writeFileHandler ) = 0;

    /**
     * @brief Sets the Read File Handler.
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
     * @brief Sets the ARINC 665 Version Flag.
     *
     * @param[in] version
     *   ARINC 665 version used for exporting.
     *
     * @return *this for chaining.
     **/
    virtual MediaSetExporter& arinc665Version(
      SupportedArinc665Version version ) = 0;

    /**
     * @brief Sets the Create Batch Files Flag.
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
     * @brief Sets the Create Load Header Files Flag.
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

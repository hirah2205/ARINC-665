/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665CompilerApplication.
 **/

#ifndef ARINC665COMPILERAPPLICATION_HPP
#define ARINC665COMPILERAPPLICATION_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/file/File.hpp>
#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

class Arinc665CompilerApplication
{
  public:
    /**
     * @brief Constructs the TFTP client application.
     *
     * @param[in] context
     *   The application context.
     **/
    Arinc665CompilerApplication();

    /**
     * @brief Executes the application.
     *
     * @return The return code, which should be returned to the system.
     * @retval EXIT_SUCCESS
     *   If operation has been performed successfully.
     * @retval EXIT_FAILURE
     *   If an error has occurred.
     **/
    int operator()( int argc, char *argv[]);

  private:
    /**
     * @brief Returns the medium path.
     *
     * @param[in] mediumNumber
     *   The medium number.
     *
     * @return The medium path.
     **/
    std::filesystem::path mediumPath( uint8_t mediumNumber) const;

    /**
     * @brief Creates the directory for the given medium.
     *
     * @param[in] medium
     *   The medium.
     **/
    void createMedium( Arinc665::Media::ConstMediumPtr medium);

    /**
     * @brief Creates the directory for the given directory.
     *
     * @param[in] directory
     *   The directory.
     **/
    void createDirectory( Arinc665::Media::ConstDirectoryPtr directory);

    bool checkFileExistance(
      const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
      Arinc665::Media::ConstFilePtr file);

    void createFile(
      const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
      Arinc665::Media::ConstFilePtr file);

    void writeFile(
      uint8_t mediumNumber,
      const std::filesystem::path &path,
      const Arinc665::File::RawFile &file);

    Arinc665::File::RawFile readFile(
      uint8_t mediumNumber,
      const std::filesystem::path &path);

    //! Program Options description
    boost::program_options::options_description optionsDescription;

    //! Media Set XML file
    std::filesystem::path mediaSetXmlFile;
    //! Media Set source directory
    std::filesystem::path mediaSetSourceDirectory;
    //! Media Set destination directory
    std::filesystem::path mediaSetDestinationDirectory;
    //! Create batch file policy
    Arinc665::Utils::FileCreationPolicy createBatchFiles;
    //! Create Load Header file policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles;
    //! The ARINC 665 XML instance.
   Arinc665::Utils::Arinc665XmlPtr xml;
};

#endif

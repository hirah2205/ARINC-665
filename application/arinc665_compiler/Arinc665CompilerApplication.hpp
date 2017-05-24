/*
 * $Date$
 * $Revision$
 */
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
#include <boost/application.hpp>
#include <boost/filesystem/path.hpp>

class Arinc665CompilerApplication
{
  public:
    /**
     * @brief Constructs the TFTP client application.
     *
     * @param[in] context
     *   The application context.
     **/
    Arinc665CompilerApplication( boost::application::context &context);

    /**
     * @brief Executes the FIND client.
     *
     * @return The return code, which should be returned to the system.
     * @retval EXIT_SUCCESS
     *   If operation has been performed successfully.
     * @retval EXIT_FAILURE
     *   If an error has occurred.
     **/
    int operator()();

  private:
    using path = boost::filesystem::path;

    /**
     * @brief Parsed the command line and assigns parameter.
     *
     * @return If parsing was successful
     **/
    bool handleCommandLine();

    /**
     * @brief Returns the medium path.
     *
     * @param[in] mediumNumber
     *   The medium number.
     *
     * @return The medium path.
     **/
    path getMediumPath( uint8_t mediumNumber) const;

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

    void createFile(
      const Arinc665::Utils::Arinc665Xml::LoadXmlResult &mediaSetInfo,
      Arinc665::Media::ConstFilePtr file);

    void writeFile(
      uint8_t mediumNumber,
      const path &path,
      const Arinc665::File::RawFile &file);

    Arinc665::File::RawFile readFile(
      uint8_t mediumNumber,
      const path &path);

    //! The application context
    boost::application::context& context;
    //! Program Options description
    boost::program_options::options_description optionsDescription;

    //! Media Set XML file
    path mediaSetXmlFile;
    //! Media Set source directory
    path mediaSetSourceDirectory;
    //! Media Set destination directory
    path mediaSetDestinationDirectory;
    //!
    bool createBatchFiles;
    bool createLoadHeaderFiles;
    Arinc665::Utils::Arinc665XmlPtr xml;
};

#endif

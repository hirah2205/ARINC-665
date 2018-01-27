/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665DecompilerApplication.
 **/

#ifndef ARINC665DECOMPILERAPPLICATION_HPP
#define ARINC665DECOMPILERAPPLICATION_HPP

#include <arinc665/file/File.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>

#include <vector>

//! ARINC 665 decompiler application
class Arinc665DecompilerApplication
{
  public:
    /**
     * @brief Initialises the application.
     **/
    Arinc665DecompilerApplication();

    /**
     * @brief Executes the FIND client.
     *
     * @return The return code, which should be returned to the system.
     * @retval EXIT_SUCCESS
     *   If operation has been performed successfully.
     * @retval EXIT_FAILURE
     *   If an error has occurred.
     **/
    int operator()( int argc, char *argv[]);

  private:
    using path = boost::filesystem::path;

    /**
     * @brief Parsed the command line and assigns parameter.
     *
     * @return If parsing was successful
     **/
    bool handleCommandLine();

    Arinc665::File::RawFile readFile( uint8_t mediumNumber, const path &path);

    //! Program Options description
    boost::program_options::options_description optionsDescription;

    //! Media Set media source directories
    std::vector< path> mediaSourceDirectories;
    //! Media Set destination directory
    path mediaSetXmlFile;
};

#endif

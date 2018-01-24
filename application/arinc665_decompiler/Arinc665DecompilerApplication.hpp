/*
 * $Date: 2017-05-21 19:56:17 +0200 (So, 21. Mai 2017) $
 * $Revision: 2038 $
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
 * @brief Declaration of class Arinc665DecompilerApplication.
 **/

#ifndef ARINC665DECOMPILERAPPLICATION_HPP
#define ARINC665DECOMPILERAPPLICATION_HPP

#include <arinc665/file/File.hpp>

#include <boost/program_options.hpp>
#include <boost/application.hpp>
#include <boost/filesystem/path.hpp>

#include <vector>

class Arinc665DecompilerApplication
{
  public:
    /**
     * @brief Constructs the TFTP client application.
     *
     * @param[in] context
     *   The application context.
     **/
    Arinc665DecompilerApplication( boost::application::context &context);

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

    Arinc665::File::RawFile readFile( uint8_t mediumNumber, const path &path);

    //! The application context
    boost::application::context& context;
    //! Program Options description
    boost::program_options::options_description optionsDescription;

    //! Media Set media source directories
    std::vector< path> mediaSourceDirectories;
    //! Media Set destination directory
    path mediaSetDestinationDirectory;
};

#endif

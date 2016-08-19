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
 * @brief Declaration of class Arinc665DecompilerApplication.
 **/

#ifndef ARINC665DECOMPILERAPPLICATION_HPP
#define ARINC665DECOMPILERAPPLICATION_HPP

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
    int operator()( void);

  private:
    /**
     * @brief Parsed the command line and assigns parameter.
     *
     * @return If parsing was successful
     **/
    bool handleCommandLine( void);

    //! The application context
    boost::application::context& context;
    //! Program Options description
    boost::program_options::options_description optionsDescription;

    //! Media Set XML file
    boost::filesystem::path mediaSetXmlFile;
    //! Media Set source directories
    std::vector< boost::filesystem::path> mediaSetSourceDirectories;
    //! Media Set destination directory
    boost::filesystem::path mediaSetDestinationDirectory;
};

#endif

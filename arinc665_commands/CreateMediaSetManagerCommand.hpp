/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::CreateMediaSetManagerCommand.
 **/

#ifndef ARINC665_COMMANDS_CREATEMEDIASETMANAGERCOMMAND_HPP
#define ARINC665_COMMANDS_CREATEMEDIASETMANAGERCOMMAND_HPP

#include <arinc665_commands/Arinc665Commands.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

namespace Arinc665Commands {

/**
 * @brief Create Media Set Manager %Command
 *
 * Creates a Media Set Manager Configuration with a specified directory.
 **/
class ARINC665_COMMANDS_EXPORT CreateMediaSetManagerCommand
{
  public:
    /**
     * @brief Constructs the Command.
     **/
    CreateMediaSetManagerCommand();

    /**
     * @brief Executes the Operation.
     *
     * @param[in] parameters
     *   Parameters supplied by User.
     **/
    void execute( const Commands::Parameters &parameters );

    //! Prints help screen.
    void help();

  private:
    //! Program Options Description
    boost::program_options::options_description optionsDescription;
    //! Media Set Manager Directory
    std::filesystem::path mediaSetManagerDirectory;
};

}

#endif

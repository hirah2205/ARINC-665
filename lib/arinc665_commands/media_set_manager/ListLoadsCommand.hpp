/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class
 *   Arinc665Commands::MediaSetManager::ListLoadsCommand.
 **/

#ifndef ARINC665_COMMANDS_MEDIA_SET_MANAGER_LISTLOADSCOMMAND_HPP
#define ARINC665_COMMANDS_MEDIA_SET_MANAGER_LISTLOADSCOMMAND_HPP

#include <arinc665_commands/media_set_manager/MediaSetManager.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>

namespace Arinc665Commands::MediaSetManager {

/**
 * @brief List Loads within Media Set Manager %Command.
 *
 * List all loads, which are part of the media sets of the Media Set Manager.
 **/
class ARINC665_COMMANDS_EXPORT ListLoadsCommand
{
  public:
    /**
     * @brief Constructs the List Loads Command.
     **/
    ListLoadsCommand();

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
    //! Check File Integrity Flag
    bool checkFileIntegrity;
};

}

#endif

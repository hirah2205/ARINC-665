/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::ImportCommand.
 **/

#ifndef ARINC665_COMMANDS_IMPORTCOMMAND_HPP
#define ARINC665_COMMANDS_IMPORTCOMMAND_HPP

#include <arinc665_commands/Arinc665Commands.hpp>

#include <arinc665/files/Files.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
#include <vector>

namespace Arinc665Commands {

/**
 * @brief Import Command
 *
 * Adds the given media set to the media set manager.
 *
 * Copies the media from the source path to its destination paths.
 * The integrity of the media paths are not checked.
 * Missing files will be detected on trying to access them.
 * To much files are not detected.
 **/
class ImportCommand
{
  public:
    /**
     * @brief Constructs the Import Command.
     **/
    ImportCommand();

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
    Arinc665::Files::RawFile readFileHandler(
      uint8_t mediumNumber,
      const std::filesystem::path &path );

    //! Program Options Description
    boost::program_options::options_description optionsDescription;
    //! Media Set Manager Directory
    std::filesystem::path mediaSetManagerDirectory;
    //! Media source directories
    std::vector< std::filesystem::path > mediaSourceDirectories;
    //! Check File Integrity
    bool checkFileIntegrity;
};

}

#endif

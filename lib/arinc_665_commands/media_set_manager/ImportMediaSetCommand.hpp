// SPDX-License-Identifier: MPL-2.0
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
 *   Arinc665Commands::MediaSetManager::ImportMediaSetCommand.
 **/

#ifndef ARINC_665_COMMANDS_MEDIA_SET_MANAGER_IMPORTMEDIASETCOMMAND_HPP
#define ARINC_665_COMMANDS_MEDIA_SET_MANAGER_IMPORTMEDIASETCOMMAND_HPP

#include <arinc_665_commands/media_set_manager/MediaSetManager.hpp>

#include <arinc_665/files/Files.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>
#include <boost/optional/optional.hpp>

#include <filesystem>
#include <vector>

namespace Arinc665Commands::MediaSetManager {

/**
 * @brief Import Media Set to Media Set Manager %Command.
 *
 * Adds the given media set to the media set manager.
 *
 * Copies the media from the source path to its destination paths.
 * The integrity of the media paths are not checked.
 * Missing files will be detected on trying to access them.
 * Too many files are not detected.
 **/
class ARINC_665_COMMANDS_EXPORT ImportMediaSetCommand
{
  public:
    /**
     * @brief Constructs the Import Command.
     **/
    ImportMediaSetCommand();

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
    /**
     * @brief Load progress indicator.
     *
     * @param[in] mediaSet
     *   Media Set information
     * @param[in] partNumber
     *   Media Set Part Number
     * @param[in] medium
     *   Medium information
     **/
    void loadProgress(
      std::pair< std::size_t, std::size_t > mediaSet,
      std::string_view partNumber,
      std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium );

    //! Program Options Description
    boost::program_options::options_description optionsDescription;
    //! Media Set Manager Directory
    std::filesystem::path mediaSetManagerDirectory{};
    //! Check Media Set Manager Integrity
    bool checkMediaSetManagerIntegrityV{ true };
    //! Media Source Directories
    std::vector< std::filesystem::path > mediaSourceDirectories{};
    //! Check File Integrity
    boost::optional< bool > checkFileIntegrity{};
};

}

#endif

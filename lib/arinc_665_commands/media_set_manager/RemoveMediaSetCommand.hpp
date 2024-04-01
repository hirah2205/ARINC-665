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
 *   Arinc665Commands::MediaSetManager::RemoveMediaSetCommand.
 **/

#ifndef ARINC_665_COMMANDS_MEDIA_SET_MANAGER_REMOVEMEDIASETCOMMAND_HPP
#define ARINC_665_COMMANDS_MEDIA_SET_MANAGER_REMOVEMEDIASETCOMMAND_HPP

#include <arinc_665_commands/media_set_manager/MediaSetManager.hpp>

#include <arinc_665/Arinc665.hpp>

#include <commands/Commands.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
#include <vector>

namespace Arinc665Commands::MediaSetManager {

/**
 * @brief Remove Media Set from Media Set Manager %Command.
 *
 * Removes a registered Media Set from the Media Set Manager and deletes data.
 **/
class ARINC_665_COMMANDS_EXPORT RemoveMediaSetCommand
{
  public:
    /**
     * @brief Constructs the Remove Media Set Command.
     **/
    RemoveMediaSetCommand();

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
    //! Media Set Part Number
    std::string mediaSetPartNumber{};
};

}

#endif

/**
* @file
* @copyright
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* @author Thomas Vogt, thomas@thomas-vogt.de
*
* @brief ARINC 665 Media Set Manager.
**/

/**
* @dir
* @brief ARINC 665 Media Set Manager Application.
**/

#include <arinc665_commands/CreateMediaSetManagerCommand.hpp>
#include <arinc665_commands/ListLoadsCommand.hpp>
#include <arinc665_commands/ListMediaSetsCommand.hpp>
#include <arinc665_commands/ImportMediaSetXmlCommand.hpp>
#include <arinc665_commands/ImportMediaSetCommand.hpp>
#include <arinc665_commands/RemoveMediaSetCommand.hpp>

#include <commands/CommandRegistry.hpp>
#include <commands/CommandUtils.hpp>

#include <helper/Logger.hpp>

#include <iostream>
#include <cstdlib>
#include <functional>

/**
 * @brief Program Entry Point
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Success state of this operation.
 **/
int main( int argc, char * argv[] );

int main( int argc, char * argv[] )
{
  Helper::initLogging( Helper::Severity::warning, false );

  auto registry{ Commands::CommandRegistry::instance() };

  Arinc665Commands::CreateMediaSetManagerCommand createMediaSetManagerCommand{};

  registry->command(
    "Create",
    "Create Media Set Manager",
    std::bind(
      &Arinc665Commands::CreateMediaSetManagerCommand::execute,
      &createMediaSetManagerCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::CreateMediaSetManagerCommand::help,
      &createMediaSetManagerCommand ) );

  Arinc665Commands::ListLoadsCommand listLoadsCommand{};

  registry->command(
    "ListLoads",
    "List Loads",
    std::bind(
      &Arinc665Commands::ListLoadsCommand::execute,
      &listLoadsCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ListLoadsCommand::help,
      &listLoadsCommand ) );

  Arinc665Commands::ListMediaSetsCommand listCommand{};

  registry->command(
    "ListMediaSets",
    "List Media Sets",
    std::bind(
      &Arinc665Commands::ListMediaSetsCommand::execute,
      &listCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ListMediaSetsCommand::help,
      &listCommand ) );

  Arinc665Commands::ImportMediaSetXmlCommand importXmlCommand{};

  registry->command(
    "ImportXml",
    "Import XML Media Set",
    std::bind(
      &Arinc665Commands::ImportMediaSetXmlCommand::execute,
      &importXmlCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportMediaSetXmlCommand::help,
      &importXmlCommand ) );

  Arinc665Commands::ImportMediaSetCommand importCommand{};
  registry->command(
    "Import",
    "Import Media Set",
    std::bind(
      &Arinc665Commands::ImportMediaSetCommand::execute,
      &importCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportMediaSetCommand::help,
      &importCommand ) );

  Arinc665Commands::RemoveMediaSetCommand removeMediaSetCommand{};
  registry->command(
    "Remove",
    "Remove Media Set",
    std::bind(
      &Arinc665Commands::RemoveMediaSetCommand::execute,
      &removeMediaSetCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::RemoveMediaSetCommand::help,
      &removeMediaSetCommand ) );

  try
  {
    const auto result{
      Commands::CommandUtils::commandLineHandler( registry )( argc, argv ) };

    return result;
  }
  catch ( boost::exception &e )
  {
    std::cerr << "Error: " << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e )
  {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "unknown error\n";
    return EXIT_FAILURE;
  }
}

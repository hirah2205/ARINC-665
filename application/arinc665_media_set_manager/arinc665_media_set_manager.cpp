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
#include <arinc665_commands/ListCommand.hpp>
#include <arinc665_commands/ImportXmlCommand.hpp>
#include <arinc665_commands/ImportCommand.hpp>

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
    "Create Media Set manager",
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

  Arinc665Commands::ListCommand listCommand{};

  registry->command(
    "List",
    "List Media Set Manager",
    std::bind(
      &Arinc665Commands::ListCommand::execute,
      &listCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ListCommand::help,
      &listCommand ) );

  Arinc665Commands::ImportXmlCommand importXmlCommand{};

  registry->command(
    "ImportXml",
    "Import XML Media Set",
    std::bind(
      &Arinc665Commands::ImportXmlCommand::execute,
      &importXmlCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportXmlCommand::help,
      &importXmlCommand ) );

  Arinc665Commands::ImportCommand importCommand{};
  registry->command(
    "Import",
    "Import Media Set",
    std::bind(
      &Arinc665Commands::ImportCommand::execute,
      &importCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportCommand::help,
      &importCommand ) );

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
  catch (...)
  {
    std::cerr << "unknown error\n";
    return EXIT_FAILURE;
  }
}

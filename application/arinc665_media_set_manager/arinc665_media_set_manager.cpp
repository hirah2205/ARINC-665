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

#include "CreateMediaSetManagerCommand.hpp"
#include "ListLoadsCommand.hpp"
#include "ListCommand.hpp"
#include "ImportXmlCommand.hpp"

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

  CreateMediaSetManagerCommand createMediaSetManagerCommand{};

  registry->command(
    "Create",
    "Create Media Set manager",
    std::bind(
      &CreateMediaSetManagerCommand::execute,
      &createMediaSetManagerCommand,
      std::placeholders::_1 ),
    std::bind(
      &CreateMediaSetManagerCommand::help,
      &createMediaSetManagerCommand ) );

  ListLoadsCommand listLoadsCommand{};

  registry->command(
    "ListLoads",
    "List Loads",
    std::bind(
      &ListLoadsCommand::execute,
      &listLoadsCommand,
      std::placeholders::_1 ),
    std::bind(
      &ListLoadsCommand::help,
      &listLoadsCommand ) );

  ListCommand listCommand{};

  registry->command(
    "List",
    "List Media Set Manager",
    std::bind(
      &ListCommand::execute,
      &listCommand,
      std::placeholders::_1 ),
    std::bind(
      &ListCommand::help,
      &listCommand ) );

  ImportXmlCommand importXmlCommand{};

  registry->command(
    "ImportXml",
    "Import XML Media Set",
    std::bind(
      &ImportXmlCommand::execute,
      &importXmlCommand,
      std::placeholders::_1 ),
    std::bind(
      &ImportXmlCommand::help,
      &importXmlCommand ) );

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

/**
* @file
* @copyright
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* @author Thomas Vogt, thomas@thomas-vogt.de
*
* @brief Definition of Namespace Arinc665Commands.
**/

#include "Arinc665Commands.hpp"

#include <arinc665_commands/CreateMediaSetManagerCommand.hpp>
#include <arinc665_commands/ListLoadsCommand.hpp>
#include <arinc665_commands/ListMediaSetsCommand.hpp>
#include <arinc665_commands/ImportMediaSetXmlCommand.hpp>
#include <arinc665_commands/ImportMediaSetCommand.hpp>
#include <arinc665_commands/RemoveMediaSetCommand.hpp>

#include <commands/CommandRegistry.hpp>

#include <functional>
#include <memory>

namespace Arinc665Commands {

void registerCommands( const Commands::CommandRegistryPtr& registry )
{
  auto createMediaSetManagerCommand{
    std::make_shared< CreateMediaSetManagerCommand >() };
  registry->command(
    "Create",
    "Create Media Set Manager",
    std::bind(
      &Arinc665Commands::CreateMediaSetManagerCommand::execute,
      createMediaSetManagerCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::CreateMediaSetManagerCommand::help,
      createMediaSetManagerCommand ) );

  auto listLoadsCommand{ std::make_shared< ListLoadsCommand >() };
  registry->command(
    "ListLoads",
    "List Loads of contained on all Media Sets",
    std::bind(
      &Arinc665Commands::ListLoadsCommand::execute,
      listLoadsCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ListLoadsCommand::help,
      listLoadsCommand ) );

  auto listCommand{ std::make_shared< ListMediaSetsCommand >() };
  registry->command(
    "ListMediaSets",
    "List Media Sets",
    std::bind(
      &Arinc665Commands::ListMediaSetsCommand::execute,
      listCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ListMediaSetsCommand::help,
      listCommand ) );

  auto importXmlCommand{ std::make_shared< ImportMediaSetXmlCommand >() };
  registry->command(
    "ImportMediaSetXml",
    "Import Media Set XML",
    std::bind(
      &Arinc665Commands::ImportMediaSetXmlCommand::execute,
      importXmlCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportMediaSetXmlCommand::help,
      importXmlCommand ) );

  auto importCommand{ std::make_shared< ImportMediaSetCommand >() };
  registry->command(
    "ImportMediaSet",
    "Import Media Set",
    std::bind(
      &Arinc665Commands::ImportMediaSetCommand::execute,
      importCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::ImportMediaSetCommand::help,
      importCommand ) );

  auto removeMediaSetCommand{ std::make_shared< RemoveMediaSetCommand >() };
  registry->command(
    "RemoveMediaSet",
    "Remove Media Set",
    std::bind(
      &Arinc665Commands::RemoveMediaSetCommand::execute,
      removeMediaSetCommand,
      std::placeholders::_1 ),
    std::bind(
      &Arinc665Commands::RemoveMediaSetCommand::help,
      removeMediaSetCommand ) );
}

}

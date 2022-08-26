/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::CreateMediaSetManagerCommand.
 **/

#include "CreateMediaSetManagerCommand.hpp"

#include <arinc665/utils/JsonMediaSetManager.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands {

CreateMediaSetManagerCommand::CreateMediaSetManagerCommand() :
  optionsDescription{ "Create Media Set Manager" }
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager Directory"
  );
}

void CreateMediaSetManagerCommand::execute(
  const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "Create Media Set Manager\n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    Arinc665::Utils::JsonMediaSetManager::create( mediaSetManagerDirectory );
  }
  catch ( const boost::exception &e )
  {
    std::cerr << "Error: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cerr << "Error: UNKNOWN EXCEPTION\n";
  }
}

void CreateMediaSetManagerCommand::help()
{
  std::cout << "Create Media Set Manager\n" << optionsDescription;
}

}

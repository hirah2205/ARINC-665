/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::ListMediaSetsCommand.
 **/

#include "ListMediaSetsCommand.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>
#include <arinc665/utils/Printer.hpp>

#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands {

ListMediaSetsCommand::ListMediaSetsCommand() :
  optionsDescription{ "List Media Sets" }, checkFileIntegrity{}
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager Directory"
  )
  (
    "check-file-integrity",
    boost::program_options::value( &checkFileIntegrity )->default_value( true ),
    "Check Media Set File Integrity during Import"
  );
}

void ListMediaSetsCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "List Media Sets \n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    const auto mediaSetManager{ Arinc665::Utils::JsonMediaSetManager::load(
      mediaSetManagerDirectory,
      checkFileIntegrity ) };

    for ( const auto &mediaSet : mediaSetManager->manager()->mediaSets() )
    {
      std::cout << "Media Set:\n";

      Arinc665::Utils::printMediaSet( mediaSet.second, std::cout, "  ", "  " );

      std::cout << "\n";
    }
  }
  catch ( boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
  }
  catch ( boost::exception &e )
  {
    std::cerr
      << "Operation failed: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( std::exception &e )
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
  }
}

void ListMediaSetsCommand::help()
{
  std::cout << "List Media Set Manager\n" << optionsDescription;
}

}

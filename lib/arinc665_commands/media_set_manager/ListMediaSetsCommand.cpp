/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class
 *   Arinc665Commands::MediaSetManager::ListMediaSetsCommand.
 **/

#include "ListMediaSetsCommand.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetPrinter.hpp>

#include <arinc645/CheckValue.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands::MediaSetManager {

ListMediaSetsCommand::ListMediaSetsCommand() :
  optionsDescription{ "List ARINC 665 Media Sets Options" },
  checkFileIntegrity{}
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
    std::cout << "List ARINC 665 Media Sets \n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    const auto mediaSetManager{ Arinc665::Utils::MediaSetManager::load(
      mediaSetManagerDirectory,
      checkFileIntegrity ) };

    const auto mediaSets{ mediaSetManager->mediaSets() };

    if ( mediaSets.empty() )
    {
      std::cout << "*** No media sets within media set manger ***\n";
    }
    else
    {
      for ( const auto &[partNumber, mediaSet] : mediaSetManager->mediaSets() )
      {
        std::cout << "Media Set:\n";

        Arinc665::Utils::MediaSetPrinter_print(
          *mediaSet.first,
          std::cout,
          "  ",
          "  " );

        std::cout << "\n";
      }
    }
  }
  catch ( const boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Error: " << boost::diagnostic_information( e ) << "\n";
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

void ListMediaSetsCommand::help()
{
  std::cout
    << "List all Media Sets registered with the Media Set Manager\n"
    << optionsDescription;
}

}

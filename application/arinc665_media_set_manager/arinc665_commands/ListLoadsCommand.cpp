/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::ListLoadsCommand.
 **/

#include "ListLoadsCommand.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Load.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>

#include <helper/HexString.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands {

ListLoadsCommand::ListLoadsCommand() :
  optionsDescription{ "List Loads" },
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
    "Check File Integrity during Import"
  );
}

void ListLoadsCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "List Loads\n";

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

    for ( const auto &load : mediaSetManager->manager()->loads() )
    {
      std::cout
        << "Media Set P/N:         " << load->mediaSet()->partNumber()
        << "\n"
        << "Load Header File Name: " << load->name() << "\n"
        << "Load P/N:              " << load->partNumber() << "\n";

      if ( const auto loadType{ load->loadType() }; loadType )
      {
        std::cout
          << "Load Type:             " << loadType->first << " ("
          << Helper::to_hexstring( loadType->second ) << ")\n";
      }

      std::cout << "\n";
    }
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

void ListLoadsCommand::help()
{
  std::cout << "ARINC 665 List Loads\n" << optionsDescription;
}

}

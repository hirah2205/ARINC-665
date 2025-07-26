// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::MediaSetManager::CreateMediaSetManagerCommand.
 **/

#include "CreateMediaSetManagerCommand.hpp"

#include <arinc_665/utils/MediaSetManager.hpp>

#include <spdlog/spdlog.h>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands::MediaSetManager {

CreateMediaSetManagerCommand::CreateMediaSetManagerCommand() :
  optionsDescriptionV{ "Create ARINC 665 Media Set Manager Options" }
{
  optionsDescriptionV.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectoryV )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager directory."
  );
}

void CreateMediaSetManagerCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "Create ARINC 665 Media Set Manager\n";

    boost::program_options::variables_map variablesMap;
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters ).options( optionsDescriptionV ).run(),
      variablesMap );
    boost::program_options::notify( variablesMap );

    std::cout
      << std::format( "Media Set Manager directory: {}\n", mediaSetManagerDirectoryV.string() );

    auto mediaSetManager{ Arinc665::Utils::MediaSetManager::create( mediaSetManagerDirectoryV ) };
    assert( mediaSetManager );
  }
  catch ( const boost::program_options::error & )
  {
    // parsing errors are handled by command handler
    throw;
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << std::format( "Operation failed: {}\n", boost::diagnostic_information( e ) );
  }
  catch ( const std::exception &e )
  {
    std::cerr << std::format( "Operation failed: {}\n", e.what() );
  }
  catch ( ... )
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
  }
}

void CreateMediaSetManagerCommand::help()
{
  std::cout
    << "Create ARINC 665 Media Set Manager at the given location.\n\n"
    << optionsDescriptionV;
}

}

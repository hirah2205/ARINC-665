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
 * @brief Definition of Class
 *   Arinc665Commands::MediaSetManager::CreateMediaSetManagerCommand.
 **/

#include "CreateMediaSetManagerCommand.hpp"

#include "arinc_665/utils/MediaSetManager.hpp"

#include "helper/Logger.hpp"

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands::MediaSetManager {

CreateMediaSetManagerCommand::CreateMediaSetManagerCommand() :
  optionsDescription{ "Create ARINC 665 Media Set Manager Options" }
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager directory."
  );
}

void CreateMediaSetManagerCommand::execute(
  const Commands::Parameters &parameters )
{
  BOOST_LOG_FUNCTION()

  try
  {
    std::cout << "Create ARINC 665 Media Set Manager\n";

    boost::program_options::variables_map variablesMap{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      variablesMap );
    boost::program_options::notify( variablesMap );

    std::cout
      << "Media Set Manager directory: "
      << mediaSetManagerDirectory.string() << "\n";

    auto mediaSetManager{
      Arinc665::Utils::MediaSetManager::create( mediaSetManagerDirectory ) };
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
      << "Operation failed: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
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
    << optionsDescription;
}

}

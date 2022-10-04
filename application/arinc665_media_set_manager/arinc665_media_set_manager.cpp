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

#include <arinc665_commands/Arinc665Commands.hpp>

#include <commands/CommandRegistry.hpp>
#include <commands/CommandUtils.hpp>

#include <helper/Logger.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>
#include <cstdlib>

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

  Arinc665Commands::registerCommands( registry );

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

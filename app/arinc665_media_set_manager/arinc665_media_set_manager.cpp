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
 * @brief ARINC 665 Media Set Manager.
 **/

#include <arinc665_commands/Arinc665Commands.hpp>

#include <arinc665/Version.hpp>

#include <commands/CommandRegistry.hpp>
#include <commands/Utils.hpp>

#include <helper/Logger.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <cstdlib>
#include <iostream>

/**
 * @brief Application Entry Point.
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Application exit status.
 **/
int main( int argc, char * argv[] );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::warning, false );

  try
  {
    std::cout
      << "ARINC 665 Media Set Manager - "
      << Arinc665::Version::VersionInformation << "\n";

    auto registry{ Commands::CommandRegistry::instance() };

    Arinc665Commands::registerCommands( registry );

    const auto result{
      Commands::Utils_commandLineHandler( registry )( argc, argv ) };

    return result;
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Error: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr
      << "Error: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "Unknown exception occurred\n";
    return EXIT_FAILURE;
  }
}

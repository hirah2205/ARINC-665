// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set XML Printer Application.
 **/

#include <arinc_665/media/MediaSet.hpp>

#include <arinc_665/utils/Arinc665Xml.hpp>
#include <arinc_665/utils/MediaSetPrinter.hpp>

#include <arinc_665/Arinc665Exception.hpp>

#include <helper/Dump.hpp>

#include <spdlog/spdlog.h>

#include <boost/exception/all.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
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

int main( const int argc, char * argv[] )
{
  spdlog::set_level( spdlog::level::info );

  try
  {
    std::cout << "ARINC 665 Media Set XML Printer" << "\n";

    boost::program_options::options_description optionsDescription{ "Print ARINC 665 XML options" };

    std::filesystem::path xmlPath;

    optionsDescription.add_options()
    (
      "help",
      "Print Help"
    )
    (
      "xml-file",
      boost::program_options::value( &xmlPath )->required(),
      "ARINC 665 media set description XML"
    );

    boost::program_options::variables_map variablesMap;
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, optionsDescription ),
      variablesMap );

    if ( 0U != variablesMap.count( "help" ) )
    {
      std::cout
        << "Prints the ARINC 665 Media Set XML.\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( variablesMap );

    std::cout << "List XML" << "\n";

    // load ARINC 665 XML file
    const auto [ mediaSet, filePathMapping ]{ Arinc665::Utils::Arinc665Xml_load( xmlPath ) };

    Arinc665::Utils::MediaSetPrinter_print( *mediaSet, std::cout, "  ", "  " );

    return EXIT_SUCCESS;
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr << std::format(
      "Error parsing command line: {}\n"
      "Enter '{} --help' for command line description.\n",
      e.what(),
      argv[ 0 ] );
    return EXIT_FAILURE;
  }
  catch ( const boost::exception &e )
  {
    std::cerr << std::format( "Error: {}\n", boost::diagnostic_information( e ) );
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr << std::format( "Error: {}\n", boost::diagnostic_information( e ) );
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "Unknown exception occurred\n";
    return EXIT_FAILURE;
  }
}

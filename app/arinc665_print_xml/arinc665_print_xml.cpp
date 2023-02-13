/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set XML Printer Application.
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/MediaSetPrinter.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

/**
 * @brief Entry point of application.
 *
 * @param[in] argc
 *   Argument count.
 * @param[in] argv
 *   Argument values.
 *
 * @return Application exit status.
 **/
int main( int argc, char * argv[] );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  try
  {
    std::cout << "ARINC 665 Media Set XML Printer" << "\n";

    boost::program_options::options_description optionsDescription{
      "ARINC 665 List options" };

    std::filesystem::path xmlPath{};

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

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription ),
      vm );

    if ( 0U != vm.count( "help" ) )
    {
      std::cout
        << "Prints the ARINC 665 Media Set XML.\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    std::cout << "List XML" << "\n";

    // ARINC 665 XML instance
    auto xml{ Arinc665::Utils::Arinc665Xml::instance() };

    // load XML file
    const auto [ mediaSet, filePathMapping ]{ xml->loadFromXml( xmlPath ) };

    Arinc665::Utils::MediaSetPrinter_print( *mediaSet, std::cout, "  ", "  " );
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
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
      << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

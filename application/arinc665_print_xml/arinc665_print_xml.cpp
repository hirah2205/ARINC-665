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

/**
 * @dir
 * @brief ARINC 665 Media Set XML Printer Application.
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/Printer.hpp>

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
  std::cout << "ARINC 665 Media Set XML Printer" << "\n";

  boost::program_options::options_description options{
    "ARINC 665 List options"};

  std::filesystem::path xmlPath{};

  options.add_options()
  (
    "help",
    "Print Help"
  )
  (
    "xml",
    boost::program_options::value( &xmlPath )->required(),
    "XML File"
  );

  try
  {
    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, options ),
      vm );

    if ( vm.count( "help" ) != 0 )
    {
      std::cout
        << "Prints the ARINC 665 Media Set XML.\n"
        << options;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    std::cout << "List XML" << "\n";

    // ARINC 665 XML instance
    auto xml{ Arinc665::Utils::Arinc665Xml::instance() };

    // load XML file
    auto result{ xml->loadFromXml( xmlPath ) };

    auto mediaSet{ std::get< 0>( result ) };

    Arinc665::Utils::printMediaSet( mediaSet, std::cout, "  ", "  " );
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << "Error parsing command line: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cout << "Error: " << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cout << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARIN 665 Media Set Manager Application.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set Manager Application.
 **/

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetConfiguration.hpp>
#include <arinc665/utils/Printer.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <filesystem>

/**
 * @brief Program entry point
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
  Helper::initLogging( Helper::Severity::info );

  boost::program_options::options_description optionsDescription{};
  std::filesystem::path mediaSetConfig{};
  bool checkFileIntegrity{};

  optionsDescription.add_options()
  (
    "media-set-config",
     boost::program_options::value( &mediaSetConfig )->required(),
     "Media Set configuration"
  )
  (
    "check-file-integrity",
    boost::program_options::value( &checkFileIntegrity )->default_value( true ),
    "Check File Integrity during Import"
  );

  try
  {
    boost::program_options::variables_map options{};
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription ),
      options );
    boost::program_options::notify( options );

    boost::property_tree::ptree config;
    boost::property_tree::json_parser::read_json(
      mediaSetConfig.string(),
      config );

    auto configDir{
      mediaSetConfig.has_parent_path() ?
        std::filesystem::absolute( mediaSetConfig.parent_path() ) :
        std::filesystem::current_path() };
    Arinc665::Utils::MediaSetConfiguration mediaSetConfiguration{ config };

    auto mediaSetManager{ Arinc665::Utils::MediaSetManager::instance(
      configDir,
      mediaSetConfiguration,
      checkFileIntegrity ) };

    for ( const auto &mediaSet : mediaSetManager->mediaSets() )
    {
      std::cout << "Media Set:\n";

      Arinc665::Utils::printMediaSet( mediaSet, std::cout, "  ", "  " );

      std::cout << "\n";
    }
  }
  catch ( boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr
      << "Operation failed: " << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

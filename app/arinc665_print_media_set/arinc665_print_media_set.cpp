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
 * @brief ARINC 665 Media Set Print Application.
 **/

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>
#include <arinc665/utils/MediaSetPrinter.hpp>
#include <arinc665/utils/MediaSetDefaults.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Version.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <iostream>

//! Media Directories
using Directories = std::vector< std::filesystem::path >;

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

/**
 * @brief Loads the media set from the given directory.
 *
 * @param[in] mediaSetDirectories
 *   Media Set Directories
 * @param[in] checkFileIntegrity
 *   Check File Integrity
 *
 * @return Loaded Media Set.
 **/
static Arinc665::Utils::MediaSetDecompilerResult loadMediaSet(
  const Directories &mediaSetDirectories,
  bool checkFileIntegrity );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::warning, true );

  try
  {
    std::cout
      << "ARINC 665 Media Set Printer - "
      << Arinc665::Version::VersionInformation << "\n";

    boost::program_options::options_description optionsDescription{
      "ARINC 665 Media Set Printer Options" };

    // directories which contains the medias
    std::vector< std::filesystem::path > directories{};

    // Check File Integrity
    bool checkFileIntegrity{};

    optionsDescription.add_options()
    (
      "help",
      "Print Help"
    )
    (
      "directory",
      boost::program_options::value( &directories)->required()->multitoken(),
      "media directories (can be passed multiple times)"
    )
    (
      "check-file-integrity",
      boost::program_options::value( &checkFileIntegrity )
        ->default_value(
          Arinc665::Utils::MediaSetDefaults::DefaultCheckFileIntegrity ),
      "Check File Integrity during decompilation."
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
        << "Prints the ARINC 665 Media Set located in the given directory\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    const auto [mediaSet,checkValues]{
      loadMediaSet( directories, checkFileIntegrity ) };

    std::cout << "Media Set: \n";
    Arinc665::Utils::MediaSetPrinter_print( *mediaSet, std::cout, "", "  " );

    std::cout << "Check Values: \n";
    Arinc665::Utils::MediaSetPrinter_print( checkValues, std::cout, "", "  " );

    return EXIT_SUCCESS;
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0]
      << " --help for command line description\n";
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
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "Unknown exception occurred\n";
    return EXIT_FAILURE;
  }
}

static Arinc665::Utils::MediaSetDecompilerResult loadMediaSet(
  const Directories &mediaSetDirectories,
  const bool checkFileIntegrity )
{
  Arinc665::Utils::MediaPaths mediaPaths{};
  for (
    Arinc665::MediumNumber mediumNumber{};
    const auto &mediumDirectory : mediaSetDirectories )
  {
    mediaPaths.try_emplace( mediumNumber, mediumDirectory );
    ++mediumNumber;
  }

  auto decompiler{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };

  decompiler
    ->checkFileIntegrity( checkFileIntegrity )
    .mediaPaths( std::move( mediaPaths ) );

  return ( *decompiler )();
}

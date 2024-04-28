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
 * @brief ARINC 665 Media Set Decompiler Application.
 **/

#include "arinc_665/media/MediaSet.hpp"
#include "arinc_665/media/RegularFile.hpp"

#include "arinc_665/files/MediaSetInformation.hpp"

#include "arinc_665/utils/FilesystemMediaSetDecompiler.hpp"
#include "arinc_665/utils/Arinc665Xml.hpp"
#include "arinc_665/utils/MediaSetDefaults.hpp"

#include "arinc_665/Arinc665Exception.hpp"
#include "arinc_665/Version.hpp"

#include "arinc_645/CheckValue.hpp"

#include "helper/Exception.hpp"
#include "helper/Logger.hpp"

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>
#include <format>

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
 * @brief Progress Handler
 *
 * @param[in] partNumber
 *   Media Set Part Number
 * @param[in] medium
 *   Medium information.
 **/
static void progress(
  std::string_view partNumber,
  std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium );

int main( int argc, char * argv[] )
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  try
  {
    std::cout
      << "ARINC 665 Media Set Decompiler - "
      << Arinc665::Version::VersionInformation << "\n";

    boost::program_options::options_description optionsDescription{
      "ARINC 665 Media Set Decompiler Options" };

    // Media source directories
    std::vector< std::filesystem::path > mediaSourceDirectories{};

    // Media Set XML file
    std::filesystem::path mediaSetXmlFile{};

    // Check File Integrity
    bool checkFileIntegrity{};

    optionsDescription.add_options()
    (
      "help,h",
      "print this help screen"
    )
    (
      "source-directory,d",
      boost::program_options::value( &mediaSourceDirectories )
        ->required()
        ->composing(),
      "ARINC 665 media source directories."
    )
    (
      "xml-file,f",
      boost::program_options::value( &mediaSetXmlFile )->required(),
      "ARINC 665 media set description XML output file."
    )
    (
      "check-file-integrity,i",
      boost::program_options::value( &checkFileIntegrity )
        ->default_value(
          Arinc665::Utils::MediaSetDefaults::DefaultCheckFileIntegrity ),
      "Check File Integrity during decompilation."
    );

    boost::program_options::variables_map variablesMap{};
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription ),
      variablesMap );

    if ( 0U != variablesMap.count( "help" ) )
    {
      std::cout
        << "Decompiles the ARINC 665 Media Set and stores the representation as"
           "ARINC Media Set file.\n\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( variablesMap );

    // Fill Media Paths list
    Arinc665::Utils::MediaPaths mediaPaths{};
    for ( const auto &mediumSourceDirectory : mediaSourceDirectories )
    {
      const auto mediumInformation{
        Arinc665::Utils::getMediumInformation( mediumSourceDirectory ) };

      if ( !mediumInformation )
      {
        BOOST_THROW_EXCEPTION(
          boost::program_options::invalid_option_value{
            mediumSourceDirectory } );
      }

      mediaPaths.try_emplace(
        mediumInformation->mediaSequenceNumber,
        mediumSourceDirectory );
    }

    auto decompiler{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };
    assert( decompiler );

    decompiler
      ->progressHandler( std::bind_front( progress ) )
      .checkFileIntegrity( checkFileIntegrity )
      .mediaPaths( std::move( mediaPaths ) );

    // perform import
    const auto &[ mediaSet, checkValues ]{ ( *decompiler )() };

    Arinc665::Utils::FilePathMapping fileMapping{};

    // iterate over all files to add file-mapping
    for ( const auto &file : mediaSet->recursiveFiles() )
    {
      std::filesystem::path filePath(
        mediaSourceDirectories[ static_cast< uint8_t >( file->effectiveMediumNumber() ) - 1]
          / file->path().relative_path() );

      fileMapping.try_emplace( file, filePath );
    }

    // export to ARINC 665 XML file
    Arinc665::Utils::Arinc665Xml_save(
      *mediaSet,
      fileMapping,
      mediaSetXmlFile );

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

static void progress(
  std::string_view partNumber,
  std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium )
{
  std::cout << std::format(
    "Loading {} {}:{}\n",
    partNumber,
    static_cast< std::string >( medium.first ),
    static_cast< std::string >( medium.second ) );
}

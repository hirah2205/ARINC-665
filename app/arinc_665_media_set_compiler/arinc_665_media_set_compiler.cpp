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
 * @brief ARINC 665 Media Set Compiler Application.
 **/

#include <arinc_665/media/Media.hpp>

#include <arinc_665/utils/Arinc665Xml.hpp>
#include <arinc_665/utils/FilesystemMediaSetCompiler.hpp>
#include <arinc_665/utils/FileCreationPolicyDescription.hpp>
#include <arinc_665/utils/MediaSetDefaults.hpp>

#include <arinc_665/media/MediaSet.hpp>

#include <arinc_665/Arinc665Exception.hpp>
#include <arinc_665/SupportedArinc665VersionDescription.hpp>
#include <arinc_665/Version.hpp>

#include <helper/Exception.hpp>
#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <cstdlib>
#include <filesystem>
#include <format>
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

  Helper::initLogging( Helper::Severity::info );

  try
  {
    std::cout
      << "ARINC 665 Media Set Compiler - "
      << Arinc665::Version::VersionInformation << "\n";

    const auto &fileCreatePolDesc{
      Arinc665::Utils::FileCreationPolicyDescription::instance() };

    const std::string fileCreationPolicyValues{
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::None ) } + "': Create never\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::NoneExisting ) } + "': Create none-existing\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::All ) } + "': Create all" };

    const auto &versionDesc{ Arinc665::SupportedArinc665VersionDescription::instance() };

    const std::string versionValues{
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement2 ) } +   "': ARINC 665-2\n" +
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement345 ) } +  "': ARINC 665-3/4/5" };

    // Media Set XML file
    std::filesystem::path mediaSetXmlFile;
    // Media Set source directory
    std::filesystem::path mediaSetSourceDirectory;
    // Create batch file policy
    Arinc665::Utils::FileCreationPolicy createBatchFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // Create Load Header file policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // ARINC 665 Version
    Arinc665::SupportedArinc665Version version{
      Arinc665::SupportedArinc665Version::Supplement2 };
    // Media Set destination directory
    std::filesystem::path mediaSetDestinationDirectory;
    // Media Set name
    std::string mediaSetName;

    boost::program_options::options_description optionsDescription{
      "ARINC 665 Media Set Compiler Options" };

    optionsDescription.add_options()
    (
      "help,h",
      "print this help screen"
    )
    (
      "xml-file,f",
      boost::program_options::value( &mediaSetXmlFile )
        ->required(),
      "ARINC 665 media set description XML file."
    )
    (
      "source-directory,s",
      boost::program_options::value( &mediaSetSourceDirectory )->required(),
      "ARINC 665 source directory."
    )
    (
      "create-batch-files,b",
      boost::program_options::value( &createBatchFiles )
        ->default_value(
          Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy ),
      ( std::string( "batch-files creation policy:\n" ) + fileCreationPolicyValues ).c_str()
    )
    (
      "create-load-header-files,l",
      boost::program_options::value( &createLoadHeaderFiles )
        ->default_value(
          Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy ),
      ( std::string( "Load-headers-files creation policy:\n" ) + fileCreationPolicyValues).c_str()
    )
    (
      "version,v",
      boost::program_options::value( &version )->default_value(
        Arinc665::Utils::MediaSetDefaults::DefaultVersion ),
      ( std::string( "ARINC 665 Version:\n" ) + versionValues ).c_str()
    )
    (
      "destination-directory,d",
      boost::program_options::value( &mediaSetDestinationDirectory )
        ->default_value( std::filesystem::current_path() ),
      "Output directory for ARINC 665 media set"
    )
    (
      "media-set-name,n",
      boost::program_options::value( &mediaSetName ),
      "Media Set Name to use.\n"
      "Is set to part number when not provided"
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
        << "Compiles an ARINC 665 Media Set\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( variablesMap );

    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( mediaSetXmlFile );

    auto compiler{ Arinc665::Utils::FilesystemMediaSetCompiler::create() };

    // set exporter parameters
    compiler
      ->mediaSet( mediaSet )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles )
      .sourceBasePath( mediaSetSourceDirectory )
      .filePathMapping( fileMapping )
      .outputBasePath( mediaSetDestinationDirectory );

    if ( !mediaSetName.empty() )
    {
      compiler->mediaSetName( mediaSetName );
    }

    const auto &[mediaSetPath, mediaPaths]{ ( *compiler )() };

    std::cout << "Created Media Set " << mediaSetName << " in \n";
    for ( const auto &[ mediumNumber, mediumPath ] : mediaPaths )
    {
      std::cout << std::format(
        " * [%1]: %2\n",
        static_cast< std::string >( mediumNumber ),
        ( mediaSetPath / mediumPath ).string() );
    }

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

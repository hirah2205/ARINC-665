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

#include <arinc665/media/Media.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetExporter.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/Version.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <iostream>
#include <filesystem>
#include <cstdlib>

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
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::None)} + "': Create never\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::NoneExisting)} + "': Create none-existing\n" +
      "* '" + std::string{ fileCreatePolDesc.name( Arinc665::Utils::FileCreationPolicy::All)} + "': Create all" };

    const auto &versionDesc{ Arinc665::SupportedArinc665VersionDescription::instance() };

    const std::string versionValues{
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement2 ) } +   "': ARINC 665-2\n" +
      "* '" + std::string{ versionDesc.name( Arinc665::SupportedArinc665Version::Supplement345 ) } +  "': ARINC 665-3/4/5" };

    // Media Set XML file
    std::filesystem::path mediaSetXmlFile;
    // Media Set source directory
    std::filesystem::path mediaSetSourceDirectory;
    // Media Set destination directory
    std::filesystem::path mediaSetDestinationDirectory;
    // Create batch file policy
    Arinc665::Utils::FileCreationPolicy createBatchFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // Create Load Header file policy
    Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles{
      Arinc665::Utils::FileCreationPolicy::None };
    // ARINC 665 Version
    Arinc665::SupportedArinc665Version version{
      Arinc665::SupportedArinc665Version::Supplement2 };

    boost::program_options::options_description optionsDescription{
      "ARINC 665 Media Set Compiler Options" };

    optionsDescription.add_options()
    (
      "help",
      "print this help screen"
    )
    (
      "xml-file",
      boost::program_options::value( &mediaSetXmlFile )->required(),
      "ARINC 665 media set description XML"
    )
    (
      "source-directory",
      boost::program_options::value( &mediaSetSourceDirectory )->required(),
      "ARINC 665 source directory"
    )
    (
      "destination-directory",
      boost::program_options::value( &mediaSetDestinationDirectory )->required(),
      "Output directory for ARINC 665 media set"
    )
    (
      "create-batch-files",
      boost::program_options::value(
        &createBatchFiles )->default_value( Arinc665::Utils::FileCreationPolicy::None ),
      ( std::string( "batch-files creation policy:\n" ) + fileCreationPolicyValues ).c_str()
    )
    (
      "create-load-header-files",
      boost::program_options::value( &createLoadHeaderFiles)->default_value(
        Arinc665::Utils::FileCreationPolicy::None ),
      ( std::string( "Load-headers-files creation policy:\n" ) + fileCreationPolicyValues).c_str()
    )
    (
      "version",
      boost::program_options::value( &version )->default_value(
        Arinc665::SupportedArinc665Version::Supplement2 ),
      ( std::string( "ARINC 665 Version:\n" ) + versionValues ).c_str()
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
        << "Compiles an ARINC 665 Media Set\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( mediaSetXmlFile );

    // Add Media Set Part Number to Output Path
    mediaSetDestinationDirectory /= mediaSet->partNumber();

    if ( std::filesystem::exists( mediaSetDestinationDirectory ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories( mediaSetDestinationDirectory );

    auto exporter{ Arinc665::Utils::FilesystemMediaSetExporter::create() };

    // set exporter parameters
    exporter
      ->mediaSet( mediaSet )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles )
      .mediaSetBasePath( mediaSetDestinationDirectory )
      .sourceBasePath( mediaSetSourceDirectory )
      .filePathMapping( fileMapping );

    (*exporter)();
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    std::cerr
      << "Arinc665Exception in compiler: "
      << boost::diagnostic_information( e ) << "\n";
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

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

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/Version.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <vector>
#include <cstdlib>
#include <iostream>

/**
 * @brief Entry point of application.
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Success state of this operation.
 **/
int main( int argc, char const * argv[] );

int main( int argc, char const * argv[] )
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
      "help",
      "print this help screen"
    )
    (
      "source-directory",
      boost::program_options::value(
        &mediaSourceDirectories )->required()->composing(),
      "ARINC 665 media source directories"
    )
    (
      "xml-file",
      boost::program_options::value( &mediaSetXmlFile )->required(),
      "Output ARINC 665 media set description XML"
    )
    (
      "check-file-integrity",
      boost::program_options::value( &checkFileIntegrity )
        ->default_value( true ),
      "Check File Integrity during Import"
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
        << "Decompiles the ARINC 665 Media Set\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    // Fill Media Paths list
    Arinc665::Utils::MediaPaths mediaPaths{};
    for ( const auto &mediumSourceDirectory : mediaSourceDirectories )
    {
      const auto mediumInformation{
        Arinc665::Utils::getMediumInformation( mediumSourceDirectory ) };

      if ( !mediumInformation )
      {
        BOOST_THROW_EXCEPTION(
          boost::program_options::invalid_option_value( mediumSourceDirectory ) );
      }

      mediaPaths.try_emplace(
        mediumInformation->mediaSequenceNumber,
        mediumSourceDirectory );
    }

    auto decompiler{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };
    assert( decompiler );

    decompiler
      ->checkFileIntegrity( checkFileIntegrity )
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
      << "Error in decompiler: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr
      << "Error in decompiler: "
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

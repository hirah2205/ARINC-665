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

/**
 * @dir
 * @brief ARINC 665 Media Set Decompiler Application.
 **/

#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/utils/MediaSetImporter.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

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
int main( int argc, char const * argv[] );

/**
 * @brief Returns File Size of given File.
 *
 * @param[in] mediaSourceDirectories
 *   Media Source Directory Mapping.
 * @param[in] mediumNumber
 *   Medium number.
 * @param[in] path
 *   Path of file on Medium.
 *
 * @return File Size
 **/
static size_t getFileSize(
  const std::vector< std::filesystem::path > &mediaSourceDirectories,
  uint8_t mediumNumber,
  const std::filesystem::path &path );

/**
 * @brief Reads the give file and returns the data.
 *
 * @param[in] mediaSourceDirectories
 *   Media Source Directory Mapping.
 * @param[in] mediumNumber
 *   Medium number.
 * @param[in] path
 *   Path of file on Medium.
 *
 * @return THe read file data.
 *
 * @throw Arinc665Exception
 *   If file does not exist or cannot be read.
 **/
static Arinc665::Files::RawFile readFile(
  const std::vector< std::filesystem::path > &mediaSourceDirectories,
  uint8_t mediumNumber,
  const std::filesystem::path &path );

int main( int argc, char const * argv[] )
{
  std::cout << "ARINC 665 Media Set Decompiler" << "\n";

  Helper::initLogging( Helper::Severity::info);

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
    boost::program_options::value( &checkFileIntegrity )->default_value( true ),
    "Check File Integrity during Import"
  );

  try
  {
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

    // create importer
    auto importer{ Arinc665::Utils::MediaSetImporter::create() };

    importer->fileSizeHandler(
      std::bind_front( &getFileSize, mediaSourceDirectories ) )
      .readFileHandler( std::bind_front( &readFile, mediaSourceDirectories ) )
      .checkFileIntegrity( checkFileIntegrity );

    // perform import
    auto result{ (*importer)() };

    Arinc665::Utils::Arinc665Xml::FilePathMapping fileMapping{};

    // iterate over files
    for ( const auto &file : result->files() )
    {
      std::filesystem::path filePath(
        mediaSourceDirectories[ file->medium()->mediumNumber() - 1]
          / file->path().relative_path());

      fileMapping.try_emplace( file, filePath );
    }

    // XML exporter
    const auto xml{ Arinc665::Utils::Arinc665Xml::instance() };

    // export the XML file
    xml->saveToXml( result, fileMapping, mediaSetXmlFile );
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
      << "Error in decompiler: " << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr << "std exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static size_t getFileSize(
  const std::vector< std::filesystem::path > &mediaSourceDirectories,
  const uint8_t mediumNumber,
  const std::filesystem::path &path )
{
  auto filePath{
    mediaSourceDirectories.at( mediumNumber - 1U ) / path.relative_path() };

  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << boost::errinfo_file_name{ filePath.string() }
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  return std::filesystem::file_size( filePath );
}

static Arinc665::Files::RawFile readFile(
  const std::vector< std::filesystem::path > &mediaSourceDirectories,
  const uint8_t mediumNumber,
  const std::filesystem::path &path )
{
  // check medium number
  if ( mediumNumber > mediaSourceDirectories.size() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Medium number unknown" } );
  }

  auto filePath{ mediaSourceDirectories[ mediumNumber-1] / path.relative_path() };

  // check existence of file
  if ( !std::filesystem::is_regular_file( filePath))
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << boost::errinfo_file_name{ filePath.string() }
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  Arinc665::Files::RawFile data( std::filesystem::file_size( filePath ) );

  // load file
  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in };

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // read the data to the buffer
  file.read(
    (char *)&data.at( 0 ),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}

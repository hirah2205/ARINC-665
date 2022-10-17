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

/**
 * @dir
 * @brief ARINC 665 Media Set Print Application.
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/MediaSetImporter.hpp>
#include <arinc665/utils/MediaSetPrinter.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

//! Media Directories
using Directories = std::vector< std::filesystem::path >;

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
int main( int argc, char const * argv[] );

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
static Arinc665::Media::MediaSetPtr loadMediaSet(
  const Directories &mediaSetDirectories,
  bool checkFileIntegrity );

/**
 * @brief Returns the File Size of the given File.
 *
 * @param[in] mediaSetDirectories
 *
 * @param[in] mediaSetDirectories
 *   Media Set Directory Mapping.
 * @param[in] mediumNumber
 *   Medium number.
 * @param[in] path
 *   Path of file on medium.
 * @return
 */
static size_t getFileSize(
  const Directories &mediaSetDirectories,
  uint8_t mediumNumber,
  const std::filesystem::path &path );

/**
 * @brief Reads the give file and returns the data.
 *
 * @param[in] mediaSetDirectories
 *   Media Set Directory Mapping.
 * @param[in] mediumNumber
 *   Medium number,
 * @param[in] path
 *   Path of file on medium.
 *
 * @return THe read file data.
 *
 * @throw Arinc665Exception
 *   If file does not exist or cannot be read.
 **/
static Arinc665::Files::RawFile readFile(
  const Directories &mediaSetDirectories,
  uint8_t mediumNumber,
  const std::filesystem::path &path );

int main( int argc, char const * argv[] )
{
  std::cout << "ARINC 665 Media Set Printer\n";

  Helper::initLogging( Helper::Severity::warning, true );

  boost::program_options::options_description optionsDescription{
    "ARINC 665 Media Set Printer options" };

  // directories which contains the medias
  std::vector< std::filesystem::path > directories{};
  bool checkFileIntegrity{ true };

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
    boost::program_options::value( &checkFileIntegrity )->default_value( true ),
    "Check File Integrity during Import"
  );

  try
  {
    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, optionsDescription ),
      vm );

    if ( 0U != vm.count( "help" ) )
    {
      std::cout
        << "Prints the ARINC 665 Media Set located in the given directory\n"
        << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    auto mediaSet{ loadMediaSet( directories, checkFileIntegrity ) };

    Arinc665::Utils::MediaSetPrinter_print( *mediaSet, std::cout, "", "  " );
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
      << "Error: " << boost::diagnostic_information( e ) << "\n";
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

static Arinc665::Media::MediaSetPtr loadMediaSet(
  const Directories &mediaSetDirectories,
  const bool checkFileIntegrity )
{
  auto importer{ Arinc665::Utils::MediaSetImporter::create() };

  importer->fileSizeHandler(
    std::bind_front( &getFileSize, mediaSetDirectories ) )
  .readFileHandler( std::bind_front( &readFile, mediaSetDirectories ) )
  .checkFileIntegrity( checkFileIntegrity );

  auto mediaSet{ (*importer)() };

  return mediaSet.first;
}

static size_t getFileSize(
  const Directories &mediaSetDirectories,
  const uint8_t mediumNumber,
  const std::filesystem::path &path )
{
  auto filePath{
    mediaSetDirectories.at( mediumNumber - 1U ) / path.relative_path() };

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
  const Directories &mediaSetDirectories,
  const uint8_t mediumNumber,
  const std::filesystem::path &path )
{
  auto filePath{
    mediaSetDirectories.at( mediumNumber - 1U ) / path.relative_path() };

  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << boost::errinfo_file_name{ filePath.string() }
      << Helper::AdditionalInfo{ "File not found" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  Arinc665::Files::RawFile data( std::filesystem::file_size( filePath ) );

  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in };

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
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

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

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

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
int main( int argc, char const * argv[]);

/**
 * @brief Loads the media set from the given directory.
 *
 * @param[in] mediaSetDirectories
 *   Media Set Directories
 * @param[in] checkFileIntegrity
 *   Check File Integrity
 *
 * @return The loaded media set.
 **/
static Arinc665::Media::MediaSetPtr loadMediaSet(
  const Directories &mediaSetDirectories,
  bool checkFileIntegrity );

/**
 * @brief Decodes an prints the content of the Media Set.
 *
 * @param[in] mediaSet
 *   The media set to print.
 **/
static void printMediaSet( Arinc665::Media::MediaSetPtr &mediaSet );

int main( int argc, char const * argv[] )
{
  Helper::initLogging( Helper::Severity::warning, true );

  std::cout
    << "ARINC 665 Media Set Printer\n";

  boost::program_options::options_description options{
    "ARINC 665 Media Set Printer options"};

  // directories which contains the medias
  std::vector< std::filesystem::path > directories{};
  bool checkFileIntegrity{ true };

  options.add_options()
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
    boost::program_options::variables_map vm;
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, options ),
      vm);

    if ( vm.count( "help") != 0)
    {
      std::cout
        << "Prints the ARINC 665 Media Set located in the given directory\n"
        << options;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm);

    std::cout << "\n" << "Load Media Set\n";
    auto mediaSet{ loadMediaSet( directories, checkFileIntegrity ) };

    std::cout << "\n" << "Print Media Set\n";
    printMediaSet( mediaSet);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information(e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cout << "std exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


static Arinc665::Media::MediaSetPtr loadMediaSet(
  const Directories &mediaSetDirectories,
  const bool checkFileIntegrity )
{
  auto importer{ Arinc665::Utils::MediaSetImporter::create() };

  // read file handler
  importer->readFileHandler(
    [&mediaSetDirectories](
      const uint8_t mediumNumber,
      const std::filesystem::path &path)->Arinc665::File::RawFile {
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

        Arinc665::File::RawFile data( std::filesystem::file_size( filePath ) );

        std::ifstream file(
          filePath.string().c_str(),
          std::ifstream::binary | std::ifstream::in );

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
      } )
    .checkFileIntegrity( checkFileIntegrity );

  auto mediaSet{ (*importer)() };

  std::cout
    << "Media Set PN: \""
      << mediaSet->partNumber() << "\"\n"
    << "Number of Media Set Members: "
      << std::dec << (int)mediaSet->numberOfMedia() << "\n";

  return mediaSet;
}

static void printMediaSet( Arinc665::Media::MediaSetPtr &mediaSet)
{
  std::cout << "Media Set \"" << mediaSet->partNumber() << "\"\n";

  // iterate over files
  std::cout << " * Files " << "\n";

  for ( auto const &file : mediaSet->files() )
  {
    std::cout << "   * File " << file->path().generic_string() << " (";
    switch ( file->fileType() )
    {
      case Arinc665::Media::BaseFile::FileType::RegularFile:
        std::cout << "Regular File";
        break;

      case Arinc665::Media::BaseFile::FileType::BatchFile:
        std::cout << "Batch File";
        break;

      case Arinc665::Media::BaseFile::FileType::LoadFile:
        std::cout << "Load Header File";
        break;

      default:
        std::cout << "Illegal Value";
        break;
    }
    std::cout << ")\n";
  }

  // iterate over loads
  std::cout << " * Loads " << "\n";

  for ( auto const &load : mediaSet->loads())
  {
    std::cout
      << "   * Load " << load->name() << " " << load->partNumber() << "\n";

    std::cout << "       Compatible THW IDs\n";
    // iterate over THW ID list
    for ( auto const &[thwId,positions] : load->targetHardwareIdPositions() )
    {
      std::cout << "        * " << thwId << "\n";

      for ( auto const &position : positions )
      {
        std::cout << "          * " << position << "\n";
      }
    }

    std::cout << "       Data Files\n";
    // iterate over Data Files
    for ( const auto & dataFile : load->dataFiles())
    {
      std::cout
        << "        * "
        << dataFile.first.lock()->name() << " "
        << dataFile.second << "\n";
    }

    std::cout << "       Support Files\n";
    // iterate over Support Files
    for ( const auto & supportFile : load->supportFiles())
    {
      std::cout
        << "        * "
        << supportFile.first.lock()->name() << " "
        << supportFile.second << "\n";
    }
  }

  // iterate over Batches
  std::cout << " * Batches " << "\n";

  for ( auto const &batch : mediaSet->batches())
  {
    std::cout
      << "   * Batch " << batch->name() << " " << batch->partNumber() << " " << batch->comment() << "\n";

    std::cout << "       Targets\n";
    // iterate over Target list
    for ( auto const & [target,loads] : batch->targets())
    {
      std::cout << "        * " << target << "\n";
      for ( const auto &load : loads )
      {
        std::cout << "          * " << load.lock()->name() << "\n";
      }
    }
  }
}

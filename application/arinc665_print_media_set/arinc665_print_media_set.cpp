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

#include <arinc665/utils/Arinc665Utils.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

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
int main( int argc, char const * argv[]);

/**
 * @brief Loads the media set from the given directory.
 *
 * @param[in] mediaSetDirectory
 *   Media Set directory
 *
 * @return The loaded media set.
 **/
static Arinc665::Media::MediaSetPtr loadMediaSet(
  const std::filesystem::path &mediaSetDirectory);

/**
 * @brief Decodes an prints the content of the Media Set.
 *
 * @param[in] mediaSet
 *   The media set to print.
 **/
static void printMediaSet( Arinc665::Media::MediaSetPtr &mediaSet);


int main( int argc, char const * argv[])
{
  std::cout
    << "ARINC 665 Media Set Printer\n";

  boost::program_options::options_description options{ "ARINC 665 Media Set Printer options"};

  // directory which contains the medium 1 of the media set
  std::filesystem::path directory;

  options.add_options()
  (
    "help",
    "Print Help"
  )
  (
    "directory",
    boost::program_options::value( &directory)->required(),
    "start directory"
  );

  try
  {
    boost::program_options::variables_map vm;
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, options),
      vm);

    if ( vm.count( "help") != 0)
    {
      std::cout
        << "Prints the ARINC 665 Media Set located in the given directory\n"
        << options;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm);

    std::cout << std::endl << "Load Media Set " << std::endl;
    auto mediaSet{ loadMediaSet( directory)};

    std::cout << std::endl << "Print Media Set " << std::endl;
    printMediaSet( mediaSet);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description" << std::endl;
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cout << "std exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


static Arinc665::Media::MediaSetPtr loadMediaSet(
  const std::filesystem::path &mediaSetDirectory)
{
  auto importer{ Arinc665::Utils::Arinc665Utils::arinc665Importer(
    // read file handler
    [&mediaSetDirectory](
      const uint8_t /*mediumNumber*/,
      const std::filesystem::path &path)->Arinc665::File::RawFile
    {
      auto filePath( mediaSetDirectory / path.relative_path());

      if (!std::filesystem::is_regular_file( filePath))
      {
        BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
          << boost::errinfo_file_name( filePath.string())
          << Helper::AdditionalInfo( "File not found"));
      }

      Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

      std::ifstream file(
        filePath.string().c_str(),
        std::ifstream::binary | std::ifstream::in);

      if ( !file.is_open())
      {
        BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
          << Helper::AdditionalInfo( "Error opening files"));
      }

      // read the data to the buffer
      file.read( (char*) &data.at( 0), data.size());

      // return the buffer
      return data;
    })};

  auto mediaSet{ importer()};

  std::cout
    << "media set pn: " << mediaSet->partNumber() << "\n";

  std::cout
    << "no of media set members: "
    << std::dec << (int)mediaSet->numberOfMedia() << "\n";

  return mediaSet;
}

static void printMediaSet( Arinc665::Media::MediaSetPtr &mediaSet)
{
  std::cout << "Media Set " << mediaSet->partNumber() << std::endl;

  // iterate over files
  std::cout << " * Files " << "\n";

  for ( auto const &file : mediaSet->files())
  {
    std::cout << "   * File " << file->name() << "\n";
  }

  // iterate over loads
  std::cout << " * Loads " << "\n";

  for ( auto const &load : mediaSet->loads())
  {
    std::cout << "   * Load " << load->name() << " " << load->partNumber()
      << std::endl;

    std::cout << "     Compatible THW IDs" << std::endl;
    // iterate over THW ID list
    for ( auto const & thwId : load->targetHardwareIds())
    {
      std::cout << "      * " << thwId << std::endl;
    }

    std::cout << "     Data Files" << std::endl;
    // iterate over Data Files
    for ( const auto & dataFile : load->dataFiles())
    {
      std::cout << "      * " << dataFile.lock()->name() << " "
        << dataFile.lock()->partNumber() << std::endl;
    }

    std::cout << "     Support Files" << std::endl;
    // iterate over Data Files
    for ( const auto & supportFile : load->supportFiles())
    {
      std::cout << "      * " << supportFile.lock()->name() << " "
        << supportFile.lock()->partNumber() << std::endl;
    }
  }
}

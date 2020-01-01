/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set Validator.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set Validator Application.
 **/

#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>

/**
* @brief Program entry point
*
* @param[in] argc
*   Number of arguments.
* @param[in] argv
*   The arguments
*
* @return The success state of this operation.
**/
int main( int argc, char * argv[]);

/**
 * @brief Reads the give file and returns the data.
 *
 * @param[in] mediumNumber
 *   Medium number
 * @param[in] path
 *   Path of file on medium.
 *
 * @return THe read file data.
 *
 * @throw Arinc665Exception
 *   If file does not exist or cannot be read.
 **/
static Arinc665::File::RawFile readFile(
  uint8_t mediumNumber,
  const std::filesystem::path &path);

static void printInformation( std::string_view information);

//! Media Directories
static std::vector< std::filesystem::path> mediaDirectories;


/**
 * tasks:
 * # check integrity of media set
 * # check integrity of all files
 * # check integrity of all loads
 * # check integrity of all batches
 *
 * # verbose options print file-contents
 **/
int main( int argc, char * argv[])
{
  boost::program_options::options_description optionsDescription(
    "ARINC 665 Media Set Validator Options");

  optionsDescription.add_options()
    (
      "help",
      "print this help screen"
    )
    (
      "medium-directory",
      boost::program_options::value(
        &mediaDirectories)->required()->composing(),
      "ARINC 665 medium source directory.\n"
      "For more media, repeat this parameter."
    );

  Helper::initLogging( Helper::Severity::info);

  try
  {
    std::cout << "ARINC 665 Media Set Validator" << std::endl;

    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription),
      options);

    if ( options.count( "help") != 0)
    {
      std::cout << optionsDescription << std::endl;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( options);

    // create importer
    auto validator{
      Arinc665::Utils::Arinc665Utils::arinc665Validator(
        std::bind( &readFile, std::placeholders::_1, std::placeholders::_2),
        std::bind( &printInformation, std::placeholders::_1))};

    // perform validation
    auto result{ validator()};

    if (!result)
    {
      std::cerr << "Validation FAILED" << std::endl;
      return EXIT_FAILURE;
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cerr << e.what() <<
      "\nEnter " << argv[0] << " --help to get help" << std::endl;
    return EXIT_FAILURE;
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< Helper::AdditionalInfo>( e);

    std::cerr
      << "Validation failed: "
      // << typid( e).name() << " - "
      << ((nullptr == info) ? "Unknown" : *info) << std::endl;

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Error in validation: " << boost::diagnostic_information( e)
              << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in validation: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Validation Successfully completed" << std::endl;
  return EXIT_SUCCESS;
}

static Arinc665::File::RawFile readFile(
  const uint8_t mediumNumber,
  const std::filesystem::path &path)
{
  // check medium number
  if (mediumNumber > mediaDirectories.size())
  {
    return {};
  }

  auto filePath{ mediaDirectories[ mediumNumber-1] / path.relative_path()};

  // check existence of file
  if (!std::filesystem::is_regular_file( filePath))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
        << boost::errinfo_file_name( filePath.string())
        << Helper::AdditionalInfo( "File not found"));
  }

  Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

  // load file
  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in};

  if ( !file.is_open())
  {
    BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "Error opening files"));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

static void printInformation( std::string_view information)
{
  std::cout << "Validation: " << information << std::endl;
}

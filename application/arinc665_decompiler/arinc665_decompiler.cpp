/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief ARINC 665 Decompiler.
 **/

#include <arinc665/file/File.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/exception/all.hpp>

#include <filesystem>
#include <vector>
#include <cstdlib>
#include <memory>
#include <fstream>
#include <iostream>

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
 * @throw Arind665Exception
 *   If file does not exist or cannot be read.
 **/
static Arinc665::File::RawFile readFile(
  uint8_t mediumNumber,
  const std::filesystem::path &path);

static std::vector< std::filesystem::path> mediaSourceDirectories;
static std::filesystem::path mediaSetXmlFile;

int main( int argc, char * argv[])
{
  boost::program_options::options_description optionsDescription(
    "ARINC 665 Media Set Decompiler Options");

  optionsDescription.add_options()
  (
    "help",
    "print this help screen"
  )
  (
    "source-directory",
    boost::program_options::value(
      &mediaSourceDirectories)->required()->composing(),
    "ARINC 665 media source directories"
  )
  (
    "xml-file",
    boost::program_options::value( &mediaSetXmlFile)->required(),
    "Output ARINC 665 media set description XML"
  );

  initLogging( severity_level::info);

  try
  {
    std::cout << "ARINC 665 Media Set Decompiler" << std::endl;

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
    auto importer{
      Arinc665::Utils::Arinc665Utils::arinc665Importer(
        std::bind( &readFile, std::placeholders::_1, std::placeholders::_2))};

    // perform import
    auto result{ importer()};

    Arinc665::Utils::Arinc665Xml::FilePathMapping fileMapping;

    // iterate over files
    for ( auto file : result->files())
    {
      std::filesystem::path filePath(
        mediaSourceDirectories[ file->medium()->mediumNumber() - 1]
          / file->path().relative_path());

      fileMapping.insert( { file, filePath });
    }

    // XML exporter
    auto xml{ Arinc665::Utils::Arinc665Xml::instance()};

    // export the XML file
    xml->saveToXml( result, fileMapping, mediaSetXmlFile);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< AdditionalInfo>( e);

    std::cerr << "decompiler failed: " <<
    //      typid( e).name() << " - " <<
      ((nullptr == info) ? "Unknown" : *info) << std::endl;

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Error in decompiler: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in decompiler: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static Arinc665::File::RawFile readFile(
  const uint8_t mediumNumber,
  const std::filesystem::path &path)
{
  // check medium number
  if (mediumNumber > mediaSourceDirectories.size())
  {
    return {};
  }

  auto filePath{ mediaSourceDirectories[ mediumNumber-1] / path.relative_path()};

  // check existence of file
  if (!std::filesystem::is_regular_file( filePath))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() <<
        boost::errinfo_file_name( filePath.string()) <<
        AdditionalInfo( "File not found"));
  }

  Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << AdditionalInfo( "Error opening files"));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

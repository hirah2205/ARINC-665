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

#include <arinc665/file/File.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/RegularFile.hpp>

#include <arinc665/utils/MediaSetImporter.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc665/Arinc665Exception.hpp>

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
  const std::filesystem::path &path );

//! Media source directories
static std::vector< std::filesystem::path> mediaSourceDirectories{};

//! Media Set XML file
static std::filesystem::path mediaSetXmlFile{};

//! Check File Integrity
bool checkFileIntegrity{};

int main( int argc, char const * argv[])
{
  std::cout << "ARINC 665 Media Set Decompiler" << "\n";

  boost::program_options::options_description optionsDescription{
    "ARINC 665 Media Set Decompiler Options" };

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

  Helper::initLogging( Helper::Severity::info);

  try
  {
    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription),
      options );

    if ( options.count( "help" ) != 0 )
    {
      std::cout << optionsDescription << "\n";
      return EXIT_FAILURE;
    }

    boost::program_options::notify( options);

    // create importer
    auto importer{ Arinc665::Utils::MediaSetImporter::create() };

    importer->readFileHandler(
      std::bind( &readFile, std::placeholders::_1, std::placeholders::_2 ) );

    importer->checkFileIntegrity( checkFileIntegrity );

    // perform import
    auto result{ (*importer)()};

    Arinc665::Utils::Arinc665Xml::FilePathMapping fileMapping{};

    // iterate over files
    for ( const auto &file : result->files())
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
    std::cout
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
  }
  catch ( Arinc665::Arinc665Exception &e)
  {
    std::string const * const info =
      boost::get_error_info< Helper::AdditionalInfo>( e);

    std::cerr << "decompiler failed: "
      //<< typid( e).name() << " - "
      << ((nullptr == info) ? "Unknown" : *info) << "\n";

    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr
      << "Error in decompiler: " << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Error in decompiler: UNKNOWN EXCEPTION\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static Arinc665::File::RawFile readFile(
  const uint8_t mediumNumber,
  const std::filesystem::path &path)
{
  // check medium number
  if ( mediumNumber > mediaSourceDirectories.size())
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
      << Helper::AdditionalInfo{ "File not found" } );
  }

  Arinc665::File::RawFile data( std::filesystem::file_size( filePath ) );

  // load file
  std::ifstream file{
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in };

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" } );
  }

  // read the data to the buffer
  file.read(
    (char*) &data.at( 0),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}

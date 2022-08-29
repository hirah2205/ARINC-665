/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 List Application
 *
 * Utility to Scan Directories/ Files for ARINC 665 Content and Decodes them.
 **/

/**
 * @dir
 * @brief ARINC 665 List Application.
 **/

#include <arinc665/Arinc665.hpp>

#include <arinc665/files/FileListFile.hpp>
#include <arinc665/files/LoadListFile.hpp>
#include <arinc665/files/BatchListFile.hpp>
#include <arinc665/files/LoadHeaderFile.hpp>
#include <arinc665/files/BatchFile.hpp>

#include <arinc665/utils/FilePrinter.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/Utils.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <fmt/format.h>

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
int main( int argc, char const * argv[] );

/**
 * @brief Loads the load upload header file and decodes its content.
 *
 * @param[in] lubFile
 *   Load upload header.
 **/
static void printBatchFile( const std::filesystem::path &lubFile );

/**
 * @brief Loads the load upload header file and decodes its content.
 *
 * @param[in] luhFile
 *   Load upload header.
 **/
static void printLoadHeaderFile( const std::filesystem::path &luhFile );

/**
 * @brief Loads the load list file and decodes its content.
 *
 * @param[in] loadsLum
 *   Load list file.
 **/
static void printLoadListFile( const std::filesystem::path &loadsLum );

/**
 * @brief Loads the Batch list file and decodes its content.
 *
 * @param[in] filePath
 *   File Path.
 **/
static void printBatchListFile( const std::filesystem::path &filePath );

/**
 * @brief Loads the file list file and decodes its content.
 *
 * @param[in] filesLum
 *   File list file.
 **/
static void printFileListFile( const std::filesystem::path &filesLum );

/**
 * @brief Iterates over every file and subdirectory and tries to decode its
 *   content.
 *
 * @param[in] loadDir
 *   Directory to decode.
 **/
static void list_files( const std::filesystem::path &loadDir );

static Arinc665::Files::RawFile loadFile( const std::filesystem::path &file );

int main( int argc, char const * argv[] )
{
  std::cout << "ARINC 665 list\n";

  boost::program_options::options_description optionsDescription{
    "ARINC 665 List options" };

  // directory to list
  std::filesystem::path directory{};

  optionsDescription.add_options()
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
      boost::program_options::parse_command_line( argc, argv, optionsDescription ),
      vm);

    if ( vm.count( "help") != 0)
    {
      std::cout
        << "Prints the ARINC 665 Media File information located in the given directory\n"
        << optionsDescription;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm );

    std::cout << "List files in " << directory << "\n";

    list_files( directory );
  }
  catch ( const boost::program_options::error &e )
  {
    std::cout
      << "Error parsing command line: " << e.what() << "\n"
      << "Enter " << argv[0] << " --help for command line description\n";
    return EXIT_FAILURE;
  }
  catch ( const boost::exception &e )
  {
    std::cout << "Error: " << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cout << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred" << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void printBatchFile( const std::filesystem::path &lubFile )
{
  try
  {
    Arinc665::Files::BatchFile batch{ loadFile( lubFile ) };

    std::cout << "part number: "<< batch.partNumber() << "\n";
    std::cout << "comment: "<< batch.comment() << "\n";

    for ( auto const &targetHardware : batch.targetsHardware())
    {
      std::cout
        << "target HW: " << targetHardware.targetHardwareIdPosition << "\n";
      for ( auto const &load : targetHardware.loads )
      {
        std::cout
          << "  load: " << load.headerFilename
          << " - " << load.partNumber << "\n";
      }
    }
  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information(e) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cout << "std exception: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred\n";
  }
}

static void printLoadHeaderFile( const std::filesystem::path &luhFile)
{
  try
  {
    const auto rawLoadHeaderFile{ loadFile( luhFile ) };

    Arinc665::Files::LoadHeaderFile load{ rawLoadHeaderFile };

    Arinc665::Utils::FilePrinter_print(
      load,
      std::cout,
      "\t" );

    std::cout
      << fmt::format(
        "\tLoad CRC 0x{:02X}\n",
        Arinc665::Files::LoadHeaderFile::decodeLoadCrc( rawLoadHeaderFile ) );

    if ( load.loadCheckValueType() != Arinc645::CheckValueType::NotUsed )
    {
      std::cout
        << fmt::format(
          "\tLoad Check Value {}\n",
          Arinc645::Utils_toString(
            Arinc665::Files::LoadHeaderFile::decodeLoadCheckValue(
                 rawLoadHeaderFile ) ) );
    }
  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information(e) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cout << "std exception: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred\n";
  }
}

static void printLoadListFile( const std::filesystem::path &loadsLum )
{
  try
  {
    Arinc665::Utils::FilePrinter_print(
      Arinc665::Files::LoadListFile{loadFile( loadsLum ) },
      std::cout,
      "\t" );
  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cout << "std exception: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred\n";
  }
}

static void printBatchListFile( const std::filesystem::path &filePath )
{
  try
  {
    Arinc665::Utils::FilePrinter_print(
      Arinc665::Files::BatchListFile{ loadFile( filePath ) },
      std::cout,
      "\t" );
  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cout << "std exception: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred\n";
  }
}

static void printFileListFile( const std::filesystem::path &filesLum )
{
  try
  {
    Arinc665::Utils::FilePrinter_print(
      Arinc665::Files::FileListFile{ loadFile( filesLum ) },
      std::cout,
      "\t" );
  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cout << "std::exception: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cout << "unknown exception occurred\n";
  }
}

static void list_files( const std::filesystem::path &loadDir )
{
  for ( std::filesystem::directory_iterator itr( loadDir );
    itr != std::filesystem::directory_iterator(); ++itr )
  {
    std::cout << " * " << itr->path().filename() << " - ";

    if ( std::filesystem::is_directory( itr->path() ) )
    {
      std::cout << "Directory\n";
      list_files( itr->path());
    }
    else if ( is_regular_file( itr->status()))
    {
      switch ( Arinc665::Files::Arinc665File::fileType( itr->path().filename()))
      {
        case Arinc665::FileType::BatchFile:
          std::cout << "ARINC 665 BATCH file\n";
          printBatchFile( itr->path() );
          break;

        case Arinc665::FileType::LoadUploadHeader:
          std::cout << "ARINC 665 LOAD UPLOAD HEADER file\n";
          printLoadHeaderFile( itr->path() );
          break;

        case Arinc665::FileType::LoadList:
          std::cout << "ARINC 665 LOAD LIST file\n";
          printLoadListFile( itr->path() );
          break;

        case Arinc665::FileType::BatchList:
          std::cout << "ARINC 665 BATCH LIST file\n";
          printBatchListFile( itr->path() );
          break;

        case Arinc665::FileType::FileList:
          std::cout << "ARINC 665 FILE LIST file\n";
          printFileListFile( itr->path() );
          break;

        default:
          std::cout << "No special ARINC 665 file\n";
          break;
      }
    }
  }
}

static Arinc665::Files::RawFile loadFile( const std::filesystem::path &file )
{
  std::cout
    << fmt::format( "\tFile size is: {}\n", std::filesystem::file_size( file ) );

  Arinc665::Files::RawFile data( std::filesystem::file_size( file ) );

  std::ifstream fileStream{
    file,
    std::ifstream::binary | std::ifstream::in };

  if ( !fileStream.is_open() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception{}
        << Helper::AdditionalInfo{ "Error opening file" }
        << boost::errinfo_file_name{ file.string() } );
  }

  fileStream.read( (char*) &data.at( 0), static_cast< std::streamsize >( data.size() ) );

  return data;
}

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
static void list_lub( const std::filesystem::path &lubFile );

/**
 * @brief Loads the load upload header file and decodes its content.
 *
 * @param[in] luhFile
 *   Load upload header.
 **/
static void list_luh( const std::filesystem::path &luhFile );

/**
 * @brief Loads the load list file and decodes its content.
 *
 * @param[in] loadsLum
 *   Load list file.
 **/
static void list_loads_lum( const std::filesystem::path &loadsLum );

/**
 * @brief Loads the file list file and decodes its content.
 *
 * @param[in] filesLum
 *   File list file.
 **/
static void list_files_lum( const std::filesystem::path &filesLum );

/**
 * @brief Iterates over every file and subdirectory and tries to decode its
 *   content.
 *
 * @param[in] loadDir
 *   Directory to decode.
 **/
static void list_files( const std::filesystem::path &loadDir );

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

static void list_lub( const std::filesystem::path &lubFile )
{
  try
  {
    std::cout
      << fmt::format( "File size is: {}\n", std::filesystem::file_size( lubFile ) );

    std::vector< uint8_t> data( std::filesystem::file_size( lubFile ) );

    std::ifstream file{
      lubFile.string().c_str(),
      std::ifstream::binary | std::ifstream::in };

    if ( !file.is_open() )
    {
      std::cout << "Error opening file: " << lubFile.string() << "\n";
      return;
    }

    file.read( (char*)&data.at(0), static_cast< std::streamsize>( data.size() ) );

    Arinc665::Files::BatchFile batch( data);

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

static void list_luh( const std::filesystem::path &luhFile)
{
  try
  {
    std::cout
      << fmt::format( "File size is: {}\n", std::filesystem::file_size( luhFile ) );

    std::vector< uint8_t> data( std::filesystem::file_size( luhFile) );

    std::ifstream file{
      luhFile.string().c_str(),
      std::ifstream::binary | std::ifstream::in };

    if ( !file.is_open() )
    {
      std::cout << "Error opening file: " << luhFile.string() << "\n";
      return;
    }

    file.read( (char*)&data.at(0), static_cast< std::streamsize>( data.size() ) );

    Arinc665::Files::LoadHeaderFile load{ data };

    std::cout << "part number: "<< load.partNumber() << "\n";

    for ( auto const &targetHardwareId : load.targetHardwareIds() )
    {
      std::cout << "target HW id: " << targetHardwareId << "\n";
    }

    for ( auto const &dataFile : load.dataFiles() )
    {
      std::cout
        << "data file name: " << dataFile.filename << "\n"
        << "data file PN:   " << dataFile.partNumber << "\n"
        << "data file size: " << std::dec << dataFile.length << "\n"
        << "data file CRC:  " << std::hex << dataFile.crc << "\n\n";
    }

    for ( auto const &supportFile : load.supportFiles() )
    {
      std::cout
        << "support file name: " << supportFile.filename << "\n"
        << "support file PN:   " << supportFile.partNumber << "\n"
        << "support file size: " << std::dec << supportFile.length << "\n"
        << "support file CRC:  " << std::hex << supportFile.crc << "\n\n";
    }

    std::cout << "load crc " << std::hex << load.loadCrc() << "\n";
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

static void list_loads_lum( const std::filesystem::path &loadsLum )
{
  try
  {
    std::cout
      << fmt::format( "File size is: {}\n", std::filesystem::file_size( loadsLum ) );

    std::vector< uint8_t> data( std::filesystem::file_size( loadsLum ) );

    std::ifstream file(
      loadsLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in );

    if ( !file.is_open() )
    {
      std::cout << "Error opening file: " << loadsLum.string() << "\n";
      return;
    }

    file.read( (char*)&data.at(0), static_cast< std::streamsize>( data.size() ) );

    Arinc665::Files::LoadListFile loadList( data);

    std::cout
      << "media set pn: " << loadList.mediaSetPn() << "\n";

    std::cout
      << "media seq no: "
      << std::dec << (int)loadList.mediaSequenceNumber() << "\n";

    std::cout
      << "no of media set members: "
      << (int)loadList.numberOfMediaSetMembers() << "\n";

    for ( const auto & load : loadList.loads())
    {
      std::cout
        << "load load pn: "                << load.partNumber << "\n"
        << "load header file name: "       << load.headerFilename << "\n"
        << "load member sequence number: "
          << std::dec << (int)load.memberSequenceNumber << "\n\n";

      for ( const auto & thw : load.targetHardwareIds )
      {
        std::cout << "target hardware id: " << thw << "\n\n";
      }
    }

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

static void list_files_lum( const std::filesystem::path &filesLum )
{
  try
  {
    std::cout
      << fmt::format( "File size is: {}\n", std::filesystem::file_size( filesLum ) );

    std::vector< uint8_t> data( std::filesystem::file_size( filesLum ) );

    std::ifstream fileStream{
      filesLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in };

    if ( !fileStream.is_open() )
    {
      std::cout << "Error opening file: " << filesLum.string() << "\n";
      return;
    }

    fileStream.read( (char*) &data.at( 0), static_cast< std::streamsize >( data.size() ) );

    Arinc665::Files::FileListFile fileList( data );

    std::cout
      << fmt::format(
        "media set pn: {}\n"
        "media seq no: {}\n"
        "no of media set members: {}\n",
        fileList.mediaSetPn(),
        (unsigned int)fileList.mediaSequenceNumber(),
        (unsigned int)fileList.numberOfMediaSetMembers() );

    for ( const auto & file : fileList.files() )
    {
      std::cout << "file file name: " << file.filename << "\n";
      std::cout << "file path name: " << file.pathName << "\n";
      std::cout
        << "file member sequence number: "
        << std::dec << file.memberSequenceNumber << "\n";
      std::cout << "file crc: " << std::hex << file.crc << "\n\n";
    }

  }
  catch ( const boost::exception &e )
  {
    std::cout
      << "Boost exception: " << boost::diagnostic_information(e) << "\n";
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
          list_lub( itr->path());
          break;

        case Arinc665::FileType::LoadUploadHeader:
          std::cout << "ARINC 665 LOAD UPLOAD HEADER file\n";
          list_luh( itr->path());
          break;

        case Arinc665::FileType::LoadList:
          std::cout << "ARINC 665 LOAD LIST file\n";
          list_loads_lum( itr->path());
          break;

        case Arinc665::FileType::BatchList:
          std::cout << "ARINC 665 BATCH LIST file\n";
          break;

        case Arinc665::FileType::FileList:
          std::cout << "ARINC 665 FILE LIST file\n";
          list_files_lum( itr->path());
          break;

        default:
          std::cout << "No special ARINC 665 file\n";
          break;
      }
    }
  }
}

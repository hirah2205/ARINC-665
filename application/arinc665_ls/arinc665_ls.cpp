/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Utility to scan directories/ files for ARINC 665 content ad decodes
 *   them.
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/file/FileListFile.hpp>
#include <arinc665/file/LoadListFile.hpp>
#include <arinc665/file/BatchListFile.hpp>
#include <arinc665/file/LoadHeaderFile.hpp>
#include <arinc665/file/BatchFile.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

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
int main( int argc, char* argv[]);

/**
 * @brief Loads the load upload header file and decodes its content.
 *
 * @param[in] luhFile
 *   The load upload header.
 **/
static void list_lub( const std::filesystem::path &lubFile);

/**
 * @brief Loads the load upload header file and decodes its content.
 *
 * @param[in] luhFile
 *   The load upload header.
 **/
static void list_luh( const std::filesystem::path &luhFile);

/**
 * @brief Loads the load list file and decodes its content.
 *
 * @param[in] loadsLum
 *   The load list file.
 **/
static void list_loads_lum( const std::filesystem::path &loadsLum);

/**
 * @brief Loads the file list file and decodes its content.
 *
 * @param[in] filesLum
 *   The file list file.
 **/
static void list_files_lum( const std::filesystem::path &filesLum);

/**
 * @brief Iterates over every file and sub-directory and tries to decodes its
 *   content.
 *
 * @param[in] loadDir
 *   The directory to decode.
 **/
static void list_files( const std::filesystem::path &loadDir);

int main( int argc, char* argv[])
{
  boost::program_options::options_description options( "ARINC 665 List options");

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
      std::cout <<
        "Prints the ARINC 665 Media File informations located in the given directory\n" <<
        options;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm);

    std::cout << "List files" << std::endl;

    list_files( directory);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << "Error parsing command line: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cout << "Error: " << boost::diagnostic_information( e) << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void list_lub( const std::filesystem::path &lubFile)
{
  try
  {
    std::cout
      << "File size is: "
      << std::dec << std::filesystem::file_size( lubFile) << "\n";

    std::vector< uint8_t> data( std::filesystem::file_size( lubFile));

    std::ifstream file(
      lubFile.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if (!file.is_open())
    {
      std::cout << "Error opening file: " << lubFile.string() << "\n";
      return;
    }

    file.read( (char*)&data.at(0), data.size());

    Arinc665::File::BatchFile batch( data);

    std::cout << "part number: "<< batch.partNumber() << "\n";
    std::cout << "comment: "<< batch.comment() << "\n";

    for ( auto const &targetHardware : batch.targetHardwares())
    {
      std::cout
        << "target HW: " << targetHardware.targetHardwareIdPosition() << "\n";
      for (auto const &load : targetHardware.loads())
      {
        std::cout
          << "  load: " << load.headerFilename() << " - " << load.partNumber() << "\n";
      }
    }
  }
  catch (boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
  }
  catch ( std::exception &e)
  {
    std::cout << "std exception: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "unknown exception occurred" << std::endl;
  }
}

static void list_luh( const std::filesystem::path &luhFile)
{
  try
  {
    std::cout
      << "File size is: "
      << std::dec << std::filesystem::file_size( luhFile) << std::endl;

    std::vector< uint8_t> data( std::filesystem::file_size( luhFile));

    std::ifstream file(
      luhFile.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if (!file.is_open())
    {
      std::cout << "Error opening file: " << luhFile.string() << std::endl;
      return;
    }

    file.read( (char*)&data.at(0), data.size());

    Arinc665::File::LoadHeaderFile load( data);

    std::cout << "part number: "<< load.partNumber() << std::endl;

    for ( auto const &targetHardwareId : load.targetHardwareIds())
    {
      std::cout << "target HW id: " << targetHardwareId << std::endl;
    }

    for ( auto const &dataFile : load.dataFiles())
    {
      std::cout <<
        "data file name: " << dataFile.filename() << "\n" <<
        "data file PN:   " << dataFile.partNumber() << "\n" <<
        "data file size: " << std::dec << dataFile.length() << "\n" <<
        "data file CRC:  " << std::hex << dataFile.crc() << std::endl << std::endl;
    }

    for ( auto const &supportFile : load.supportFiles())
    {
      std::cout <<
        "support file name: " << supportFile.filename() << "\n" <<
        "support file PN:   " << supportFile.partNumber() << "\n" <<
        "support file size: " << std::dec << supportFile.length() << "\n" <<
        "support file CRC:  " << std::hex << supportFile.crc() << std::endl << std::endl;
    }

    std::cout << "load crc " << std::hex << load.loadCrc() << std::endl;
  }
  catch (boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
  }
  catch ( std::exception &e)
  {
    std::cout << "std exception: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "unknown exception occurred" << std::endl;
  }
}

static void list_loads_lum( const std::filesystem::path &loadsLum)
{
  try
  {
    std::cout << "File size is: " << std::dec << std::filesystem::file_size( loadsLum) << std::endl;

    std::vector< uint8_t> data( std::filesystem::file_size( loadsLum));

    std::ifstream file(
      loadsLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if (!file.is_open())
    {
      std::cout << "Error opening file: " << loadsLum.string() << std::endl;
      return;
    }

    file.read( (char*)&data.at(0), data.size());

    Arinc665::File::LoadListFile loadList( data);

    std::cout << "media set pn: " << loadList.mediaSetPn() << std::endl;

    std::cout << "media seq no: " << std::dec << (int)loadList.mediaSequenceNumber() << std::endl;

    std::cout << "no of media set members: " << (int)loadList.numberOfMediaSetMembers() << std::endl;

    for ( const auto & load : loadList.loads())
    {
      std::cout <<
        "load load pn: "                << load.partNumber() << "\n" <<
        "load header file name: "       << load.headerFilename() << "\n" <<
        "load member sequence number: " << std::dec << (int)load.memberSequenceNumber() << std::endl << std::endl;

      for ( const auto & thw : load.targetHardwareIds())
      {
        std::cout << "target hardware id: " << thw << std::endl << std::endl;
      }
    }

  }
  catch (boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
  }
  catch ( std::exception &e)
  {
    std::cout << "std exception: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "unknown exception occurred" << std::endl;
  }
}

static void list_files_lum( const std::filesystem::path &filesLum)
{
  try
  {
    std::cout << "File size is: " << std::dec << std::filesystem::file_size( filesLum) << std::endl;

    std::vector< uint8_t> data( std::filesystem::file_size( filesLum));

    std::ifstream file(
      filesLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if ( !file.is_open())
    {
      std::cout << "Error opening file: " << filesLum.string() << std::endl;
      return;
    }

    file.read( (char*) &data.at( 0), data.size());

    Arinc665::File::FileListFile fileList( data);

    std::cout << "media set pn: " << fileList.mediaSetPn() << std::endl;

    std::cout << "media seq no: " << (int)fileList.mediaSequenceNumber() << std::endl;

    std::cout << "no of media set members: " << std::dec << (int)fileList.numberOfMediaSetMembers() << std::endl;

    for ( const auto & file : fileList.files())
    {
      std::cout << "file file name: " << file.filename() << "\n";
      std::cout << "file path name: " << file.pathName() << "\n";
      std::cout << "file member sequence number: " << std::dec << file.memberSequenceNumber() << "\n";
      std::cout << "file crc: " << std::hex << file.crc() << "\n\n";
    }

  }
  catch (boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information(e) << std::endl;
  }
  catch ( std::exception &e)
  {
    std::cout << "std::exception: " << e.what() << std::endl;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred" << std::endl;
  }
}

static void list_files( const std::filesystem::path &loadDir)
{
  for ( std::filesystem::directory_iterator itr( loadDir);
    itr != std::filesystem::directory_iterator(); ++itr)
  {
    std::cout << " * " << itr->path().filename() << " - ";

    if ( std::filesystem::is_directory( itr->path()))
    {
      std::cout << "Directory\n";
      list_files( itr->path());
    }
    else if ( is_regular_file( itr->status()))
    {
      switch ( Arinc665::File::Arinc665File::fileType( itr->path().filename()))
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


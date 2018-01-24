/*
 * $Date: 2017-05-25 13:41:33 +0200 (Do, 25. Mai 2017) $
 * $Revision: 2068 $
 */
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

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>

void list_luh( const boost::filesystem::path &luhFile)
{
  using Arinc665::File::LoadHeaderFile;
  using Arinc665::File::LoadFileInfo;

  try
  {
    std::cout << "File size is: " << std::dec << boost::filesystem::file_size( luhFile) << std::endl;

    std::vector< uint8_t> data( boost::filesystem::file_size( luhFile));

    std::ifstream file(
      luhFile.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if (!file.is_open())
    {
      std::cout << "Error opening file: " << luhFile.string() << std::endl;
      return;
    }

    file.read( (char*)&data.at(0), data.size());

    LoadHeaderFile load( data);

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

    std::cout << "header file crc " << std::hex << load.crc() << std::endl << std::endl;
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

void list_loads_lum( const boost::filesystem::path &loadsLum)
{
  using Arinc665::File::LoadListFile;
  using Arinc665::File::LoadInfo;

  try
  {
    std::cout << "File size is: " << std::dec << boost::filesystem::file_size( loadsLum) << std::endl;

    std::vector< uint8_t> data( boost::filesystem::file_size( loadsLum));

    std::ifstream file(
      loadsLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if (!file.is_open())
    {
      std::cout << "Error opening file: " << loadsLum.string() << std::endl;
      return;
    }

    file.read( (char*)&data.at(0), data.size());

    LoadListFile loadList( data);

    std::cout << "media set pn: " << loadList.mediaSetPn() << std::endl;

    std::cout << "media seq no: " << std::dec << (int)loadList.mediaSequenceNumber() << std::endl;

    std::cout << "no of media set members: " << (int)loadList.numberOfMediaSetMembers() << std::endl;

    for ( const auto & load : loadList.loadsInfo())
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

    std::cout << "load list crc: " << std::hex << (int)loadList.crc() << std::endl;
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

void list_files_lum( const boost::filesystem::path &filesLum)
{
  using Arinc665::File::FileListFile;
  using Arinc665::File::FileInfo;

  try
  {
    std::cout << "File size is: " << std::dec << boost::filesystem::file_size( filesLum) << std::endl;

    std::vector< uint8_t> data( boost::filesystem::file_size( filesLum));

    std::ifstream file(
      filesLum.string().c_str(),
      std::ifstream::binary | std::ifstream::in);

    if ( !file.is_open())
    {
      std::cout << "Error opening file: " << filesLum.string() << std::endl;
      return;
    }

    file.read( (char*) &data.at( 0), data.size());

    FileListFile fileList( data);

    std::cout << "media set pn: " << fileList.mediaSetPn() << std::endl;

    std::cout << "media seq no: " << (int)fileList.mediaSequenceNumber() << std::endl;

    std::cout << "no of media set members: " << std::dec << (int)fileList.numberOfMediaSetMembers() << std::endl;

    for ( const auto & file : fileList.filesInfo())
    {
      std::cout << "file file name: " << file.filename() << "\n";
      std::cout << "file path name: " << file.pathName() << "\n";
      std::cout << "file member sequence number: " << std::dec << file.memberSequenceNumber() << "\n";
      std::cout << "file crc: " << std::hex << file.crc() << "\n\n";
    }

    std::cout << "file list crc: " << std::hex << (int)fileList.crc() << std::endl;

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

void list_file( const boost::filesystem::path &filename)
{
  std::cout << filename << "\n";
}

static void list_files( const boost::filesystem::path &loadDir)
{
  std::cout << "directory: " << loadDir << std::endl;

  for ( boost::filesystem::directory_iterator itr( loadDir);
    itr != boost::filesystem::directory_iterator(); ++itr)
  {
    std::cout << itr->path().filename() << std::endl;

    if ( is_regular_file( itr->status()))
    {
      std::cout << " - ";

      switch ( Arinc665::File::Arinc665File::getFileType( itr->path().filename()))
      {
        case Arinc665::FileType::BatchFile:
          std::cout << "ARINC 665 BATCH file";
          break;

        case Arinc665::FileType::LoadUploadHeader:
          std::cout << "ARINC 665 LOAD UPLOAD HEADER file";
          break;

        case Arinc665::FileType::LoadList:
          std::cout << "ARINC 665 LOAD LIST file";
          break;

        case Arinc665::FileType::BatchList:
          std::cout << "ARINC 665 BATCH LIST file";
          break;

        case Arinc665::FileType::FileList:
          std::cout << "ARINC 665 FILE LIST file";
          break;

        default:
          std::cout << "No special ARINC 665 file";
          break;
      }

      std::cout << std::endl;

      switch ( Arinc665::File::Arinc665File::getFileType( itr->path().filename()))
      {
        case Arinc665::FileType::BatchFile:
          break;

        case Arinc665::FileType::LoadUploadHeader:
          list_file( itr->path());
          list_luh( itr->path());
          break;

        case Arinc665::FileType::LoadList:
          list_loads_lum( itr->path());
          break;

        case Arinc665::FileType::BatchList:
          break;

        case Arinc665::FileType::FileList:
          list_files_lum( itr->path());
          break;

        default:
          list_file( itr->path());
          break;
      }
    }
  }
}

int main( int argc, char* argv[])
{
  if ( argc != 2)
  {
    std::cout << "enter load directory" << std::endl;

    return EXIT_FAILURE;
  }

  try
  {
    boost::filesystem::path loadDir( argv[1]);

    std::cout << "List files" << std::endl;
    list_files( loadDir);
  }
  catch ( boost::exception &e)
  {
    std::cout << "Boost exception: " << boost::diagnostic_information( e)
      << std::endl;
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


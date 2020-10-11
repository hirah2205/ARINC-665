/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set XML Printer Application.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set XML Printer Application.
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <arinc665/utils/Utils.hpp>
#include <arinc665/utils/Arinc665Xml.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

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

static void listXml( std::filesystem::path &xmlPath );

static void listLoad( Arinc665::Media::LoadPtr load );

static void listBatch( Arinc665::Media::BatchPtr batch );

int main( int argc, char* argv[])
{
  std::cout << "ARINC 665 Media Set XML Printer" << "\n";

  boost::program_options::options_description options{
    "ARINC 665 List options"};

  std::filesystem::path xmlPath;

  options.add_options()
  (
    "help",
    "Print Help"
  )
  (
    "xml",
    boost::program_options::value( &xmlPath)->required(),
    "XML File"
  );

  try
  {
    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::parse_command_line( argc, argv, options),
      vm);

    if ( vm.count( "help") != 0)
    {
      std::cout
        << "Prints the ARINC 665 Media Set XML.\n"
        << options;
      return EXIT_FAILURE;
    }

    boost::program_options::notify( vm);

    std::cout << "List XML" << "\n";

    listXml( xmlPath);
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << "Error parsing command line: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cout << "Error: " << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cout << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cout << "unknown exception occurred\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void listXml( std::filesystem::path &xmlPath)
{
  // ARINC 665 XML instance
  auto xml{ Arinc665::Utils::Arinc665Xml::instance()};

  // load XML file
  auto result{ xml->loadFromXml( xmlPath)};

  auto mediaSet{ std::get< 0>( result)};

  std::cout << "Media Set"
    << "  PN: " << mediaSet->partNumber() << "\n"
    << "  # Media: " << mediaSet->numberOfMedia() << "\n";

  for ( auto const &[mediumNumber, medium] : mediaSet->media())
  {
    std::cout << "Medium: #" << (unsigned int)mediumNumber << "\n";

    for ( const auto &file : medium->files( true))
    {
      std::cout << file->path() << "/" << "\n";

      switch ( file->fileType())
      {
        case Arinc665::Media::BaseFile::FileType::RegularFile:
          break;

        case Arinc665::Media::BaseFile::FileType::LoadFile:
          listLoad( std::dynamic_pointer_cast< Arinc665::Media::Load>( file));
          break;

        case Arinc665::Media::BaseFile::FileType::BatchFile:
          listBatch( std::dynamic_pointer_cast< Arinc665::Media::Batch>( file));
          break;

        default:
          break;
      }
    }
  }
}

static void listLoad( Arinc665::Media::LoadPtr load)
{
  std::cout
    << "Load: " << "\n" << "PN: " << load->partNumber() << "\n";

  for ( const auto &[thwId, positions] : load->targetHardwareIdPositions())
  {
    std::cout << "THW ID: " << thwId << "\n";

    for (const auto &position : positions)
    {
      std::cout << "  Position: " << position << "\n";
    }
  }

  const auto type{ load->loadType()};

  if (type)
  {
    std::cout
      << "Load Type: '" << type->first
      << "' 0x" << std::hex << type->second << std::dec << "\n";
  }
}

static void listBatch( Arinc665::Media::BatchPtr batch)
{
  std::cout
    << "Batch: " << "\n"
    << "PN: " << batch->partNumber() << "\n";

  for ( const auto &[target, loads] : batch->targets())
  {
    std::cout << "Target: " << target << "\n";

    for ( const auto &load : loads)
    {
      std::cout << "  Load: " << load.lock()->path() << "\n";
    }
  }

  std::cout << "Comment: '" << batch->comment() << "'\n";
}

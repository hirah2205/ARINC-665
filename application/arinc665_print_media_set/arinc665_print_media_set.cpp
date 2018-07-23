/*
 * $Date: 2017-05-25 10:46:35 +0200 (Do, 25. Mai 2017) $
 * $Revision: 2064 $
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
 * @brief TODO Enter Brief description
 **/

#include <arinc665/Arinc665.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Utils.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/format.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

using Arinc665::Media::MediaSetPtr;

static MediaSetPtr loadMediaSet( const std::filesystem::path &mediaSetDirectory);

static void printMediaSet( MediaSetPtr &mediaSet);

int main( int argc, char* argv[])
{
  if ( argc != 2)
  {
    std::cout << "enter load directory" << std::endl;

    return EXIT_FAILURE;
  }

  try
  {
    std::filesystem::path loadDir{ argv[1]};

    std::cout << std::endl << "Load Media Set " << std::endl;
    MediaSetPtr mediaSet( loadMediaSet( loadDir));

    std::cout << std::endl << "Print Media Set " << std::endl;
    printMediaSet( mediaSet);
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


static MediaSetPtr loadMediaSet( const std::filesystem::path &mediaSetDirectory)
{
  auto importer( Arinc665::Utils::Arinc665Utils::createArinc665Importer(
    // read file handler
    [&mediaSetDirectory](
      const uint8_t /*mediumNumber*/,
      const std::filesystem::path &path)->Arinc665::File::RawFile
    {
      auto filePath( mediaSetDirectory / path.relative_path());

      if (!std::filesystem::is_regular_file( filePath))
      {
        BOOST_THROW_EXCEPTION(
          Arinc665::Arinc665Exception() <<
            boost::errinfo_file_name( filePath.string()) <<
            AdditionalInfo( "File not found"));
      }

      Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

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
    }));

  auto mediaSet{ importer()};

  std::cout << "media set pn: " << mediaSet->partNumber() << std::endl;

  std::cout << "no of media set members: " << std::dec << (int)mediaSet->numberOfMedia() << std::endl;

  return mediaSet;
}

static void printMediaSet( MediaSetPtr &mediaSet)
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

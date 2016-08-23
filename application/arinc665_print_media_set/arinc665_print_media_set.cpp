/*
 * $Date$
 * $Revision$
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
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>

using Arinc665::Media::MediaSetPtr;

using boost::filesystem::path;

static MediaSetPtr loadMediaSet( const boost::filesystem::path &mediaSetDirectory);

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
    boost::filesystem::path loadDir( argv[1]);

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


static MediaSetPtr loadMediaSet( const path &mediaSetDirectory)
{
  Arinc665::Utils::Arinc665Utils::Arinc665Importer importer(
    Arinc665::Utils::Arinc665Utils::createArinc665Importer(
      [&mediaSetDirectory]( uint8_t)
      {
        return mediaSetDirectory;
      }));

  MediaSetPtr mediaSet( importer());

  std::cout << "media set pn: " << mediaSet->getPartNumber() << std::endl;

  std::cout << "no of media set members: " << std::dec << (int)mediaSet->getNumberOfMedia() << std::endl;

  return mediaSet;
}

static void printMediaSet( MediaSetPtr &mediaSet)
{
  std::cout << "Media Set " << mediaSet->getPartNumber() << std::endl;

  // iterate over files
  for ( auto const &file : mediaSet->getFiles())
  {
    std::cout << "File " << file->getName() << "\n";
  }

  // iterate over loads
  for ( auto const &load : mediaSet->getLoads())
  {
    std::cout << "Load " << load->getName() << " " << load->getPartNumber()
      << std::endl;

    std::cout << "Compatible THW IDs" << std::endl;
    // iterate over THW ID list
    for ( auto const & thwId : load->getTargetHardwareIdList())
    {
      std::cout << " * " << thwId << std::endl;
    }

    std::cout << "Data Files" << std::endl;
    // iterate over Data Files
    for ( const auto & dataFile : load->getDataFiles())
    {
      std::cout << " * " << dataFile.lock()->getName() << " "
        << dataFile.lock()->getPartNumber() << std::endl;
    }

    std::cout << "Support Files" << std::endl;
    // iterate over Data Files
    for ( const auto & supportFile : load->getSupportFiles())
    {
      std::cout << " * " << supportFile.lock()->getName() << " "
        << supportFile.lock()->getPartNumber() << std::endl;
    }
  }
}

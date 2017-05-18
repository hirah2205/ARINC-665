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
 * @brief .
 **/

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetConfiguration.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

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
int main( int argc, char ** argv);

int main( int argc, char ** argv)
{
  initLogging( severity_level::info);

  boost::program_options::options_description optionsDescription;
  boost::filesystem::path mediaSetConfig;

  optionsDescription.add_options()
  (
    "media-set-config",
     boost::program_options::value( &mediaSetConfig)->required(),
     "Media Set configuration"
  );

  try
  {
    boost::program_options::variables_map options;
    boost::program_options::store(
      boost::program_options::parse_command_line(
        argc,
        argv,
        optionsDescription),
      options);
    boost::program_options::notify( options);

    boost::property_tree::ptree config;
    boost::property_tree::json_parser::read_json(
      mediaSetConfig.string(),
      config);

    auto mediaSetManager( Arinc665::Utils::MediaSetManager::createInstance(
      Arinc665::Utils::MediaSetConfiguration( config)));

    for ( auto medium : mediaSetManager->getMediaSets())
    {
      std::cout << medium->getName() << " " << medium->getPartNumber() << "\n";

      std::cout << "  Files:\n";
      // iterate over files
      for ( auto file : medium->getFiles())
      {
        std::cout << "    " << file->getName() << " " << file->getPartNumber() << " " << mediaSetManager->getFilePath( file) << "\n";
      }

      std::cout << "  Loads:\n";
      // iterate over loads
      for ( auto load : medium->getLoads())
      {
        std::cout << "    " << load->getName() << " " << load->getPartNumber() << " " << mediaSetManager->getFilePath( load) << "\n";

        std::cout << "    Data Files:\n";
        for ( auto dataFile : load->getDataFiles())
        {
          std::cout << "      " << dataFile.lock()->getName() << " " << dataFile.lock()->getPartNumber() << " " << mediaSetManager->getFilePath( dataFile.lock()) << "\n";
        }

        std::cout << "    Support Files:\n";
        for ( auto supportFile : load->getSupportFiles())
        {
          std::cout << "      " << supportFile.lock()->getName() << " " << supportFile.lock()->getPartNumber() << " " << mediaSetManager->getFilePath( supportFile.lock()) << "\n";
        }
      }

      std::cout << "  Batches:\n";
      for ( auto batch : medium->getBatches())
      {
        std::cout << "    " << batch->getName() << " " << batch->getPartNumber() << " " << mediaSetManager->getFilePath( batch) << "\n";
      }
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << std::endl << optionsDescription << std::endl;
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr << "Operation failed: " << boost::diagnostic_information( e)
      << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cerr << "Operation failed: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

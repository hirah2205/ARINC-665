/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARIN 665 Media Set Manager Application.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set Manager Application.
 **/

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetConfiguration.hpp>

#include <helper/Logger.hpp>

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <filesystem>
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
  Helper::initLogging( Helper::Severity::info);

  boost::program_options::options_description optionsDescription;
  std::filesystem::path mediaSetConfig;

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

    auto configDir{
      mediaSetConfig.has_parent_path() ?
        std::filesystem::absolute( mediaSetConfig.parent_path() ) :
        std::filesystem::current_path() };
    Arinc665::Utils::MediaSetConfiguration mediaSetConfiguration{ config };

    auto mediaSetManager{ Arinc665::Utils::MediaSetManager::instance(
      configDir,
      mediaSetConfiguration ) };

    for ( const auto &mediaSet : mediaSetManager->mediaSets())
    {
      std::cout << "Media Set: P/N '" << mediaSet->partNumber() << "'\n";

      std::cout << "  Files:\n";
      // iterate over files
      for ( const auto &file : mediaSet->files())
      {
        std::cout
          << "    Name: '" << file->name()
          << mediaSetManager->filePath( file) << "\n";
      }

      std::cout << "  Loads:\n";
      // iterate over loads
      for ( const auto &load : mediaSet->loads())
      {
        std::cout
          << "    Name: '" << load->name() << "' "
          << "P/N: '" << load->partNumber() << "' "
          << "Path: '" << mediaSetManager->filePath( load) << "'\n";

        std::cout << "    Data Files:\n";
        for ( const auto &dataFile : load->dataFiles())
        {
          std::cout
            << "      "
            << "Name: '" << dataFile.first.lock()->name() << "' "
            << "P/N: '" << dataFile.second << "' "
            << "Path: '" << mediaSetManager->filePath( dataFile.first.lock()) << "'\n";
        }

        std::cout << "    Support Files:\n";
        for ( const auto &supportFile : load->supportFiles())
        {
          std::cout
            << "      "
            << "Name: '" << supportFile.first.lock()->name() << "' "
            << "P/N: '" << supportFile.second << "' "
            << "Path: '" << mediaSetManager->filePath( supportFile.first.lock()) << "'\n";
        }
      }

      std::cout << "  Batches:\n";
      for ( const auto &batch : mediaSet->batches())
      {
        std::cout
          << "    "
          << "Name: '" << batch->name() << "' "
          << "P/N: '" << batch->partNumber() << "' "
          << "Path: '" << mediaSetManager->filePath( batch) << "'\n";
        std::cout << "      Comment: '" << batch->comment() << "'\n";
        for ( const auto &targetHardware : batch->targets())
        {
          std::cout << "      THW: '" << targetHardware.first << "'\n";

          for ( const auto &load : targetHardware.second)
          {
            auto load2{ load.lock()};
            if (load2)
            {
              std::cout
                << "        "
                << "Load Name: '" << load2->name() << "' "
                << "Load P/N: '" << load2->partNumber() << "'\n";
            }
            else
            {
              std::cout << "        " << "invalid load\n";
            }
          }
        }
      }
    }
  }
  catch ( boost::program_options::error &e)
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
    return EXIT_FAILURE;
  }
  catch ( boost::exception &e)
  {
    std::cerr
      << "Operation failed: " << boost::diagnostic_information( e) << "\n";
    return EXIT_FAILURE;
  }
  catch ( std::exception &e)
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

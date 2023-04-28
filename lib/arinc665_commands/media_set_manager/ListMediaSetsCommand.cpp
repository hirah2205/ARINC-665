// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class
 *   Arinc665Commands::MediaSetManager::ListMediaSetsCommand.
 **/

#include "ListMediaSetsCommand.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetPrinter.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

#include <fmt/format.h>

namespace Arinc665Commands::MediaSetManager {

ListMediaSetsCommand::ListMediaSetsCommand() :
  optionsDescription{ "List ARINC 665 Media Sets Options" }
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager Directory"
  )
  (
    "check-media-set-manager-integrity",
    boost::program_options::value( &checkMediaSetManagerIntegrityV )
      ->default_value( true ),
    "Check Media Set Manager Integrity"
  );
}

void ListMediaSetsCommand::execute( const Commands::Parameters &parameters )
{
  BOOST_LOG_FUNCTION()

  try
  {
    std::cout << "List ARINC 665 Media Sets \n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    // Media Set Manager
    const auto mediaSetManager{
      Arinc665::Utils::MediaSetManager::load(
        mediaSetManagerDirectory,
        checkMediaSetManagerIntegrityV,
        std::bind_front( &ListMediaSetsCommand::loadProgress, this ) ) };

    const auto mediaSets{ mediaSetManager->mediaSets() };

    if ( mediaSets.empty() )
    {
      std::cout << "*** No media sets within media set manger ***\n";
    }
    else
    {
      for ( const auto &[partNumber, mediaSet] : mediaSetManager->mediaSets() )
      {
        std::cout << "Media Set:\n";

        Arinc665::Utils::MediaSetPrinter_print(
          *mediaSet.first,
          std::cout,
          "  ",
          "  " );

        std::cout << "\n";
      }
    }
  }
  catch ( const boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Error: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cerr << "Error: UNKNOWN EXCEPTION\n";
  }
}

void ListMediaSetsCommand::help()
{
  std::cout
    << "List all Media Sets registered with the Media Set Manager\n"
    << optionsDescription;
}

void ListMediaSetsCommand::loadProgress(
  std::pair< std::size_t, std::size_t > mediaSet,
  std::string_view partNumber,
  std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium )
{
  std::cout << fmt::format(
    "{}/{} {} {}:{}\n",
    mediaSet.first,
    mediaSet.second,
    partNumber,
    static_cast< std::string >( medium.first ),
    static_cast< std::string >( medium.second ) );
}

}

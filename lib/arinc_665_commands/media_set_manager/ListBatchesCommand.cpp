// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::MediaSetManager::ListBatchesCommand.
 **/

#include "ListBatchesCommand.hpp"

#include <arinc_665/media/MediaSet.hpp>
#include <arinc_665/media/Batch.hpp>

#include <arinc_665/utils/MediaSetManager.hpp>

#include <helper/HexString.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>

#include <iostream>
#include <format>

namespace Arinc665Commands::MediaSetManager {

ListBatchesCommand::ListBatchesCommand() :
  optionsDescription{ "List ARINC 665 Batches Options" }
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir,d",
    boost::program_options::value( &mediaSetManagerDirectoryV )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager directory"
  )
  (
    "check-media-set-manager-integrity,i",
    boost::program_options::value( &checkMediaSetManagerIntegrityV )
      ->default_value( true ),
    "Check Media Set Manager integrity during initialisation."
  );
}

void ListBatchesCommand::execute( const Commands::Parameters &parameters )
{
  BOOST_LOG_FUNCTION()

  try
  {
    std::cout << "List ARINC 665 Batches\n";

    boost::program_options::variables_map variablesMap;
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters ).options( optionsDescription ).run(),
      variablesMap );
    boost::program_options::notify( variablesMap );

    // Media Set Manager
    const auto mediaSetManager{
      Arinc665::Utils::MediaSetManager::load(
        mediaSetManagerDirectoryV,
        checkMediaSetManagerIntegrityV,
        std::bind_front( &ListBatchesCommand::loadProgress, this ) ) };

    const auto batches{ mediaSetManager->batches() };

    if ( batches.empty() )
    {
      std::cout << "*** No batches within media set manger ***\n";
    }
    else
    {
      for ( const auto &batch : batches )
      {
        std::cout << std::format(
          "Media Set P/N:  {}\n"
          "Batch Filename: {}\n"
          "Batch P/N:      {}\n"
          "Batch Comment:  {}\n\n",
          batch->mediaSet()->partNumber(),
          batch->name(),
          batch->partNumber(),
          batch->comment() );
      }
    }
  }
  catch ( const boost::program_options::error & )
  {
    // parsing errors are handled by command handler
    throw;
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Operation failed: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( const std::exception &e )
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
  }
}

void ListBatchesCommand::help()
{
  std::cout
    << "List all batches contained with the Media Set Manager.\n\n"
    << optionsDescription;
}

void ListBatchesCommand::loadProgress(
  std::pair< std::size_t, std::size_t > mediaSet,
  std::string_view partNumber,
  std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium )
{
  std::cout << std::format(
    "Loading: {}/{} {} {}:{}\n",
    mediaSet.first,
    mediaSet.second,
    partNumber,
    static_cast< std::string >( medium.first ),
    static_cast< std::string >( medium.second ) );
}

}

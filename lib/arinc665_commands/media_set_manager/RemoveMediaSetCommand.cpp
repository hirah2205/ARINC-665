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
 *   Arinc665Commands::MediaSetManager::RemoveMediaSetCommand.
 **/

#include "RemoveMediaSetCommand.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/FilesystemMediaSetRemover.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands::MediaSetManager {

RemoveMediaSetCommand::RemoveMediaSetCommand() :
  optionsDescription{ "Remove ARINC 665 Media Set Options" }
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
    "media-set-part-number",
    boost::program_options::value( &mediaSetPartNumber )
      ->required()
      ->composing(),
    "ARINC 665 Media Set Part Number"
  );
}

void RemoveMediaSetCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "Remove ARINC 665 Media Set\n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    // Media Set Manager
    const auto mediaSetManager{
      Arinc665::Utils::MediaSetManager::load( mediaSetManagerDirectory ) };

    auto mediaSet{
      mediaSetManager->mediaSet( mediaSetPartNumber ) };

    if ( !mediaSet )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set does not exist" } );
    }

    auto mediaSetPaths{
      mediaSetManager->deregisterMediaSet( mediaSetPartNumber ) };
    mediaSetManager->saveConfiguration();

    auto remover{ Arinc665::Utils::FilesystemMediaSetRemover::create() };
    assert( remover );

    mediaSetPaths.first = mediaSetManagerDirectory / mediaSetPaths.first;
    remover->mediaSetPaths( mediaSetPaths );
    ( *remover )();
  }
  catch ( boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
  }
  catch ( boost::exception &e )
  {
    std::cerr
      << "Operation failed: " << boost::diagnostic_information( e ) << "\n";
  }
  catch ( std::exception &e )
  {
    std::cerr << "Operation failed: " << e.what() << "\n";
  }
  catch ( ... )
  {
    std::cerr << "Operation failed: UNKNOWN EXCEPTION\n";
  }
}

void RemoveMediaSetCommand::help()
{
  std::cout
    << "Remove Media Set from the Media Set Manager\n"
    << optionsDescription;
}

}

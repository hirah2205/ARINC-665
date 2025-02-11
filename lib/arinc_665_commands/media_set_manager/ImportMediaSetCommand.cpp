// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::MediaSetManager::ImportMediaSetCommand.
 **/

#include "ImportMediaSetCommand.hpp"

#include <arinc_665/media/MediaSet.hpp>

#include <arinc_665/files/MediaSetInformation.hpp>

#include <arinc_665/utils/MediaSetManager.hpp>
#include <arinc_665/utils/FilesystemMediaSetDecompiler.hpp>
#include <arinc_665/utils/FilesystemMediaSetCopier.hpp>

#include <arinc_665/Arinc665Exception.hpp>

#include <helper/Exception.hpp>
#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>

#include <iostream>
#include <format>

namespace Arinc665Commands::MediaSetManager {

ImportMediaSetCommand::ImportMediaSetCommand() :
  optionsDescription{ "Import ARINC 665 Media Set Options" }
{
  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager directory."
  )
  (
    "check-media-set-manager-integrity",
    boost::program_options::value( &checkMediaSetManagerIntegrityV )
      ->default_value( true ),
    "Check Media Set Manager integrity during initialisation."
  )
  (
    "source-directory",
    boost::program_options::value( &mediaSourceDirectories )
      ->required()
      ->composing(),
    "ARINC 665 media source directories.\n"
      "Must be provided for each media directory."
  )
  (
    "check-file-integrity",
    boost::program_options::value( &checkFileIntegrity ),
    "Check File integrity during media set decompilation and registration."
  );
}

void ImportMediaSetCommand::execute( const Commands::Parameters &parameters )
{
  BOOST_LOG_FUNCTION()

  try
  {
    std::cout << "Import ARINC 665 Media Set\n";

    boost::program_options::variables_map variablesMap{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters ).options( optionsDescription ).run(),
      variablesMap );
    boost::program_options::notify( variablesMap );

    // Media Set Manager
    const auto mediaSetManager{ Arinc665::Utils::MediaSetManager::load(
      mediaSetManagerDirectory,
      checkMediaSetManagerIntegrityV,
      std::bind_front( &ImportMediaSetCommand::loadProgress, this ) ) };

    // Fill Media Paths list
    Arinc665::Utils::MediaPaths sourceMediaPaths{};
    for ( const auto &mediumSourceDirectory : mediaSourceDirectories )
    {
      const auto mediumInformation{ Arinc665::Utils::getMediumInformation( mediumSourceDirectory ) };

      if ( !mediumInformation )
      {
        BOOST_THROW_EXCEPTION( boost::program_options::invalid_option_value( mediumSourceDirectory ) );
      }

      sourceMediaPaths.try_emplace( mediumInformation->mediaSequenceNumber, mediumSourceDirectory );
    }

    auto importer{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };
    assert( importer );

    const auto &defaults{ mediaSetManager->configuration().defaults };

    importer
      ->checkFileIntegrity( checkFileIntegrity.value_or( defaults.checkFileIntegrity ) )
      .mediaPaths( sourceMediaPaths );

    const auto &[ mediaSet, checkValues]{ ( *importer )() };

    if ( mediaSetManager->hasMediaSet( mediaSet->partNumber() ) )
    {
      BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception{} << Helper::AdditionalInfo{ "Media Set already exist" } );
    }

    const auto copier{ Arinc665::Utils::FilesystemMediaSetCopier::create() };
    assert( copier );

    copier
      ->mediaPaths( sourceMediaPaths )
      .outputBasePath( mediaSetManagerDirectory )
      .mediaSetName( std::string{ mediaSet->partNumber() } );

    auto destinationPaths{ ( *copier )() };

    mediaSetManager->registerMediaSet(
      { std::move( destinationPaths ) },
      checkFileIntegrity.value_or( defaults.checkFileIntegrity ) );

    mediaSetManager->saveConfiguration();
  }
  catch ( const boost::program_options::error &e )
  {
    std::cerr << e.what() << "\n" << optionsDescription << "\n";
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

void ImportMediaSetCommand::help()
{
  std::cout
    << "Import existing ARINC 665 Media Set into Media Set Manager.\n\n"
    << optionsDescription;
}

void ImportMediaSetCommand::loadProgress(
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

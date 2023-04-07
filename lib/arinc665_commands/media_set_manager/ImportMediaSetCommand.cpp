/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class
 *   Arinc665Commands::MediaSetManager::ImportMediaSetCommand.
 **/

#include "ImportMediaSetCommand.hpp"

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>
#include <arinc665/utils/FilesystemMediaSetCopier.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

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
    "ARINC 665 Media Set Manager Directory"
  )
  (
    "source-directory",
    boost::program_options::value( &mediaSourceDirectories )
      ->required()
      ->composing(),
    "ARINC 665 media source directories"
  )
  (
    "check-file-integrity",
    boost::program_options::value( &checkFileIntegrity )->default_value( true ),
    "Check File Integrity during Import"
  );
}

void ImportMediaSetCommand::execute( const Commands::Parameters &parameters )
{
  BOOST_LOG_FUNCTION()

  try
  {
    std::cout << "Import ARINC 665 Media Set\n";

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
        checkFileIntegrity ) };

    // Fill Media Paths list
    Arinc665::Utils::MediaPaths sourceMediaPaths{};
    for ( const auto &mediumSourceDirectory : mediaSourceDirectories )
    {
      const auto mediumInformation{
        Arinc665::Utils::getMediumInformation( mediumSourceDirectory ) };

      if ( !mediumInformation )
      {
        BOOST_THROW_EXCEPTION(
          boost::program_options::invalid_option_value( mediumSourceDirectory ) );
      }

      sourceMediaPaths.try_emplace(
        mediumInformation->mediaSequenceNumber,
        mediumSourceDirectory );
    }

    auto importer{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };
    assert( importer );

    importer
      ->checkFileIntegrity( checkFileIntegrity )
      .mediaPaths( sourceMediaPaths );

    const auto &[ mediaSet, checkValues]{ ( *importer )() };

    if ( mediaSetManager->hasMediaSet( mediaSet->partNumber() ) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set already exist" } );
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
      checkFileIntegrity );

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
  std::cout << "Import ARINC 665 Media Set\n" << optionsDescription;
}

}

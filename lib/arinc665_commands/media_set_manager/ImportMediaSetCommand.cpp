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
#include <arinc665/utils/FilesystemMediaSetImporter.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <fmt/format.h>

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

    auto importer{ Arinc665::Utils::FilesystemMediaSetImporter::create() };
    assert( importer );

    importer
      ->checkFileIntegrity( checkFileIntegrity )
      .mediaPaths( sourceMediaPaths );

    const auto &[ mediaSet, checkValues]{ ( *importer )() };

    // Add Media Set Part Number to Output Path
    Arinc665::Utils::MediaPaths mediaPaths{};
    for (
      Arinc665::MediumNumber mediumNumber{ 1U };
      mediumNumber <= mediaSet->lastMediumNumber();
      ++mediumNumber )
    {
      mediaPaths.try_emplace(
        mediumNumber,
        fmt::format( "MEDIUM_{:03d}", static_cast< uint8_t >( mediumNumber ) ) );
    }

    std::filesystem::path mediaSetPath{ mediaSet->partNumber() };

    if ( std::filesystem::exists( mediaSetManagerDirectory / mediaSetPath ) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
          << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories(
      mediaSetManagerDirectory / mediaSetPath );

    // iterate over media - copy it to media set manager
    for ( auto const &[ mediumNumber, mediumPath ] : mediaPaths )
    {
      std::filesystem::copy(
        sourceMediaPaths[ mediumNumber ],
        mediaSetManagerDirectory / mediaSetPath / mediumPath,
        std::filesystem::copy_options::recursive );
    }

    mediaSetManager->registerMediaSet(
      { std::move( mediaSetPath ), std::move( mediaPaths ) },
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

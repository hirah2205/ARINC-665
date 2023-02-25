/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::ImportMediaSetCommand.
 **/

#include "ImportMediaSetCommand.hpp"

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>
#include <arinc665/utils/MediaSetImporter.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <fmt/format.h>

#include <iostream>
#include <fstream>

namespace Arinc665Commands {

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
      Arinc665::Utils::JsonMediaSetManager::load(
        mediaSetManagerDirectory,
        checkFileIntegrity ) };

    auto importer{ Arinc665::Utils::MediaSetImporter::create() };
    importer
      ->checkFileIntegrity( checkFileIntegrity )
      .fileSizeHandler(
        std::bind_front( &ImportMediaSetCommand::fileSizeHandler, this ) )
      .readFileHandler(
        std::bind_front( &ImportMediaSetCommand::readFileHandler, this ) );
    assert( importer );

    const auto &[ mediaSet, checkValues]{ ( *importer )() };

    // Add Media Set Part Number to Output Path
    Arinc665::Utils::MediaSetManagerConfiguration::MediaPaths mediaPaths{};
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
        mediaSourceDirectories.at( static_cast< uint8_t >( mediumNumber ) - 1 ),
        mediaSetManagerDirectory / mediaSetPath / mediumPath,
        std::filesystem::copy_options::recursive );
    }

    mediaSetManager->manager()->registerMediaSet(
      { std::move( mediaSetPath ), std::move( mediaPaths ) },
      checkFileIntegrity );
    mediaSetManager->saveConfiguration();
  }
  catch ( const boost::program_options::error &e )
  {
    std::cout << e.what() << "\n" << optionsDescription << "\n";
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

size_t ImportMediaSetCommand::fileSizeHandler(
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path ) const
{
  BOOST_LOG_FUNCTION()

  // check medium number
  if ( static_cast< uint8_t >( mediumNumber ) > mediaSourceDirectories.size() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set not found" } );
  }

  const auto filePath{
    mediaSourceDirectories.at( static_cast< uint8_t >( mediumNumber )- 1 )
      / path.relative_path() };

  // check existence of file
  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << Helper::AdditionalInfo{
        "File not found" } << boost::errinfo_file_name{ filePath.string() } );
  }

  return std::filesystem::file_size( filePath );
}

Arinc665::Files::RawFile ImportMediaSetCommand::readFileHandler(
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path )
{
  BOOST_LOG_FUNCTION()

  // check medium number
  if ( static_cast< uint8_t >( mediumNumber ) > mediaSourceDirectories.size() )
  {
    return {};
  }

  const auto filePath{
    mediaSourceDirectories.at( static_cast< uint8_t >( mediumNumber ) - 1 )
      / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Read file " << filePath;

  // check existence of file
  if ( !std::filesystem::is_regular_file( filePath ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception() << Helper::AdditionalInfo{
        "File not found" } << boost::errinfo_file_name{ filePath.string() } );
  }

  Arinc665::Files::RawFile data( std::filesystem::file_size( filePath ) );

  // load file
  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in );

  if ( !file.is_open() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // read the data to the buffer
  file.read(
    (char *)&data.at( 0 ),
    static_cast< std::streamsize >( data.size() ) );

  // return the buffer
  return data;
}

}

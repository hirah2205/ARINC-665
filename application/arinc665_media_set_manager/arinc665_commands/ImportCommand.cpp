/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Commands::ImportCommand.
 **/

#include "ImportCommand.hpp"

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>
#include <arinc665/utils/MediaSetImporter.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>

#include <boost/exception/all.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <fstream>

namespace Arinc665Commands {

ImportCommand::ImportCommand() : optionsDescription{ "Import Media Set" }
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

void ImportCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "Import XML\n";

    boost::program_options::variables_map vm{};
    boost::program_options::store(
      boost::program_options::command_line_parser( parameters )
        .options( optionsDescription )
        .run(),
      vm );
    boost::program_options::notify( vm );

    // Media Set Manager
    const auto mediaSetManager{
      Arinc665::Utils::JsonMediaSetManager::load( mediaSetManagerDirectory ) };

    auto importer{ Arinc665::Utils::MediaSetImporter::create() };
    importer->checkFileIntegrity( checkFileIntegrity )
      .readFileHandler( std::bind(
        &ImportCommand::readFileHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2 ) );

    auto mediaSet{ ( *importer )() };

    // Add Media Set Part Number to Output Path
    Arinc665::Utils::MediaSetManagerConfiguration::MediaPaths mediaPaths{};
    for ( const auto &medium : mediaSet->media() )
    {
      mediaPaths.emplace(
        medium.first,
        ( boost::format( "MEDIUM_%03u" ) % (unsigned int)medium.first ).str() );
    }
    Arinc665::Utils::MediaSetManagerConfiguration::MediaSetPaths mediaSetPaths{
      std::make_pair( mediaSet->partNumber(), std::move( mediaPaths ) ) };

    if ( std::filesystem::exists(
           mediaSetManagerDirectory / mediaSetPaths.first ) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories(
      mediaSetManagerDirectory / mediaSetPaths.first );

    // iterate over media
    for ( auto const &[mediumNumber, mediumPath] : mediaSetPaths.second )
    {
      const auto sourcePath{ mediaSourceDirectories.at( mediumNumber - 1 ) };
      const auto destinationMediumPath{ mediaSetManagerDirectory / mediumPath };

      std::filesystem::copy(
        sourcePath,
        destinationMediumPath,
        std::filesystem::copy_options::recursive );
    }

    mediaSetManager->manager()->registerMediaSet( mediaSetPaths );
    mediaSetManager->saveConfiguration();
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

void ImportCommand::help()
{
  std::cout << "Import Media Set\n" << optionsDescription;
}


Arinc665::Files::RawFile ImportCommand::readFileHandler(
  uint8_t mediumNumber,
  const std::filesystem::path &path )
{
  BOOST_LOG_FUNCTION()

  // check medium number
  if ( mediumNumber > mediaSourceDirectories.size() )
  {
    return {};
  }

  const auto filePath{
    mediaSourceDirectories.at( mediumNumber - 1 ) / path.relative_path() };

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

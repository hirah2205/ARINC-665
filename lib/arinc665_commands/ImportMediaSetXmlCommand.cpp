/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Commands::ImportMediaSetXmlCommand.
 **/

#include "ImportMediaSetXmlCommand.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>
#include <arinc665/utils/MediaSetExporter.hpp>

#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <fmt/format.h>

#include <iostream>
#include <fstream>

namespace Arinc665Commands {

ImportMediaSetXmlCommand::ImportMediaSetXmlCommand() :
  optionsDescription{ "Import ARINC 665 Media Set XML Options" }
{
  const auto &fileCreatPolDes{
    Arinc665::Utils::FileCreationPolicyDescription::instance() };

  const std::string fileCreationPolicyValues{
    "* '"
    + std::string{ fileCreatPolDes.name(
      Arinc665::Utils::FileCreationPolicy::None ) }
    + "': Create never\n" + "* '"
    + std::string{ fileCreatPolDes.name(
      Arinc665::Utils::FileCreationPolicy::NoneExisting ) }
    + "': Create none-existing\n" + "* '"
    + std::string{ fileCreatPolDes.name(
      Arinc665::Utils::FileCreationPolicy::All ) }
    + "': Create all" };

  const auto &versionDes{
    Arinc665::SupportedArinc665VersionDescription::instance() };

  const std::string versionValues{
    "* '"
    + std::string{ versionDes.name(
      Arinc665::SupportedArinc665Version::Supplement2 ) }
    + "': ARINC 665-2\n" + "* '"
    + std::string{ versionDes.name(
      Arinc665::SupportedArinc665Version::Supplement345 ) }
    + "': ARINC 665-3/4/5" };

  optionsDescription.add_options()
  (
    "media-set-manager-dir",
    boost::program_options::value( &mediaSetManagerDirectory )
      ->required()
      ->value_name( "Directory" ),
    "ARINC 665 Media Set Manager Directory"
  )
  (
    "xml-file",
    boost::program_options::value( &mediaSetXmlFile )->required(),
    "ARINC 665 media set description file"
  )
  (
    "source-directory",
    boost::program_options::value( &mediaSetSourceDirectory )->required(),
    "ARINC 665 source directory"
  )
  (
    "create-batch-files",
    boost::program_options::value( &createBatchFiles )
      ->default_value( Arinc665::Utils::FileCreationPolicy::None ),
    ( std::string( "batch-files creation policy:\n" )
      + fileCreationPolicyValues )
      .c_str() )(
    "create-load-header-files",
    boost::program_options::value( &createLoadHeaderFiles )
      ->default_value( Arinc665::Utils::FileCreationPolicy::None ),
    ( std::string( "Load-headers-files creation policy:\n" )
      + fileCreationPolicyValues )
      .c_str()
  )
  (
    "version",
    boost::program_options::value( &version )
      ->default_value( Arinc665::SupportedArinc665Version::Supplement2 ),
    ( std::string( "ARINC 665 Version:\n" ) + versionValues ).c_str()
  );
}

void ImportMediaSetXmlCommand::execute( const Commands::Parameters &parameters )
{
  try
  {
    std::cout << "Import ARINC 665 Media Set XML\n";

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

    // load ARINC 665 XML file
    loadXmlResult = Arinc665::Utils::Arinc665Xml_load( mediaSetXmlFile );

    // Add Media Set Part Number to Output Path
    Arinc665::Utils::MediaSetManagerConfiguration::MediaPaths mediaPaths{};
    for (
      Arinc665::MediumNumber mediumNumber{ 1U };
      mediumNumber <= std::get< 0 >( loadXmlResult )->lastMediumNumber();
      ++mediumNumber )
    {
      mediaPaths.try_emplace(
        mediumNumber,
        fmt::format( "MEDIUM_{:03d}", static_cast< uint8_t >( mediumNumber ) ) );
    }
    mediaSetPaths = std::make_pair(
      std::get< 0 >( loadXmlResult )->partNumber(),
      std::move( mediaPaths ) );

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

    auto exporter{ Arinc665::Utils::MediaSetExporter::create() };

    // set exporter parameters
    exporter->mediaSet( std::get< 0 >( loadXmlResult ) )
      .createMediumHandler( std::bind_front(
        &ImportMediaSetXmlCommand::createMediumHandler,
        this ) )
      .createDirectoryHandler( std::bind_front(
        &ImportMediaSetXmlCommand::createDirectoryHandler,
        this ) )
      .checkFileExistenceHandler( std::bind_front(
        &ImportMediaSetXmlCommand::checkFileExistenceHandler,
        this ) )
      .createFileHandler( std::bind_front(
        &ImportMediaSetXmlCommand::createFileHandler,
        this ) )
      .writeFileHandler( std::bind_front(
        &ImportMediaSetXmlCommand::writeFileHandler,
        this ) )
      .readFileHandler( std::bind_front(
        &ImportMediaSetXmlCommand::readFileHandler,
        this ) )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles );

    ( *exporter )();

    mediaSetManager->manager()->registerMediaSet( mediaSetPaths );
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

void ImportMediaSetXmlCommand::help()
{
  std::cout
    << "Compiles Media Set given by XML description and registers it to the Media Set Manager\n"
    << optionsDescription;
}

void ImportMediaSetXmlCommand::createMediumHandler(
  const Arinc665::MediumNumber &mediumNumber )
{
  BOOST_LOG_FUNCTION()

  auto mPath{
    mediaSetManagerDirectory / mediaSetPaths.first
    / mediaSetPaths.second.at( mediumNumber ) };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create medium directory " << mPath;

  std::filesystem::create_directory( mPath );
}

void ImportMediaSetXmlCommand::createDirectoryHandler(
  const Arinc665::MediumNumber &mediumNumber,
  const Arinc665::Media::ConstDirectoryPtr &directory )
{
  BOOST_LOG_FUNCTION()

  auto directoryPath{
    mediaSetManagerDirectory / mediaSetPaths.first
    / mediaSetPaths.second.at( mediumNumber )
    / directory->path().relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "Create directory " << directoryPath;

  std::filesystem::create_directory( directoryPath );
}

bool ImportMediaSetXmlCommand::checkFileExistenceHandler(
  const Arinc665::Media::ConstFilePtr &file )
{
  BOOST_LOG_FUNCTION()

  BOOST_LOG_TRIVIAL( severity_level::trace )
    << "check existence of " << file->path();

  // search for file
  auto fileIt{ std::get< 1 >( loadXmlResult ).find( file ) };

  if ( fileIt == std::get< 1 >( loadXmlResult ).end() )
  {
    return false;
  }

  return std::filesystem::is_regular_file(
    mediaSetSourceDirectory / fileIt->second );
}

void ImportMediaSetXmlCommand::createFileHandler(
  const Arinc665::Media::ConstFilePtr &file )
{
  BOOST_LOG_FUNCTION()

  // search for file
  auto fileIt{ std::get< 1 >( loadXmlResult ).find( file ) };

  if ( fileIt == std::get< 1 >( loadXmlResult ).end() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "file mapping not found" }
      << boost::errinfo_file_name( std::string{ file->name() } ) );
  }

  auto filePath{
    mediaSetManagerDirectory / mediaSetPaths.first
    / mediaSetPaths.second.at( file->effectiveMediumNumber() )
    / file->path().relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Copy file " << filePath;

  // copy file
  std::filesystem::copy( mediaSetSourceDirectory / fileIt->second, filePath );
}

void ImportMediaSetXmlCommand::writeFileHandler(
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path,
  const Arinc665::Files::ConstRawFileSpan &file )
{
  BOOST_LOG_FUNCTION()

  const auto filePath{
    mediaSetManagerDirectory / mediaSetPaths.first
    / mediaSetPaths.second.at( mediumNumber )
    / path.relative_path() };

  BOOST_LOG_TRIVIAL( severity_level::trace ) << "Write file " << filePath;

  // check existence of file
  if ( std::filesystem::exists( filePath ) )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "File already exists" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // save file
  std::ofstream fileStream{
    filePath.string(),
    std::ofstream::binary | std::ofstream::out | std::ofstream::trunc };

  if ( !fileStream.is_open() )
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo{ "Error opening files" }
      << boost::errinfo_file_name{ filePath.string() } );
  }

  // write the data to the buffer
  fileStream.write(
    (char const *)file.data(),
    static_cast< std::streamsize >( file.size() ) );
}

Arinc665::Files::RawFile ImportMediaSetXmlCommand::readFileHandler(
  const Arinc665::MediumNumber &mediumNumber,
  const std::filesystem::path &path )
{
  BOOST_LOG_FUNCTION()

  // check medium number
  const auto filePath{
    mediaSetManagerDirectory / mediaSetPaths.first
    / mediaSetPaths.second.at( mediumNumber )
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

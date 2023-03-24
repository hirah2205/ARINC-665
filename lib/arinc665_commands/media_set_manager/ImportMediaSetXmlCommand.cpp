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
 *   Arinc665Commands::MediaSetManager::ImportMediaSetXmlCommand.
 **/

#include "ImportMediaSetXmlCommand.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>
#include <arinc665/utils/FilesystemMediaSetExporter.hpp>

#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <boost/exception/all.hpp>

#include <iostream>

namespace Arinc665Commands::MediaSetManager {

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
    auto [ mediaSet, filePathMapping ] =
      Arinc665::Utils::Arinc665Xml_load( mediaSetXmlFile );

    std::filesystem::path mediaSetPath{ mediaSet->partNumber() };

    if ( std::filesystem::exists(
      mediaSetManagerDirectory / mediaSetPath ) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories(
      mediaSetManagerDirectory / mediaSetPath );

    auto exporter{ Arinc665::Utils::FilesystemMediaSetExporter::create() };

    // set exporter parameters
    exporter
      ->mediaSet( mediaSet )
      .arinc665Version( version )
      .createBatchFiles( createBatchFiles )
      .createLoadHeaderFiles( createLoadHeaderFiles )
      .mediaSetBasePath( mediaSetManagerDirectory / mediaSetPath )
      .sourceBasePath( mediaSetSourceDirectory )
      .filePathMapping( std::move( filePathMapping ) );

    auto mediaPaths{ ( *exporter )() };

    mediaSetManager->manager()->registerMediaSet(
      { std::move( mediaSetPath ), std::move( mediaPaths ) } );
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

}

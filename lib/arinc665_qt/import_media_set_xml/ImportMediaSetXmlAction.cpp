/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ImportMediaSetXmlAction.hpp"

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXmlWizard.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetExporter.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

ImportMediaSetXmlAction::ImportMediaSetXmlAction(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  wizard{ std::make_unique< ImportMediaSetXmlWizard >( parent ) },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  exporter{ Arinc665::Utils::FilesystemMediaSetExporter::create() }
{
  assert( exporter );

  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::xmlFile,
    this,
    &ImportMediaSetXmlAction::xmlFile );
  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::inputDirectory,
    this,
    &ImportMediaSetXmlAction::inputDirectory );
  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::arinc665Version,
    this,
    &ImportMediaSetXmlAction::arinc665Version );
  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::createBatchFiles,
    this,
    &ImportMediaSetXmlAction::createBatchFiles );
  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::createLoadHeaderFiles,
    this,
    &ImportMediaSetXmlAction::createLoadHeaderFiles );

  wizard->show();

  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::start,
    this,
    &ImportMediaSetXmlAction::start );
  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::finished,
    this,
    &ImportMediaSetXmlAction::finished );
}

ImportMediaSetXmlAction::~ImportMediaSetXmlAction() = default;

void ImportMediaSetXmlAction::xmlFile( std::filesystem::path xmlFile )
{
  xmlFileV = std::move( xmlFile );
}

void ImportMediaSetXmlAction::inputDirectory( std::filesystem::path directory )
{
  exporter->sourceBasePath( std::move( directory ) );
}

void ImportMediaSetXmlAction::arinc665Version(
  Arinc665::SupportedArinc665Version version )
{
  exporter->arinc665Version( version );
}

void ImportMediaSetXmlAction::createBatchFiles(
  Arinc665::Utils::FileCreationPolicy createBatchFiles )
{
  exporter->createBatchFiles( createBatchFiles );
}

void ImportMediaSetXmlAction::createLoadHeaderFiles(
  Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles )
{
  exporter->createLoadHeaderFiles( createLoadHeaderFiles );
}

void ImportMediaSetXmlAction::start()
{
  try
  {
    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( xmlFileV );

    auto outputPath{ mediaSetManagerV->directory() / mediaSet->partNumber() };

    if ( std::filesystem::exists( outputPath ) )
    {
      QMessageBox::critical(
        nullptr,
        tr( "Error during import" ),
        tr( "Media Set directory must not exist" ) );
      return;
    }

    std::filesystem::create_directory( outputPath );

    exporter
      ->mediaSet( mediaSet )
      .filePathMapping( std::move( fileMapping ) )
      .mediaSetBasePath( std::move( outputPath ) );
    assert( exporter );
    auto mediaPaths{ ( *exporter )() };

    mediaSetManagerV->registerMediaSet(
      { mediaSet->partNumber(), std::move( mediaPaths ) },
      true );
    mediaSetManagerV->saveConfiguration();
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during compilation" ),
      QString::fromStdString( info ) );
    return;
  }
}

}

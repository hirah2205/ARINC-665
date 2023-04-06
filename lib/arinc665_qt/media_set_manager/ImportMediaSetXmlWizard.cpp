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
 *   Arinc665Qt::MediaSetManager::ImportMediaSetXmlWizard.
 **/

#include "ImportMediaSetXmlWizard.hpp"

#include "ui_ImportMediaSetXmlWizard.h"

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetCompiler.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt::MediaSetManager {

ImportMediaSetXmlWizard::ImportMediaSetXmlWizard(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlWizard >() },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  compilerV{ Arinc665::Utils::FilesystemMediaSetCompiler::create() }
{
  assert( compilerV );

  ui->setupUi( this );
  ui->settings->setCommitPage( true );

  connect(
    ui->settings,
    &ImportMediaSetXmlSettingsPage::xmlFile,
    this,
    &ImportMediaSetXmlWizard::xmlFile );
  connect(
    ui->settings,
    &ImportMediaSetXmlSettingsPage::inputDirectory,
    this,
    &ImportMediaSetXmlWizard::inputDirectory );
  connect(
    ui->settings,
    &ImportMediaSetXmlSettingsPage::arinc665Version,
    this,
    &ImportMediaSetXmlWizard::arinc665Version );
  connect(
    ui->settings,
    &ImportMediaSetXmlSettingsPage::createBatchFiles,
    this,
    &ImportMediaSetXmlWizard::createBatchFiles );
  connect(
    ui->settings,
    &ImportMediaSetXmlSettingsPage::createLoadHeaderFiles,
    this,
    &ImportMediaSetXmlWizard::createLoadHeaderFiles );

  connect(
    this,
    &QWizard::currentIdChanged,
    this,
    &ImportMediaSetXmlWizard::pageChanged );
}

ImportMediaSetXmlWizard::~ImportMediaSetXmlWizard() = default;

void ImportMediaSetXmlWizard::pageChanged( int id )
{
  if ( ui->settings->nextId() == id )
  {
    importMediaSetXml();
  }
}

void ImportMediaSetXmlWizard::xmlFile( std::filesystem::path xmlFile )
{
  xmlFileV = std::move( xmlFile );
}

void ImportMediaSetXmlWizard::inputDirectory( std::filesystem::path directory )
{
  compilerV->sourceBasePath( std::move( directory ) );
}

void ImportMediaSetXmlWizard::arinc665Version(
  Arinc665::SupportedArinc665Version version )
{
  compilerV->arinc665Version( version );
}

void ImportMediaSetXmlWizard::createBatchFiles(
  Arinc665::Utils::FileCreationPolicy createBatchFiles )
{
  compilerV->createBatchFiles( createBatchFiles );
}

void ImportMediaSetXmlWizard::createLoadHeaderFiles(
  Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles )
{
  compilerV->createLoadHeaderFiles( createLoadHeaderFiles );
}

void ImportMediaSetXmlWizard::importMediaSetXml()
{
  try
  {
    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( xmlFileV );

    compilerV
      ->mediaSet( mediaSet )
      .filePathMapping( std::move( fileMapping ) )
      .outputBasePath( mediaSetManagerV->directory() );
    auto mediaSetPaths{ ( *compilerV )() };

    mediaSetManagerV->registerMediaSet( mediaSetPaths, true );
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

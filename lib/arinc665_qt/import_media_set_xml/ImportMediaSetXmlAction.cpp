/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlAction.
 **/

#include "ImportMediaSetXmlAction.hpp"

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXmlWizard.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetCompiler.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

ImportMediaSetXmlAction::ImportMediaSetXmlAction(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  wizardV{ std::make_unique< ImportMediaSetXmlWizard >( parent ) },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  compilerV{ Arinc665::Utils::FilesystemMediaSetCompiler::create() }
{
  assert( compilerV );

  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::xmlFile,
    this,
    &ImportMediaSetXmlAction::xmlFile );
  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::inputDirectory,
    this,
    &ImportMediaSetXmlAction::inputDirectory );
  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::arinc665Version,
    this,
    &ImportMediaSetXmlAction::arinc665Version );
  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::createBatchFiles,
    this,
    &ImportMediaSetXmlAction::createBatchFiles );
  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::createLoadHeaderFiles,
    this,
    &ImportMediaSetXmlAction::createLoadHeaderFiles );

  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::start,
    this,
    &ImportMediaSetXmlAction::start );
  connect(
    wizardV.get(),
    &ImportMediaSetXmlWizard::finished,
    this,
    &ImportMediaSetXmlAction::finished );

  wizardV->show();
}

ImportMediaSetXmlAction::~ImportMediaSetXmlAction() = default;

void ImportMediaSetXmlAction::xmlFile( std::filesystem::path xmlFile )
{
  xmlFileV = std::move( xmlFile );
}

void ImportMediaSetXmlAction::inputDirectory( std::filesystem::path directory )
{
  compilerV->sourceBasePath( std::move( directory ) );
}

void ImportMediaSetXmlAction::arinc665Version(
  Arinc665::SupportedArinc665Version version )
{
  compilerV->arinc665Version( version );
}

void ImportMediaSetXmlAction::createBatchFiles(
  Arinc665::Utils::FileCreationPolicy createBatchFiles )
{
  compilerV->createBatchFiles( createBatchFiles );
}

void ImportMediaSetXmlAction::createLoadHeaderFiles(
  Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles )
{
  compilerV->createLoadHeaderFiles( createLoadHeaderFiles );
}

void ImportMediaSetXmlAction::start()
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

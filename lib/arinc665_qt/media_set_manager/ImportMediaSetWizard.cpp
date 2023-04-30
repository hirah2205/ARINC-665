// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetManager::ImportMediaSetWizard.
 **/

#include "ImportMediaSetWizard.hpp"

#include "ui_ImportMediaSetWizard.h"

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/utils/FilesystemMediaSetCopier.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt::MediaSetManager {

ImportMediaSetWizard::ImportMediaSetWizard(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetWizard >() },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  copierV{ Arinc665::Utils::FilesystemMediaSetCopier::create() }
{
  assert( copierV );

  ui->setupUi( this );

  // set Logo of Wizard Pages
  QIcon icon{};
  icon.addFile(
    QString::fromUtf8( ":/media_set_manager/arinc665_media_set_import.svg" ),
    QSize{},
    QIcon::Normal,
    QIcon::Off );
  for ( const auto pageId : pageIds() )
  {
    page( pageId )->setPixmap(
      QWizard::WizardPixmap::LogoPixmap,
      icon.pixmap( 64 ) );
  }

  connect(
    this,
    &QWizard::accepted,
    this,
    &ImportMediaSetWizard::importMediaSet );

  connect(
    ui->settings,
    &ImportMediaSetSettingsPage::mediaPathsChanged,
    this,
    &ImportMediaSetWizard::updateMediaPaths );
  connect(
    ui->settings,
    &ImportMediaSetSettingsPage::checkFileIntegrity,
    this,
    &ImportMediaSetWizard::checkFileIntegrity );

  // finally set defaults (signals must be connected)
  ui->settings->defaults( mediaSetManagerV->mediaSetDefaults() );
}

ImportMediaSetWizard::~ImportMediaSetWizard() = default;

void ImportMediaSetWizard::updateMediaPaths(
  const Arinc665::Utils::MediaPaths &mediaPaths )
{
  mediaPathsV = mediaPaths;
}

void ImportMediaSetWizard::checkFileIntegrity( bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
}

void ImportMediaSetWizard::importMediaSet()
{
  BOOST_LOG_FUNCTION()

  try
  {
    const auto mediaInformation{
      Arinc665::Utils::getMediumInformation( mediaPathsV.begin()->second ) };

    if ( !mediaInformation )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Directory contains no medium" } );
    }

    if ( mediaSetManagerV->hasMediaSet( mediaInformation->partNumber) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set already exist" } );
    }

    assert( copierV );
    copierV
      ->mediaPaths( mediaPathsV )
      .outputBasePath( mediaSetManagerV->directory() )
      .mediaSetName( mediaInformation->partNumber );

    auto copyResult{ ( * copierV )() };

    mediaSetManagerV->registerMediaSet( copyResult, checkFileIntegrityV );
    mediaSetManagerV->saveConfiguration();
  }
  catch ( const boost::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString{ tr( "Error:<br/><tt>%1</tt>") }
        .arg( QString::fromStdString( info ) ) );
    return;
  }
  catch ( const std::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString{ tr( "Error:<br/><tt>%1</tt>") }
        .arg( QString::fromStdString( info ) ) );
    return;
  }
}

}

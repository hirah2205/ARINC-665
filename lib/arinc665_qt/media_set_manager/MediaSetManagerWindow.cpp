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
 * @brief Definition of Class
 *   Arinc665Qt::MediaSetManager::MediaSetManagerWindow.
 **/

#include "MediaSetManagerWindow.hpp"

#include "ui_MediaSetManagerWindow.h"

#include <arinc665_qt/media_set_manager/ViewMediaSetDialog.hpp>
#include <arinc665_qt/media_set_manager/RemoveMediaSetAction.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetWizard.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetXmlWizard.hpp>
#include <arinc665_qt/media_set_manager/MediaSetManagerSettingsDialog.hpp>

#include <arinc665_qt/media/MediaSetsModel.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/Version.hpp>

#include <arinc645/Version.hpp>

#include <commands/Version.hpp>

#include <qt_icon_resources/Version.hpp>

#include <helper_qt/AboutDialog.hpp>
#include <helper_qt/String.hpp>

#include <helper/Version.hpp>

#include <QIcon>
#include <QDesktopServices>
#include <QUrl>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerWindow::MediaSetManagerWindow( QWidget * const parent ) :
  QMainWindow{ parent },
  ui{ std::make_unique< Ui::MediaSetManagerWindow >() },
  viewMediaSetDialog{ std::make_unique< ViewMediaSetDialog >( this ) },
  settingsDialog{ std::make_unique< MediaSetManagerSettingsDialog >( this ) },
  aboutDialog{ std::make_unique< HelperQt::AboutDialog >( this ) },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) }
{
  ui->setupUi( this );

  ui->mediaSets->setModel( mediaSetsModelV.get() );

  QIcon icon{};
  icon.addFile(
    QString::fromUtf8( ":/fa/solid/database.svg" ),
    QSize{},
    QIcon::Normal,
    QIcon::Off );
  aboutDialog->productLogo( icon.pixmap( 64 ) );
  aboutDialog->productName( QString{ "%1 (%2)" }.arg(
    tr( "ARINC 665 Media Set Manager" ),
    QString::fromStdString( Arinc665::Version::Name ) ) );
  aboutDialog->productVersion(
    QString::fromStdString( Arinc665::Version::VersionString ) );
  aboutDialog->productLicense(
    QString::fromStdString( Arinc665::Version::License ) );
  aboutDialog->productUrl( QString::fromStdString( Arinc665::Version::Url ) );
  aboutDialog->versions(
    {
      Arinc665::Version::VersionInformation(),
      Arinc645::Version::VersionInformation(),
      Arinc645::Arinc645_botanVersion(),
      QtIconResources::Version::VersionInformation(),
      Commands::Version::VersionInformation(),
      Helper::Version::VersionInformation(),
      HelperQt::HelperQt_qtVersion()
    } );

  connect(
    ui->mediaSets,
    &QTableView::activated,
    this,
    &MediaSetManagerWindow::viewMediaSet );

  connect(
    ui->viewMediaSet,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::viewMediaSet );
  connect(
    ui->importMediaSet,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::importMediaSet );
  connect(
    ui->importMediaSetXml,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::importMediaSetXml );
  connect(
    ui->removeMediaSet,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::removeMediaSet );
  connect(
    ui->openMediaSetsDirectory,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::openMediaSetsDirectory );

  connect(
    ui->mediaSetManagerSettings,
    &QAction::triggered,
    this,
    &MediaSetManagerWindow::showSettings );
  connect(
    settingsDialog.get(),
    &MediaSetManagerSettingsDialog::accepted,
    this,
    &MediaSetManagerWindow::saveSettings );

  connect(
    ui->about,
    &QAction::triggered,
    aboutDialog.get(),
    QOverload<>::of( &HelperQt::AboutDialog::open ) );
}

MediaSetManagerWindow::~MediaSetManagerWindow() = default;

void MediaSetManagerWindow::mediaSetManger(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager )
{
  mediaSetManagerV = std::move( mediaSetManager );

  reloadMediaSetModel();
}

void MediaSetManagerWindow::reloadMediaSetModel()
{
  if ( !mediaSetsModelV )
  {
    return;
  }

  Arinc665::Media::ConstMediaSets mediaSets{};

  if ( mediaSetManagerV )
  {
    // convert media sets to const media sets
    std::ranges::transform(
      mediaSetManagerV->mediaSets(),
      std::back_inserter( mediaSets ),
      []( const auto &mediaSet )
      {
        return mediaSet.second.first;
      } );
  }

  mediaSetsModelV->mediaSets( std::move( mediaSets ) );

  ui->mediaSets->selectRow( 0 );
}

void MediaSetManagerWindow::viewMediaSet()
{
  const auto index{ ui->mediaSets->currentIndex() };

  if ( !index.isValid() )
  {
    return;
  }

  auto mediaSet{
    mediaSetsModelV->constMediaSet( mediaSetsModelV->mediaSet( index ) ) };

  if ( !mediaSet )
  {
    return;
  }

  viewMediaSetDialog->setWindowTitle(
    HelperQt::toQString( mediaSet->partNumber() ) );
  viewMediaSetDialog->mediaSet( std::move( mediaSet ) );
  viewMediaSetDialog->show();
}

void MediaSetManagerWindow::importMediaSet()
{
  if ( !mediaSetsModelV )
  {
    return;
  }

  auto wizard{ new ImportMediaSetWizard{ mediaSetManagerV, this } };

  // connect to reload media set model slot
  connect(
    wizard,
    &ImportMediaSetWizard::finished,
    this,
    &MediaSetManagerWindow::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetWizard::finished,
    wizard,
    &ImportMediaSetWizard::deleteLater );

  wizard->open();
}

void MediaSetManagerWindow::importMediaSetXml()
{
  if ( !mediaSetsModelV )
  {
    return;
  }

  auto wizard{ new ImportMediaSetXmlWizard{ mediaSetManagerV, this } };

  // connect to reload media set model slot
  connect(
    wizard,
    &ImportMediaSetXmlWizard::finished,
    this,
    &MediaSetManagerWindow::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetXmlWizard::finished,
    wizard,
    &ImportMediaSetXmlWizard::deleteLater );

  wizard->open();
}

void MediaSetManagerWindow::removeMediaSet()
{
  const auto index{ ui->mediaSets->currentIndex() };

  if ( !index.isValid() )
  {
    return;
  }

  auto mediaSet{
    mediaSetsModelV->constMediaSet(  mediaSetsModelV->mediaSet( index ) ) };

  if ( !mediaSet )
  {
    return;
  }

  auto action{ new RemoveMediaSetAction{ this } };

  // connect to reload media set model slot
  connect(
    action,
    &RemoveMediaSetAction::finished,
    this,
    &MediaSetManagerWindow::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    action,
    &RemoveMediaSetAction::finished,
    action,
    &RemoveMediaSetAction::deleteLater );

  action->start( mediaSetManagerV, mediaSet );
}

void MediaSetManagerWindow::openMediaSetsDirectory()
{
  if ( !mediaSetManagerV )
  {
    return;
  }

  QDesktopServices::openUrl( QUrl::fromLocalFile(
    QString::fromStdString( mediaSetManagerV->directory().string() ) ) );
}

void MediaSetManagerWindow::showSettings()
{
  settingsDialog->configuration( mediaSetManagerV->mediaSetDefaults() );
  settingsDialog->open();
}

void MediaSetManagerWindow::saveSettings()
{
  mediaSetManagerV->mediaSetDefaults( settingsDialog->configuration() );
  mediaSetManagerV->saveConfiguration();
}

}

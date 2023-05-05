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
 *   Arinc665Qt::MediaSetManager::MediaSetManagerDialog.
 **/

#include "MediaSetManagerDialog.hpp"

#include "ui_MediaSetManagerDialog.h"

#include <arinc665_qt/media_set_manager/ViewMediaSetDialog.hpp>
#include <arinc665_qt/media_set_manager/RemoveMediaSetController.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetWizard.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetXmlWizard.hpp>
#include <arinc665_qt/media_set_manager/MediaSetManagerSettingsDialog.hpp>

#include <arinc665_qt/media/MediaSetsModel.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <helper_qt/String.hpp>

#include <QDesktopServices>
#include <QUrl>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerDialog::MediaSetManagerDialog( QWidget * const parent ) :
  QDialog{ parent },
  ui{ std::make_unique< Ui::MediaSetManagerDialog>() },
  viewMediaSetDialog{ std::make_unique< ViewMediaSetDialog >( this ) },
  settingsDialog{ std::make_unique< MediaSetManagerSettingsDialog >( this ) },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) }
{
  ui->setupUi( this );

  ui->mediaSets->setModel( mediaSetsModelV.get() );

  connect(
    ui->mediaSets,
    &QTableView::activated,
    this,
    &MediaSetManagerDialog::viewMediaSet );

  connect(
    ui->viewMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::viewMediaSet );
  connect(
    ui->importMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::importMediaSet );
  connect(
    ui->importMediaSetXml,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::importMediaSetXml );
  connect(
    ui->removeMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::removeMediaSet );
  connect(
    ui->openMediaSetsDirectory,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::openMediaSetsDirectory );

  connect(
    ui->settings,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::showSettings );
  connect(
    settingsDialog.get(),
    &MediaSetManagerSettingsDialog::accepted,
    this,
    &MediaSetManagerDialog::saveSettings );
}

MediaSetManagerDialog::~MediaSetManagerDialog() = default;

void MediaSetManagerDialog::mediaSetManger(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager )
{
  mediaSetManagerV = std::move( mediaSetManager );

  reloadMediaSetModel();
}

void MediaSetManagerDialog::reloadMediaSetModel()
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

void MediaSetManagerDialog::viewMediaSet()
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

void MediaSetManagerDialog::importMediaSet()
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
    &MediaSetManagerDialog::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetWizard::finished,
    wizard,
    &ImportMediaSetWizard::deleteLater );

  wizard->open();
}

void MediaSetManagerDialog::importMediaSetXml()
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
    &MediaSetManagerDialog::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetXmlWizard::finished,
    wizard,
    &ImportMediaSetXmlWizard::deleteLater );

  wizard->open();
}

void MediaSetManagerDialog::removeMediaSet()
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

  auto controller{ new RemoveMediaSetController{ this } };

  // connect to reload media set model slot
  connect(
    controller,
    &RemoveMediaSetController::finished,
    this,
    &MediaSetManagerDialog::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    controller,
    &RemoveMediaSetController::finished,
    controller,
    &RemoveMediaSetController::deleteLater );

  controller->start( mediaSetManagerV, mediaSet );
}

void MediaSetManagerDialog::openMediaSetsDirectory()
{
  if ( !mediaSetManagerV )
  {
    return;
  }

  QDesktopServices::openUrl( QUrl::fromLocalFile(
    QString::fromStdString( mediaSetManagerV->directory().string() ) ) );
}

void MediaSetManagerDialog::showSettings()
{
  settingsDialog->configuration( mediaSetManagerV->mediaSetDefaults() );
  settingsDialog->open();
}

void MediaSetManagerDialog::saveSettings()
{
  mediaSetManagerV->mediaSetDefaults( settingsDialog->configuration() );
  mediaSetManagerV->saveConfiguration();
}

}

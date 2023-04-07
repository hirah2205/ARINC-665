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

#include <arinc665_qt/media/MediaSetsModel.hpp>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerDialog::MediaSetManagerDialog(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  QDialog{ parent },
  ui{ std::make_unique< Ui::MediaSetManagerDialog>() },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) }
{
  ui->setupUi( this );

  ui->mediaSets->setModel( mediaSetsModelV.get() );

  reloadMediaSetModel();

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
}

MediaSetManagerDialog::~MediaSetManagerDialog() = default;

void MediaSetManagerDialog::reloadMediaSetModel()
{
  if ( !mediaSetsModelV )
  {
    return;
  }

  Arinc665::Media::ConstMediaSets mediaSets{};

  // convert media sets to const media sets
  for ( const auto &[ partNumber, mediaSet ] : mediaSetManagerV->mediaSets() )
  {
    mediaSets.emplace_back( mediaSet.first );
  }

  mediaSetsModelV->mediaSets( std::move( mediaSets ) );
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

  auto viewMediaSetDialog{ new ViewMediaSetDialog{ this } };

  connect(
    viewMediaSetDialog,
    &ViewMediaSetDialog::finished,
    viewMediaSetDialog,
    &ViewMediaSetDialog::deleteLater );

  viewMediaSetDialog->mediaSet( std::move( mediaSet ) );
  emit viewMediaSetDialog->show();
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

  emit wizard->open();
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

  emit wizard->open();
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

  emit controller->start( mediaSetManagerV, mediaSet );
}

}

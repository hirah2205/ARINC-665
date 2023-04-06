/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetManagerController.
 **/

#include "MediaSetManagerController.hpp"

#include <arinc665_qt/media_set_manager/MediaSetManagerDialog.hpp>
#include <arinc665_qt/media_set_manager/RemoveMediaSetController.hpp>
#include <arinc665_qt/media_set_manager/ViewMediaSetDialog.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetWizard.hpp>
#include <arinc665_qt/media_set_manager/ImportMediaSetXmlWizard.hpp>

#include <arinc665_qt/media/MediaSetsModel.hpp>

#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc645/CheckValue.hpp>

#include <boost/property_tree/json_parser.hpp>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerController::MediaSetManagerController(
  QWidget * const parent ) :
  QObject{ parent },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) },
  mediaSetManagerDialogV{ std::make_unique< MediaSetManagerDialog >( parent ) },
  selectMediaSetDirectoryDialogV{ std::make_unique< QFileDialog >( parent ) }
{
  mediaSetManagerDialogV->mediaSetsModel( mediaSetsModelV.get() );
  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::finished,
    this,
    &MediaSetManagerController::finished );

  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::viewMediaSet,
    this,
    &MediaSetManagerController::viewMediaSet );
  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::importMediaSet,
    this,
    &MediaSetManagerController::importMediaSet );
  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::importMediaSetXml,
    this,
    &MediaSetManagerController::importMediaSetXml );
  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::removeMediaSet,
    this,
    &MediaSetManagerController::removeMediaSet );

  selectMediaSetDirectoryDialogV->setWindowTitle(
    tr( "Select ARINC 665 Media Set Manager Configuration" ) );
  selectMediaSetDirectoryDialogV->setFileMode( QFileDialog::Directory );
  selectMediaSetDirectoryDialogV->setOption( QFileDialog::ShowDirsOnly );
  connect(
    selectMediaSetDirectoryDialogV.get(),
    &QFileDialog::rejected,
    this,
    &MediaSetManagerController::finished );
  connect(
    selectMediaSetDirectoryDialogV.get(),
    &QFileDialog::accepted,
    this,
    &MediaSetManagerController::directorySelected );
}

MediaSetManagerController::~MediaSetManagerController() = default;

void MediaSetManagerController::start()
{
  selectMediaSetDirectoryDialogV->open();
}

void MediaSetManagerController::start(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager )
{
  mediaSetManagerV = std::move( mediaSetManager );
  reloadMediaSetModel();
  mediaSetManagerDialogV->open();
}

void MediaSetManagerController::directorySelected()
{
  auto directory{ selectMediaSetDirectoryDialogV->selectedFiles().first() };

  mediaSetManagerV = Arinc665::Utils::MediaSetManager::loadOrCreate(
    directory.toStdString(),
    true );

  reloadMediaSetModel();

  mediaSetManagerDialogV->setWindowTitle( directory );
  mediaSetManagerDialogV->open();
}

void MediaSetManagerController::reloadMediaSetModel()
{
  Arinc665::Media::ConstMediaSets mediaSets{};

  for ( const auto &[ partNumber, mediaSet ] : mediaSetManagerV->mediaSets() )
  {
    mediaSets.emplace_back( mediaSet.first );
  }

  mediaSetsModelV->mediaSets( std::move( mediaSets ) );
}

void MediaSetManagerController::viewMediaSet( const QModelIndex &index )
{
  auto mediaSet{
    mediaSetsModelV->constMediaSet( mediaSetsModelV->mediaSet( index ) ) };

  if ( !mediaSet )
  {
    return;
  }

  auto viewMediaSetDialog{ new ViewMediaSetDialog{ mediaSetManagerDialogV.get() } };

  connect(
    viewMediaSetDialog,
    &ViewMediaSetDialog::finished,
    viewMediaSetDialog,
    &ViewMediaSetDialog::deleteLater );

  viewMediaSetDialog->mediaSet( mediaSet );
  emit viewMediaSetDialog->show();
}

void MediaSetManagerController::importMediaSet()
{
  auto wizard{ new ImportMediaSetWizard{
    mediaSetManagerV,
    mediaSetManagerDialogV.get() } };

  // connect to reload media set model slot
  connect(
    wizard,
    &ImportMediaSetWizard::finished,
    this,
    &MediaSetManagerController::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetWizard::finished,
    wizard,
    &ImportMediaSetWizard::deleteLater );

  emit wizard->open();
}

void MediaSetManagerController::importMediaSetXml()
{
  auto wizard{ new ImportMediaSetXmlWizard{
    mediaSetManagerV,
    mediaSetManagerDialogV.get() } };

  // connect to reload media set model slot
  connect(
    wizard,
    &ImportMediaSetXmlWizard::finished,
    this,
    &MediaSetManagerController::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    wizard,
    &ImportMediaSetXmlWizard::finished,
    wizard,
    &ImportMediaSetXmlWizard::deleteLater );

  emit wizard->open();
}

void MediaSetManagerController::removeMediaSet( const QModelIndex &index )
{
  auto mediaSet{
    mediaSetsModelV->constMediaSet(  mediaSetsModelV->mediaSet( index ) ) };

  if ( !mediaSet )
  {
    return;
  }

  auto controller{ new RemoveMediaSetController{
    mediaSetManagerDialogV.get() } };

  // connect to reload media set model slot
  connect(
    controller,
    &RemoveMediaSetController::finished,
    this,
    &MediaSetManagerController::reloadMediaSetModel );

  // connect to clean up slot
  connect(
    controller,
    &RemoveMediaSetController::finished,
    controller,
    &RemoveMediaSetController::deleteLater );

  emit controller->start( mediaSetManagerV, mediaSet );
}

}

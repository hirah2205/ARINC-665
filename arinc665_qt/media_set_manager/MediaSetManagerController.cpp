/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetController.
 **/

#include "MediaSetManagerController.hpp"

#include <arinc665_qt/media/MediaSetsModel.hpp>
#include <arinc665_qt/media_set_manager/MediaSetManagerDialog.hpp>
#include <arinc665_qt/MediaSetController.hpp>

#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/JsonMediaSetManager.hpp>

#include <boost/property_tree/json_parser.hpp>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerController::MediaSetManagerController(
  QWidget * const parent ) :
  QObject{ parent },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) },
  mediaSetManagerDialogV{ std::make_unique< MediaSetManagerDialog >( parent ) },
  selectMediaSetDirectoryDialogV{ std::make_unique< QFileDialog >(
    parent,
    tr( "Select ARINC 665 Media Set Manager Configuration" ) ) },
  mediaSetManagerV{}
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
  Arinc665::Utils::JsonMediaSetManagerPtr mediaSetManager )
{
  mediaSetManagerV = std::move( mediaSetManager );

  Arinc665::Media::ConstMediaSets mediaSets{};
  for ( auto mediaSet : mediaSetManagerV->manager()->mediaSets() )
  {
    mediaSets.emplace_back( std::move( mediaSet.second ) );
  }
  mediaSetsModelV->mediaSets( std::move( mediaSets ) );
  mediaSetManagerDialogV->open();
}

void MediaSetManagerController::directorySelected()
{
  auto directory{ selectMediaSetDirectoryDialogV->selectedFiles().first() };

  emit start( Arinc665::Utils::JsonMediaSetManager::load(
    directory.toStdString(),
    true )  );
}

void MediaSetManagerController::viewMediaSet( const QModelIndex &index )
{
  auto mediaSet{
    mediaSetsModelV->constMediaSet( mediaSetsModelV->mediaSet( index ) ) };

  if ( !mediaSet )
  {
    return;
  }

  auto controller{ new MediaSetController{ mediaSetManagerDialogV.get() } };

  connect(
    controller,
    &MediaSetController::finished,
    controller,
    &MediaSetController::deleteLater );

  controller->start( mediaSet );
}

void MediaSetManagerController::importMediaSet()
{

}

void MediaSetManagerController::importMediaSetXml()
{

}

void MediaSetManagerController::removeMediaSet( const QModelIndex &index )
{

}

}

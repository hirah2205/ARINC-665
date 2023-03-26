/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetXmlDialog.
 **/

#include "ViewMediaSetXmlDialog.hpp"

#include "ui_ViewMediaSetXmlDialog.h"

#include <arinc665_qt/media/MediaSetModel.hpp>
#include <arinc665_qt/media/LoadsModel.hpp>
#include <arinc665_qt/media/BatchesModel.hpp>

#include <arinc665_qt/FilePathMappingModel.hpp>

#include <arinc665/media/Base.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

namespace Arinc665Qt {

ViewMediaSetXmlDialog::ViewMediaSetXmlDialog( QWidget * const parent ):
  QDialog{ parent },
  ui{ std::make_unique< Ui::ViewMediaSetXmlDialog >() }
{
  ui->setupUi( this );

  connect(
    ui->mediaSetTreeView,
    &QTreeView::activated,
    this,
    &ViewMediaSetXmlDialog::itemSelected );
}

ViewMediaSetXmlDialog::~ViewMediaSetXmlDialog() = default;

void ViewMediaSetXmlDialog::mediaSetModel( Media::MediaSetModel * const model )
{
  if ( mediaSetModelV != nullptr )
  {
    // TODO disconnect old model
  }

  mediaSetModelV = model;

  ui->mediaSetTreeView->setModel( mediaSetModelV );

  ui->mediaSetWidget->mediaSetModel( mediaSetModelV );
  ui->directoryWidget->mediaSetModel( mediaSetModelV );

  if ( nullptr != mediaSetModelV )
  {
    connect(
      mediaSetModelV,
      &Media::MediaSetModel::modelReset,
      [ this ]{
        ui->mediaSetTreeView->setCurrentIndex(
          mediaSetModelV->index( 0, 0 ) );
        ui->mediaSetTreeView->setExpanded(
          mediaSetModelV->index( 0, 0 ),
          true );
        ui->mediaSetTreeView->resizeColumnToContents( 0 );

        // initiate update
        emit( itemSelected( mediaSetModelV->index( 0, 0 ) ) );
      } );
  }
}

void ViewMediaSetXmlDialog::filePathMappingModel(
  FilePathMappingModel * const model )
{
  if ( filePathMappingModelV != nullptr )
  {
    // TODO disconnect old model
  }

  filePathMappingModelV = model;

  ui->filePathMapping->setModel( filePathMappingModelV );
}

void ViewMediaSetXmlDialog::itemSelected( const QModelIndex &index )
{
  auto element{ mediaSetModelV->element( index ) };

  if ( !element )
  {
    return;
  }

  switch ( element->type() )
  {
    case Arinc665::Media::Type::MediaSet:
    {
      const auto mediaSet{
        std::dynamic_pointer_cast< const Arinc665::Media::MediaSet >( element ) };

      ui->detailsStackedWidget->setCurrentIndex( 0 );
      ui->mediaSetWidget->selectedMediaSet( mediaSet );
      break;
    }

    case Arinc665::Media::Type::Directory:
      ui->detailsStackedWidget->setCurrentIndex( 1 );
      ui->directoryWidget->selectedDirectory( index );
      ui->directoryWidget->selectedDirectory(
        std::dynamic_pointer_cast< const Arinc665::Media::Directory >( element ) );
      break;

    case Arinc665::Media::Type::File:
      ui->detailsStackedWidget->setCurrentIndex( 2 );
      ui->fileWidget->selectedFile(
        mediaSetModelV,
        std::dynamic_pointer_cast< const Arinc665::Media::File >( element ) );
      break;

    default:
      break;
  }
}

}

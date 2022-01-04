/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetDialog.
 **/

#include "MediaSetDialog.hpp"

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665/media/Base.hpp>
#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/BaseFile.hpp>

#include "ui_MediaSetDialog.h"

namespace Arinc665Qt {

MediaSetDialog::MediaSetDialog( QWidget * const parent ):
  QDialog{ parent},
  ui{ std::make_unique< Ui::MediaSetDialog>() },
  mediaSetModelV{ nullptr }
{
  ui->setupUi( this );

  connect(
    ui->mediaSetTreeView,
    SIGNAL( activated( const QModelIndex& ) ),
    this,
    SLOT( itemSelected( const QModelIndex& ) ) );
}

MediaSetDialog::~MediaSetDialog() = default;

void MediaSetDialog::model( Media::MediaSetModel * const mediaSetModel )
{
  if ( mediaSetModelV != nullptr )
  {
    // disconnect old model
  }

  mediaSetModelV = mediaSetModel;

  ui->mediaSetTreeView->setModel( mediaSetModel );
}

void MediaSetDialog::itemSelected( const QModelIndex &index)
{
  auto element{ mediaSetModelV->element( index ) };

  if ( !element )
  {
    return;
  }

  switch ( element->type() )
  {
    case Arinc665::Media::Base::Type::MediaSet:
      ui->detailsStackedWidget->setCurrentIndex( 0 );
      ui->mediaSetWidget->selectedMediaSet(
        mediaSetModelV,
        std::dynamic_pointer_cast< Arinc665::Media::MediaSet>( element ) );
      break;

    case Arinc665::Media::Base::Type::Medium:
      ui->detailsStackedWidget->setCurrentIndex( 1 );
      ui->mediumWidget->selectedMedium(
        mediaSetModelV,
        std::dynamic_pointer_cast< Arinc665::Media::Medium>( element ) );
      break;

    case Arinc665::Media::Base::Type::Directory:
      ui->detailsStackedWidget->setCurrentIndex( 2 );
      ui->directoryWidget->selectedDirectory(
        mediaSetModelV,
        std::dynamic_pointer_cast< Arinc665::Media::Directory>( element ) );
      break;

    case Arinc665::Media::Base::Type::File:
      ui->detailsStackedWidget->setCurrentIndex( 3 );
      ui->fileWidget->selectedFile(
        mediaSetModelV,
        std::dynamic_pointer_cast< Arinc665::Media::BaseFile>( element ) );
      break;

    default:
      break;
  }
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::MediaSetWidget.
 **/

#include "MediaSetWidget.hpp"


#include "ui_MediaSetWidget.h"

#include <arinc665_qt/media/LoadsModel.hpp>
#include <arinc665_qt/media/BatchesModel.hpp>
#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665/media/MediaSet.hpp>

namespace Arinc665Qt::Media {

MediaSetWidget::MediaSetWidget( QWidget * const parent ):
  QWidget{ parent },
  ui{ std::make_unique< Ui::MediaSetWidget>() },
  loadsModelV{ std::make_unique< Media::LoadsModel >( this ) },
  batchesModelV{ std::make_unique< Media::BatchesModel >( this ) },
  mediaSetModelV{ nullptr }
{
  ui->setupUi( this );

  ui->loads->setModel( loadsModelV.get() );
  ui->batches->setModel( batchesModelV.get() );
}

MediaSetWidget::~MediaSetWidget() = default;

void MediaSetWidget::mediaSetModel(
  Arinc665Qt::Media::MediaSetModel * const model )
{
  mediaSetModelV = model;
  ui->content->setModel( model );
}

void MediaSetWidget::selectedMediaSet(
  Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  mediaSetV = std::move( mediaSet );

  if ( mediaSetV )
  {
    ui->partNumberLineEdit->setText( QString::fromUtf8(
      mediaSetV->partNumber().data(),
      static_cast< int >( mediaSetV->partNumber().length() ) ) );

    ui->content->setRootIndex( mediaSetModelV->index( 0, 0 ) );
    ui->content->resizeColumnsToContents();

    loadsModelV->loads( mediaSetV->recursiveLoads() );
    ui->loads->resizeColumnsToContents();

    batchesModelV->batches( mediaSetV->recursiveBatches() );
    ui->batches->resizeColumnsToContents();
  }
}

}

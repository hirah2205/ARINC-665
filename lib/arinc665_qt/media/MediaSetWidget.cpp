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
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <helper_qt/String.hpp>

namespace Arinc665Qt::Media {

MediaSetWidget::MediaSetWidget( QWidget * const parent ):
  QWidget{ parent },
  ui{ std::make_unique< Ui::MediaSetWidget>() },
  loadsModelV{ std::make_unique< Media::LoadsModel >( this ) },
  batchesModelV{ std::make_unique< Media::BatchesModel >( this ) }
{
  ui->setupUi( this );

  ui->loads->setModel( loadsModelV.get() );
  ui->batches->setModel( batchesModelV.get() );

  ui->content->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );
  ui->loads->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );
  ui->batches->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );

  connect(
    ui->content,
    &QTableView::activated,
    this,
    &MediaSetWidget::selectElement );
  connect(
    ui->loads,
    &QTableView::activated,
    this,
    &MediaSetWidget::selectLoad );
  connect(
    ui->batches,
    &QTableView::activated,
    this,
    &MediaSetWidget::selectBatch );
}

MediaSetWidget::~MediaSetWidget() = default;

void MediaSetWidget::mediaSetModel(
  Arinc665Qt::Media::MediaSetModel * const model )
{
  mediaSetModelV = model;
  ui->content->setModel( model );
}

void MediaSetWidget::selectMediaSet(
  Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  mediaSetV = std::move( mediaSet );

  if ( mediaSetV )
  {
    ui->partNumberLineEdit->setText(
      HelperQt::toQString( mediaSetV->partNumber() ) );

    ui->defaultMediumNumber->setValue(
      static_cast< uint8_t >( mediaSetV->effectiveDefaultMediumNumber() ) );

    ui->content->setRootIndex( mediaSetModelV->index( 0, 0 ) );

    loadsModelV->loads( mediaSetV->recursiveLoads() );

    batchesModelV->batches( mediaSetV->recursiveBatches() );
  }
}

void MediaSetWidget::selectElement( const QModelIndex &index )
{
  auto element{ mediaSetModelV->element( index ) };

  if ( element )
  {
    emit activatedElement( std::move( element ) );
  }
}

void MediaSetWidget::selectLoad( const QModelIndex &index )
{
  auto load{ loadsModelV->constLoad( loadsModelV->load( index ) ) };

  if ( load )
  {
    emit activatedElement( std::move( load ) );
  }
}

void MediaSetWidget::selectBatch( const QModelIndex &index )
{
  auto batch{ batchesModelV->constBatch( batchesModelV->batch( index ) ) };

  if ( batch )
  {
    emit activatedElement( std::move( batch ) );
  }
}

}

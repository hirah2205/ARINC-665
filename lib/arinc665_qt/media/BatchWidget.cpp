/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::BatchWidget.
 **/

#include "BatchWidget.hpp"

#include "ui_BatchWidget.h"

#include <arinc665_qt/media/BatchInfoModel.hpp>
#include <arinc665_qt/media/LoadsModel.hpp>

#include <arinc665/media/Batch.hpp>

#include <helper_qt/String.hpp>

namespace Arinc665Qt::Media {

BatchWidget::BatchWidget( QWidget * const parent ) :
  QWidget{ parent},
  ui{ std::make_unique< Ui::BatchWidget>() },
  batchInfoModel{ std::make_unique< BatchInfoModel >( this ) },
  targetLoadsModel{ std::make_unique< LoadsModel >( this ) }
{
  ui->setupUi( this );

  ui->targets->setModel( batchInfoModel.get() );
  ui->loads->setModel( targetLoadsModel.get() );
  ui->loads->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );

  connect(
    ui->targets->selectionModel(),
    &QItemSelectionModel::currentChanged,
    this,
    &BatchWidget::selectTarget );
  connect(
    ui->loads,
    &QTableView::activated,
    this,
    &BatchWidget::activateLoad );
}

BatchWidget::~BatchWidget() = default;


void BatchWidget::selectBatch( Arinc665::Media::ConstBatchPtr batch )
{
  batchV = std::move( batch );

  if ( batchV )
  {
    ui->partNumber->setText( HelperQt::toQString( batchV->partNumber() ) );
    ui->comment->setText( HelperQt::toQString( batchV->comment() ) );

    batchInfoModel->batchInformation( batchV->targets() );
    targetLoadsModel->loads( {} );

    ui->targets->selectRow( 0 );
  }
}

void BatchWidget::selectTarget( const QModelIndex &index )
{
  auto batchTargetInformation{ batchInfoModel->batchTargetInformation( index ) };

  targetLoadsModel->loads(
    batchInfoModel->constBatchTargetInformation( batchTargetInformation ).second );
}

void BatchWidget::activateLoad( const QModelIndex &index )
{
  auto load{ targetLoadsModel->constLoad(
    targetLoadsModel->load( index ) ) };

  if ( load )
  {
    emit activatedLoad( std::move( load ) );
  }
}

}

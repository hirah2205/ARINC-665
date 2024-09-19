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
 * @brief Definition of Class Arinc665Qt::Media::LoadWidget.
 **/

#include "LoadWidget.hpp"

#include "ui_LoadWidget.h"

#include "arinc_665_qt/media/BatchesModel.hpp"
#include "arinc_665_qt/media/LoadFilesModel.hpp"
#include "arinc_665_qt/media/TargetHardwareIdsPositionsModel.hpp"

#include "arinc_665/media/Load.hpp"
#include "arinc_665/media/RegularFile.hpp"
#include "arinc_665/media/MediaSet.hpp"

#include "arinc_645_qt/CheckValueTypeModel.hpp"

#include "helper_qt/String.hpp"

namespace Arinc665Qt::Media {

LoadWidget::LoadWidget( QWidget * const parent):
  QGroupBox{ parent },
  ui{ std::make_unique< Ui::LoadWidget>() },
  checkValueTypeModelV{
    std::make_unique< Arinc645Qt::CheckValueTypeModel >( this ) },
  targetHardwareIdsPositionsModel{
    std::make_unique< TargetHardwareIdsPositionsModel >( this ) },
  dataFilesModelV{ std::make_unique< LoadFilesModel >( this ) },
  supportFilesModelV{ std::make_unique< LoadFilesModel >( this ) },
  usedInBatchesModelV{ std::make_unique< BatchesModel >( this ) }
{
  ui->setupUi( this );

  ui->targetHardwareIdsPositions->setModel(
    targetHardwareIdsPositionsModel.get() );
  ui->dataFiles->setModel( dataFilesModelV.get() );
  ui->supportFiles->setModel( supportFilesModelV.get() );
  ui->usedInBatches->setModel( usedInBatchesModelV.get() );

  ui->loadCheckValueType->setModel( checkValueTypeModelV.get() );

  ui->dataFilesCheckValueType->setModel( checkValueTypeModelV.get() );
  ui->dataFiles->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );

  ui->supportFilesCheckValueType->setModel( checkValueTypeModelV.get() );
  ui->supportFiles->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );

  ui->usedInBatches->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::Stretch );

  connect(
    ui->dataFiles,
    &QTableView::activated,
    this,
    &LoadWidget::selectDataFile );
  connect(
    ui->supportFiles,
    &QTableView::activated,
    this,
    &LoadWidget::selectSupportFile );
  connect(
    ui->usedInBatches,
    &QTableView::activated,
    this,
    &LoadWidget::selectBatch );
}

LoadWidget::~LoadWidget() = default;

void LoadWidget::selectLoad( Arinc665::Media::ConstLoadPtr load )
{
  loadV = std::move( load );

  if ( loadV )
  {
    ui->partNumber->setText( HelperQt::toQString( loadV->partNumber() ) );

    ui->partFlags->setText( QString::number( loadV->partFlags(), 16 ) );

    ui->gbLoadType->setChecked( loadV->loadType().has_value() );
    ui->loadTypeDescription->setText( "" );
    ui->loadTypeId->setText( "" );
    if ( const auto loadType{ loadV->loadType() }; loadType )
    {
      ui->loadTypeDescription->setText(
        QString::fromStdString( loadType->first ) );
      ui->loadTypeId->setText( QString::number( loadType->second ) );
    }

    targetHardwareIdsPositionsModel->targetHardwareIdsPositions(
      loadV->targetHardwareIdPositions() );

    ui->loadCheckValueTypeGroupBox->setChecked(
      loadV->loadCheckValueType().has_value() );
    ui->loadCheckValueType->setCurrentIndex(
      Arinc645Qt::CheckValueTypeModel::checkValueType(
        loadV->effectiveLoadCheckValueType() ) );

    ui->dataFilesCheckValueTypeGroupBox->setChecked(
      loadV->dataFilesCheckValueType().has_value() );
    ui->dataFilesCheckValueType->setCurrentIndex(
      Arinc645Qt::CheckValueTypeModel::checkValueType(
        loadV->effectiveDataFilesCheckValueType() ) );
    dataFilesModelV->loadFiles( loadV->dataFiles() );

    ui->supportFilesCheckValueTypeGroupBox->setChecked(
      loadV->supportFilesCheckValueType().has_value() );
    ui->supportFilesCheckValueType->setCurrentIndex(
      Arinc645Qt::CheckValueTypeModel::checkValueType(
        loadV->effectiveSupportFilesCheckValueType() ) );
    supportFilesModelV->loadFiles( loadV->supportFiles() );

    usedInBatchesModelV->batches( loadV->mediaSet()->batchesWithLoad( loadV ) );
  }
}

void LoadWidget::selectDataFile( const QModelIndex &index )
{
  auto file{ dataFilesModelV->loadFile( index ) };

  if ( file )
  {
    emit activatedFile( std::get< 0 >( *file ) );
  }
}

void LoadWidget::selectSupportFile( const QModelIndex &index )
{
  auto file{ supportFilesModelV->loadFile( index ) };

  if ( file )
  {
    emit activatedFile( std::get< 0 >( *file ) );
  }
}

void LoadWidget::selectBatch( const QModelIndex &index )
{
  auto batch{
    usedInBatchesModelV->constBatch( usedInBatchesModelV->batch( index ) ) };

  if ( batch )
  {
    emit activatedBatch( batch );
  }
}

}

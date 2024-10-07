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
 * @brief Definition of Class Arinc665Qt::MediaPathsWidget.
 **/

#include "MediaPathsWidget.hpp"

#include "ui_MediaPathsWidget.h"

#include "arinc_665_qt/MediaPathsModel.hpp"

#include <QMessageBox>

namespace Arinc665Qt {

MediaPathsWidget::MediaPathsWidget(
  QWidget * const parent ) :
  QGroupBox{ parent },
  ui{ std::make_unique< Ui::MediaPathsWidget >() },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >( this ) },
  selectMediaPathDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );

  ui->mediaPaths->setModel( mediaPathsModelV.get() );
  ui->mediaPaths->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeMode::ResizeToContents );

  ui->remove->setEnabled( false );

  selectMediaPathDialog->setWindowTitle( tr( "Select Medium Directory" ) );
  selectMediaPathDialog->setFileMode( QFileDialog::FileMode::Directory );
  selectMediaPathDialog->setOptions( QFileDialog::Option::ShowDirsOnly );

  connect(
    ui->mediaPaths->selectionModel(),
    &QItemSelectionModel::currentChanged,
    this,
    &MediaPathsWidget::updateButtons );

  connect(
    ui->add,
    &QPushButton::pressed,
    selectMediaPathDialog.get(),
    qOverload<>( &QDialog::open ) );

  connect(
    ui->remove,
    &QPushButton::pressed,
    this,
    &MediaPathsWidget::removeMediumDirectory );

  connect(
    selectMediaPathDialog.get(),
    &QFileDialog::fileSelected,
    this,
    &MediaPathsWidget::mediumDirectorySelected );
}

MediaPathsWidget::~MediaPathsWidget() = default;

bool MediaPathsWidget::completed() const
{
  return mediaPathsModelV->complete();
}

void MediaPathsWidget::clear()
{
  mediaPathsModelV->clear();
}

void MediaPathsWidget::updateButtons( const QModelIndex &current )
{
  ui->remove->setEnabled( current.isValid() );
}

void MediaPathsWidget::removeMediumDirectory()
{
  mediaPathsModelV->remove( ui->mediaPaths->currentIndex() );

  emit mediaPathsChanged( mediaPathsModelV->mediaPaths() );

  ui->mediaPaths->setCurrentIndex( QModelIndex{} );
}

void MediaPathsWidget::mediumDirectorySelected( const QString &file )
{
  if ( !mediaPathsModelV->mediumPath( file.toStdString() ) )
  {
    QMessageBox::warning(
      this,
      tr( "Add Medium" ),
      QString{ tr( "%1 is not a valid ARINC 665 medium." ) }.arg( file ) );
    return;
  }

  ui->mediaPaths->resizeColumnsToContents();
  emit mediaPathsChanged( mediaPathsModelV->mediaPaths() );
}

}

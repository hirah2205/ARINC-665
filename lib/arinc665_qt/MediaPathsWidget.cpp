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

#include <arinc665_qt/MediaPathsModel.hpp>

#include <arinc665/utils/Utils.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <QMessageBox>

namespace Arinc665Qt {

MediaPathsWidget::MediaPathsWidget(
  QWidget * const parent ) :
  QGroupBox{ parent },
  ui{ std::make_unique< Ui::MediaPathsWidget >() },
  selectMediaPathDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );

  selectMediaPathDialog->setWindowTitle( tr( "Select Medium Directory" ) );
  selectMediaPathDialog->setFileMode( QFileDialog::FileMode::Directory );
  selectMediaPathDialog->setOptions( QFileDialog::Option::ShowDirsOnly );

  connect(
    ui->add,
    &QPushButton::pressed,
    this,
    &MediaPathsWidget::addMediumDirectory );

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

void MediaPathsWidget::mediaPathsModel( MediaPathsModel * const model )
{
  mediaPathsModelV = model;
  ui->mediaPaths->setModel( model );
}

void MediaPathsWidget::addMediumDirectory()
{
  selectMediaPathDialog->exec();
}

void MediaPathsWidget::removeMediumDirectory()
{
  mediaPathsModelV->remove( ui->mediaPaths->currentIndex() );
}

void MediaPathsWidget::mediumDirectorySelected( const QString &file )
{
  const auto mediumInformation{
    Arinc665::Utils::getMediumInformation( file.toStdString() ) };

  if ( !mediumInformation )
  {
    QMessageBox::warning( this, tr( "Add Medium"), tr( "is not a medium" ) );
    return;
  }

  mediaPathsModelV->mediumPath(
    mediumInformation->mediaSequenceNumber,
    file.toStdString() );
}

}

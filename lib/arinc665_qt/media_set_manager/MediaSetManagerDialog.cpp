/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class
 *   Arinc665Qt::MediaSetManager::MediaSetManagerDialog.
 **/

#include "MediaSetManagerDialog.hpp"

#include "ui_MediaSetManagerDialog.h"

#include <arinc665_qt/media/MediaSetsModel.hpp>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerDialog::MediaSetManagerDialog( QWidget * const parent ) :
  QDialog{ parent },
  ui{ std::make_unique< Ui::MediaSetManagerDialog>() },
  mediaSetsModelV{}
{
  ui->setupUi( this );

  connect(
    ui->viewMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::viewMediaSetClicked );
  connect(
    ui->importMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::importMediaSet );
  connect(
    ui->importMediaSetXml,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::importMediaSetXml );
  connect(
    ui->removeMediaSet,
    &QPushButton::clicked,
    this,
    &MediaSetManagerDialog::removeMediaSetClicked );
}

MediaSetManagerDialog::~MediaSetManagerDialog() = default;

void MediaSetManagerDialog::mediaSetsModel(
  Media::MediaSetsModel * const model )
{
  mediaSetsModelV = model;
  ui->mediaSets->setModel( model );
}

void MediaSetManagerDialog::viewMediaSetClicked()
{
  if ( ui->mediaSets->currentIndex().isValid() )
  {
    emit viewMediaSet( ui->mediaSets->currentIndex() );
  }
}

void MediaSetManagerDialog::removeMediaSetClicked()
{
  if ( ui->mediaSets->currentIndex().isValid() )
  {
    emit removeMediaSet( ui->mediaSets->currentIndex() );
  }
}

}

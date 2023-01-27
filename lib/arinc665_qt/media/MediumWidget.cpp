/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::MediumWidget.
 **/

#include "MediumWidget.hpp"

#include "ui_MediumWidget.h"

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665/media/Medium.hpp>

namespace Arinc665Qt::Media {

MediumWidget::MediumWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::MediumWidget>() },
  mediaSetModelV{ nullptr }
{
  ui->setupUi( this );
}

MediumWidget::~MediumWidget() = default;

void MediumWidget::mediaSetModel(
  Arinc665Qt::Media::MediaSetModel * const model )
{
  mediaSetModelV = model;
  ui->content->setModel( model );
  ui->content->resizeColumnsToContents();
}

void MediumWidget::selectedMedium( const QModelIndex &index )
{
  ui->content->setRootIndex( index );
  ui->content->resizeColumnsToContents();
}

void MediumWidget::selectedMedium( Arinc665::Media::ConstMediumPtr medium )
{
  mediumV = std::move( medium );

  if ( mediumV )
  {
    ui->numberSpinBox->setValue( mediumV->mediumNumber() );
  }
}

}

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

#include <arinc665/media/MediaSet.hpp>

#include "ui_MediaSetWidget.h"

namespace Arinc665Qt::Media {

MediaSetWidget::MediaSetWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::MediaSetWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this);
}

MediaSetWidget::~MediaSetWidget() = default;

void MediaSetWidget::selectedMediaSet(
  MediaSetModel * model,
  Arinc665::Media::MediaSetPtr mediaSet)
{
  modelV = model;
  mediaSetV = std::move( mediaSet);

  if ( mediaSetV)
  {
    ui->partNumberLineEdit->setText( QString::fromUtf8(
      mediaSetV->partNumber().data(),
      static_cast< int >( mediaSetV->partNumber().length() ) ) );
  }
}

}

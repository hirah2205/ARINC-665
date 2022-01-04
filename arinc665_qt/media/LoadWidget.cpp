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

namespace Arinc665Qt::Media {

LoadWidget::LoadWidget( QWidget *parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::LoadWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this);
}

LoadWidget::~LoadWidget() = default;

void LoadWidget::selectedLoad(
  Arinc665Qt::Media::MediaSetModel * const model,
  Arinc665::Media::LoadPtr load)
{
  modelV = model;
  loadV = std::move( load );

  if ( loadV )
  {
  }
}

}

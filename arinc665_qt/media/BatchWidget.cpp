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

namespace Arinc665Qt::Media {

BatchWidget::BatchWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::BatchWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this);
}

BatchWidget::~BatchWidget() = default;

void BatchWidget::selectedBatch(
  MediaSetModel * const model,
  Arinc665::Media::BatchPtr batch)
{
  modelV = model;
  batchV = std::move( batch);

  if ( batchV)
  {
  }
}

}

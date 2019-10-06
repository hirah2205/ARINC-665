/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of class Arinc665Qt::MediumWidget.
 **/

#include "MediumWidget.hpp"

#include <arinc665/media/Medium.hpp>

#include "ui_MediumWidget.h"

namespace Arinc665Qt {

MediumWidget::MediumWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::MediumWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this);
}

MediumWidget::~MediumWidget() = default;

void MediumWidget::selectedMedium( MediaSetModel * model, Arinc665::Media::MediumPtr medium)
{
  modelV = model;
  mediumV = std::move( medium);

  if (mediumV)
  {
    ui->numberSpinBox->setValue( mediumV->mediumNumber());
  }
}

}

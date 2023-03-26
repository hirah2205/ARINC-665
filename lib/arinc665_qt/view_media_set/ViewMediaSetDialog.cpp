/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetDialog.
 **/

#include "ViewMediaSetDialog.hpp"

#include "ui_ViewMediaSetDialog.h"

#include <arinc665_qt/media/MediaSetModel.hpp>

namespace Arinc665Qt {

ViewMediaSetDialog::ViewMediaSetDialog( QWidget * const parent ):
  QDialog{ parent },
  ui{ std::make_unique< Ui::ViewMediaSetDialog >() }
{
  ui->setupUi( this );
}

ViewMediaSetDialog::~ViewMediaSetDialog() = default;

void ViewMediaSetDialog::mediaSetModel( Media::MediaSetModel * const model )
{
  ui->mediaSetView->mediaSetModel( model );
}

}

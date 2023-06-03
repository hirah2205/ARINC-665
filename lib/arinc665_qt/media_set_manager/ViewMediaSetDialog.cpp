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
 * @brief Definition of Class Arinc665Qt::MediaSetManager::ViewMediaSetDialog.
 **/

#include "ViewMediaSetDialog.hpp"

#include "ui_ViewMediaSetDialog.h"

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665/media/MediaSet.hpp>

namespace Arinc665Qt::MediaSetManager {

ViewMediaSetDialog::ViewMediaSetDialog( QWidget * const parent ):
  QDialog{ parent },
  ui{ std::make_unique< Ui::ViewMediaSetDialog >() },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) }
{
  ui->setupUi( this );
  ui->mediaSetView->mediaSetModel( mediaSetModelV.get() );
}

ViewMediaSetDialog::~ViewMediaSetDialog() = default;

void ViewMediaSetDialog::mediaSet( Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  mediaSetModelV->root( std::move( mediaSet ) );
}

}

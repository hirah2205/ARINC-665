/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#include "MediaSetDialog.hpp"

#include <arinc665_qt/MediaSetModel.hpp>

#include "ui_MediaSetDialog.h"

namespace Arinc665Qt {

MediaSetDialog::MediaSetDialog( QWidget * const parent):
  QDialog{ parent},
  ui{ std::make_unique< Ui::MediaSetDialog>()},
  mediaSetModelV{ nullptr}
{
  ui->setupUi( this);
}

MediaSetDialog::~MediaSetDialog() = default;

void MediaSetDialog::model( MediaSetModel * mediaSetModel)
{
  if ( mediaSetModelV != nullptr)
  {
    // disconnect old model
  }

  mediaSetModelV = mediaSetModel;

  ui->mediaSetTreeView->setModel( mediaSetModel);
}

}

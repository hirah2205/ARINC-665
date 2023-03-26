/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetXmlDialog.
 **/

#include "ViewMediaSetXmlDialog.hpp"

#include "ui_ViewMediaSetXmlDialog.h"

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665_qt/FilePathMappingModel.hpp>

namespace Arinc665Qt {

ViewMediaSetXmlDialog::ViewMediaSetXmlDialog( QWidget * const parent ):
  QDialog{ parent },
  ui{ std::make_unique< Ui::ViewMediaSetXmlDialog >() }
{
  ui->setupUi( this );
}

ViewMediaSetXmlDialog::~ViewMediaSetXmlDialog() = default;

void ViewMediaSetXmlDialog::mediaSetModel( Media::MediaSetModel * const model )
{
  ui->mediaSetView->mediaSetModel( model );
}

void ViewMediaSetXmlDialog::filePathMappingModel(
  FilePathMappingModel * const model )
{
  ui->filePathMapping->setModel( model );
}

}

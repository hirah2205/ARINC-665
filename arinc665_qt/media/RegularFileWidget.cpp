/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::RegularFileWidget.
 **/

#include "RegularFileWidget.hpp"

#include "ui_RegularFileWidget.h"

namespace Arinc665Qt::Media {

RegularFileWidget::RegularFileWidget( QWidget * const parent ) :
  QWidget{ parent },
  ui{ std::make_unique< Ui::RegularFileWidget>() },
  modelV{ nullptr }
{
  ui->setupUi( this );
}

RegularFileWidget::~RegularFileWidget() = default;

void RegularFileWidget::selectedFile(
  Arinc665Qt::Media::MediaSetModel * const model,
  Arinc665::Media::ConstFilePtr file )
{
  modelV = model;
  fileV = std::move( file );

  if ( fileV )
  {
  }
}

}

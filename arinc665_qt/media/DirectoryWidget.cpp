/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::DirectoryWidget.
 **/

#include "DirectoryWidget.hpp"

#include <arinc665/media/Directory.hpp>

#include "ui_DirectoryWidget.h"

namespace Arinc665Qt::Media {

DirectoryWidget::DirectoryWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::DirectoryWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this);
}

DirectoryWidget::~DirectoryWidget() = default;

void DirectoryWidget::selectedDirectory(
  MediaSetModel * model,
  Arinc665::Media::DirectoryPtr directory)
{
  modelV = model;
  directoryV = std::move( directory);

  if ( directoryV)
  {
    ui->nameLineEdit->setText( QString::fromUtf8(
      directoryV->name().data(),
      directoryV->name().length()));
  }
}

}

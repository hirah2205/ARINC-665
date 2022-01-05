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

#include <arinc665_qt/media/LoadFilesModel.hpp>

#include <arinc665/media/Load.hpp>

namespace Arinc665Qt::Media {

LoadWidget::LoadWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::LoadWidget>() },
  dataFilesModelV{ std::make_unique< LoadFilesModel >( this ) },
  supportFilesModelV{ std::make_unique< LoadFilesModel >( this ) },
  mediaSetModelV{ nullptr }
{
  ui->setupUi( this );
}

LoadWidget::~LoadWidget() = default;

void LoadWidget::selectedLoad(
  Arinc665Qt::Media::MediaSetModel * const model,
  Arinc665::Media::LoadPtr load )
{
  mediaSetModelV = model;
  loadV = std::move( load );

  if ( loadV )
  {
    dataFilesModelV->loadFiles( loadV->dataFiles() );
    supportFilesModelV->loadFiles( loadV->supportFiles() );
  }

}

}

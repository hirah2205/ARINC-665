/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::FileWidget.
 **/

#include "FileWidget.hpp"

#include <arinc665/media/File.hpp>
#include <arinc665/media/RegularFile.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include "ui_FileWidget.h"

namespace Arinc665Qt::Media {

FileWidget::FileWidget( QWidget * const parent):
  QWidget{ parent},
  ui{ std::make_unique< Ui::FileWidget>()},
  modelV{ nullptr}
{
  ui->setupUi( this );
}

FileWidget::~FileWidget() = default;

void FileWidget::selectedFile(
  Arinc665Qt::Media::MediaSetModel * const model,
  Arinc665::Media::ConstFilePtr file )
{
  modelV = model;
  fileV = std::move( file );

  if ( fileV )
  {
    ui->nameLineEdit->setText( QString::fromUtf8(
      fileV->name().data(),
      static_cast< int >( fileV->name().length() ) ) );
  }

  switch ( fileV->fileType())
  {
    case Arinc665::Media::File::FileType::RegularFile:
      ui->detailsStackedWidget->setCurrentIndex( 0 );
      ui->regularFilePage->selectedFile(
        modelV,
        std::dynamic_pointer_cast< const Arinc665::Media::RegularFile >( fileV ) );
      break;

    case Arinc665::Media::File::FileType::LoadFile:
      ui->detailsStackedWidget->setCurrentIndex( 1 );
      ui->loadPage->selectedLoad(
        std::dynamic_pointer_cast< const Arinc665::Media::Load >( fileV ) );
      break;

    case Arinc665::Media::File::FileType::BatchFile:
      ui->detailsStackedWidget->setCurrentIndex( 2 );
      ui->batchPage->selectedBatch(
        std::dynamic_pointer_cast< const Arinc665::Media::Batch >( fileV ) );
      break;

    default:
      break;
  }

}

}

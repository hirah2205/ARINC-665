/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::DecompileMediaSetWizard.
 **/

#include "DecompileMediaSetWizard.hpp"

#include "ui_DecompileMediaSetWizard.h"

#include <arinc665_qt/FilePathMappingModel.hpp>

namespace Arinc665Qt {

DecompileMediaSetWizard::DecompileMediaSetWizard( QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::DecompileMediaSetWizard >() },
  selectXmlFileDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );
  ui->settings->setCommitPage( true );

  selectXmlFileDialog->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectXmlFileDialog->setNameFilter(tr( "ARINC 665 Media Set XML (*.xml)" ) );
  selectXmlFileDialog->setFileMode( QFileDialog::FileMode::AnyFile );
  selectXmlFileDialog->setAcceptMode( QFileDialog::AcceptMode::AcceptSave );
  selectXmlFileDialog->setDefaultSuffix( "xml" );

  connect(
    ui->save,
    &QPushButton::clicked,
    selectXmlFileDialog.get(),
    &QFileDialog::exec );
  connect(
    selectXmlFileDialog.get(),
    &QFileDialog::fileSelected,
    this,
    &DecompileMediaSetWizard::saveXmlFile );

  connect(
    this,
    &QWizard::currentIdChanged,
    this,
    &DecompileMediaSetWizard::pageChanged );
}

DecompileMediaSetWizard::~DecompileMediaSetWizard() = default;

void DecompileMediaSetWizard::mediaPathsModel( MediaPathsModel * const model )
{
  ui->mediaPaths->mediaPathsModel( model );
}

void DecompileMediaSetWizard::mediaSetModel( Media::MediaSetModel * const model )
{
  ui->mediaSetView->mediaSetModel( model );
}

void DecompileMediaSetWizard::filePathMappingModel(
  FilePathMappingModel * const model )
{
  ui->filePathMapping->setModel( model );
}

void DecompileMediaSetWizard::pageChanged( const int id )
{
  if ( ui->settings->nextId() == id )
  {
    emit start();

    selectXmlFileDialog->selectFile(
      QString::asprintf(
        "%s.xml",
        std::string{ ui->mediaSetView->partNumber() }.c_str() ) );
  }
}

void DecompileMediaSetWizard::saveXmlFile( const QString &file )
{
  emit save( file.toStdString() );
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlSettingsWidget.
 **/

#include "ImportMediaSetXmlSettingsWidget.hpp"

#include "ui_ImportMediaSetXmlSettingsWidget.h"

namespace Arinc665Qt {

ImportMediaSetXmlSettingsWidget::ImportMediaSetXmlSettingsWidget(
  QWidget * const parent ) :
  QGroupBox{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlSettingsWidget >() },
  selectXmlFileDialog{ std::make_unique< QFileDialog >( this ) },
  selectInputDirectoryDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );

  selectXmlFileDialog->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectXmlFileDialog->setNameFilter(tr( "ARINC 665 Media Set XML (*.xml)" ) );
  selectXmlFileDialog->setFileMode( QFileDialog::FileMode::ExistingFile );

  connect(
    ui->selectXmlFile,
    &QPushButton::clicked,
    this,
    &ImportMediaSetXmlSettingsWidget::selectXmlFile );
  connect(
    selectXmlFileDialog.get(),
    &QFileDialog::fileSelected,
    this,
    &ImportMediaSetXmlSettingsWidget::xmlFileSelected );

  selectInputDirectoryDialog->setWindowTitle( tr( "Select Input Base Directory" ) );
  selectInputDirectoryDialog->setFileMode( QFileDialog::FileMode::Directory );
  selectInputDirectoryDialog->setOptions( QFileDialog::Option::ShowDirsOnly );

  connect(
    ui->selectInputDirectory,
    &QPushButton::clicked,
    this,
    &ImportMediaSetXmlSettingsWidget::selectInputDirectory );
  connect(
    selectInputDirectoryDialog.get(),
    &QFileDialog::fileSelected,
    this,
    &ImportMediaSetXmlSettingsWidget::inputDirectorySelected );
}

ImportMediaSetXmlSettingsWidget::~ImportMediaSetXmlSettingsWidget() = default;

bool ImportMediaSetXmlSettingsWidget::completed() const
{
  return !ui->xmlFile->text().isEmpty()
    && !ui->inputDirectory->text().isEmpty();
}

void ImportMediaSetXmlSettingsWidget::selectXmlFile()
{
  selectXmlFileDialog->exec();
}

void ImportMediaSetXmlSettingsWidget::xmlFileSelected( const QString &file )
{
  ui->xmlFile->setText( file );
  emit xmlFile( file.toStdString() );

  // if not already set, update input directory
  if ( ui->inputDirectory->text().isEmpty() )
  {
    inputDirectorySelected( selectXmlFileDialog->directory().path() );
  }
}

void ImportMediaSetXmlSettingsWidget::selectInputDirectory()
{
  selectInputDirectoryDialog->setDirectory( selectXmlFileDialog->directory() );
  selectInputDirectoryDialog->exec();
}

void ImportMediaSetXmlSettingsWidget::inputDirectorySelected(
  const QString &file )
{
  ui->inputDirectory->setText( file );
  emit inputDirectory( file.toStdString() );
}

}

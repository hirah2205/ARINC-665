/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlSettingsPage.
 **/

#include "ImportMediaSetXmlSettingsPage.hpp"

#include "ui_ImportMediaSetXmlSettingsPage.h"

namespace Arinc665Qt {

ImportMediaSetXmlSettingsPage::ImportMediaSetXmlSettingsPage(
  QWidget * const parent ) :
  QWizardPage{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlSettingsPage >() }
{
  ui->setupUi( this );

  connect(
    ui->mediaSetXmlSettings,
    &ImportMediaSetXmlSettingsWidget::xmlFile,
    this,
    &ImportMediaSetXmlSettingsPage::xmlFile );
  connect(
    ui->mediaSetXmlSettings,
    &ImportMediaSetXmlSettingsWidget::xmlFile,
    this,
    &QWizardPage::completeChanged );

  connect(
    ui->mediaSetXmlSettings,
    &ImportMediaSetXmlSettingsWidget::inputDirectory,
    this,
    &ImportMediaSetXmlSettingsPage::inputDirectory );
  connect(
    ui->mediaSetXmlSettings,
    &ImportMediaSetXmlSettingsWidget::inputDirectory,
    this,
    &ImportMediaSetXmlSettingsPage::completeChanged );

  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::arinc665Version,
    this,
    &ImportMediaSetXmlSettingsPage::arinc665Version );
  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::arinc665Version,
    this,
    &ImportMediaSetXmlSettingsPage::completeChanged );

  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::createBatchFiles,
    this,
    &ImportMediaSetXmlSettingsPage::createBatchFiles );
  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::createBatchFiles,
    this,
    &ImportMediaSetXmlSettingsPage::completeChanged );

  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::createLoadHeaderFiles,
    this,
    &ImportMediaSetXmlSettingsPage::createLoadHeaderFiles );
  connect(
    ui->mediaSetOutputSettings,
    &ExportMediaSetSettingsWidget::createLoadHeaderFiles,
    this,
    &ImportMediaSetXmlSettingsPage::completeChanged );

}

ImportMediaSetXmlSettingsPage::~ImportMediaSetXmlSettingsPage() = default;

bool ImportMediaSetXmlSettingsPage::isComplete() const
{
  return QWizardPage::isComplete()
    && ui->mediaSetXmlSettings->completed()
    && ui->mediaSetOutputSettings->completed();
}

}

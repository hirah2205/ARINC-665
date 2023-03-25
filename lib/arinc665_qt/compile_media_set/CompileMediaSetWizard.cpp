/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::CompileMediaSetWizard.
 **/

#include "CompileMediaSetWizard.hpp"

#include "ui_CompileMediaSetWizard.h"

namespace Arinc665Qt {

CompileMediaSetWizard::CompileMediaSetWizard( QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::CompileMediaSetWizard >() }
{
  ui->setupUi( this );
  ui->settings->setCommitPage( true );

  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::xmlFile,
    this,
    &CompileMediaSetWizard::xmlFile );
  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::inputDirectory,
    this,
    &CompileMediaSetWizard::inputDirectory );
  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::arinc665Version,
    this,
    &CompileMediaSetWizard::arinc665Version );
  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::createBatchFiles,
    this,
    &CompileMediaSetWizard::createBatchFiles );
  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::createLoadHeaderFiles,
    this,
    &CompileMediaSetWizard::createLoadHeaderFiles );
  connect(
    ui->settings,
    &CompileMediaSetSettingsPage::outputDirectory,
    this,
    &CompileMediaSetWizard::outputDirectory );

  connect(
    this,
    &QWizard::currentIdChanged,
    this,
    &CompileMediaSetWizard::pageChanged );
}

CompileMediaSetWizard::~CompileMediaSetWizard() = default;

void CompileMediaSetWizard::pageChanged( int id )
{
  if ( ui->settings->nextId() == id )
  {
    emit start();
  }
}

}

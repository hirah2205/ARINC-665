/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetWizard.
 **/

#include "ImportMediaSetWizard.hpp"

#include "ui_ImportMediaSetWizard.h"

namespace Arinc665Qt {

ImportMediaSetWizard::ImportMediaSetWizard( QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetWizard >() }
{
  ui->setupUi( this );
  ui->settings->setCommitPage( true );

  connect(
    this,
    &QWizard::currentIdChanged,
    this,
    &ImportMediaSetWizard::pageChanged );
  connect(
    ui->settings,
    &ImportMediaSetSettingsPage::checkFileIntegrity,
    this,
    &ImportMediaSetWizard::checkFileIntegrity );
}

ImportMediaSetWizard::~ImportMediaSetWizard() = default;

void ImportMediaSetWizard::mediaPathsModel( MediaPathsModel * const model )
{
  ui->settings->mediaPathsModel( model );
}

void ImportMediaSetWizard::pageChanged( const int id )
{
  if ( ui->settings->nextId() == id )
  {
    emit start();
  }
}

}

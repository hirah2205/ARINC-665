// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetManager::ImportMediaSetSettingsPage.
 **/

#include "ImportMediaSetSettingsPage.hpp"

#include "ui_ImportMediaSetSettingsPage.h"

#include <arinc_665/utils/MediaSetDefaults.hpp>

namespace Arinc665Qt::MediaSetManager {

ImportMediaSetSettingsPage::ImportMediaSetSettingsPage(
  QWidget * const parent ) :
  QWizardPage{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetSettingsPage >() }
{
  ui->setupUi( this );

  connect(
    ui->mediaPaths,
    &MediaPathsWidget::mediaPathsChanged,
    this,
    &ImportMediaSetSettingsPage::mediaPathsChanged );
  connect(
    ui->mediaPaths,
    &MediaPathsWidget::mediaPathsChanged,
    this,
    &ImportMediaSetSettingsPage::completeChanged );

  connect(
    ui->checkFileIntegrity,
    &QCheckBox::checkStateChanged,
    this,
    &ImportMediaSetSettingsPage::checkFileIntegrityStateChanged );
}

ImportMediaSetSettingsPage::~ImportMediaSetSettingsPage() = default;

bool ImportMediaSetSettingsPage::isComplete() const
{
  return QWizardPage::isComplete() && ui->mediaPaths->completed();
}

void ImportMediaSetSettingsPage::defaults(
  const Arinc665::Utils::MediaSetDefaults &defaults )
{
  ui->checkFileIntegrity->setChecked( defaults.checkFileIntegrity );
}

void ImportMediaSetSettingsPage::checkFileIntegrityStateChanged(
  const Qt::CheckState state )
{
  switch ( state )
  {
    case Qt::CheckState::Unchecked:
      emit checkFileIntegrity( false );
      break;

    case Qt::CheckState::Checked:
      emit checkFileIntegrity( true );
      break;

    default:
      break;
  }
}

}

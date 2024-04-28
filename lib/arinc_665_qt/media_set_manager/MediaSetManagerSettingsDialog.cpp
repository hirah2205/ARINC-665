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
 * @brief Declaration of Class
 *   Arinc665Qt::MediaSetManager::MediaSetManagerSettingsDialog.
 **/

#include "MediaSetManagerSettingsDialog.hpp"

#include "ui_MediaSetManagerSettingsDialog.h"

#include "arinc_665_qt/SupportedArinc665VersionModel.hpp"
#include "arinc_665_qt/FileCreationPolicyModel.hpp"

#include "arinc_665/utils/MediaSetDefaults.hpp"

#include <QSettings>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerSettingsDialog::MediaSetManagerSettingsDialog(
  QWidget * const parent ) :
  QDialog{ parent },
  ui{ std::make_unique< Ui::MediaSetManagerSettingsDialog>() },
  supportedArinc665VersionModelV{
    std::make_unique< SupportedArinc665VersionModel >( this ) },
  fileCreationPolicyModelV{ std::make_unique< FileCreationPolicyModel >( this ) }
{
  ui->setupUi( this );

  ui->mediaSetDefaultsVersion->setModel( supportedArinc665VersionModelV.get() );
  ui->mediaSetDefaultsLoadHeaderFilePolicy->setModel(
    fileCreationPolicyModelV.get() );
  ui->mediaSetDefaultsBatchFilePolicy->setModel(
    fileCreationPolicyModelV.get() );
}

MediaSetManagerSettingsDialog::~MediaSetManagerSettingsDialog() = default;

Arinc665::Utils::MediaSetDefaults
MediaSetManagerSettingsDialog::configuration() const
{
  Arinc665::Utils::MediaSetDefaults defaults{};

  defaults.checkFileIntegrity = ui->mediaSetDefaultsCheckIntegrity->isChecked();
  defaults.version =
    supportedArinc665VersionModelV->supportedArinc665Version(
      ui->mediaSetDefaultsVersion->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultVersion );
  defaults.loadHeaderFileCreationPolicy =
    fileCreationPolicyModelV->fileCreationPolicy(
      ui->mediaSetDefaultsLoadHeaderFilePolicy->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy );
  defaults.batchFileCreationPolicy =
    fileCreationPolicyModelV->fileCreationPolicy(
      ui->mediaSetDefaultsBatchFilePolicy->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy );

  QSettings settings{};
  settings.setValue(
    "CheckIntegrityOnStartup",
    ui->checkIntegrityOnStartup->isChecked() );

  return defaults;
}

void MediaSetManagerSettingsDialog::configuration(
  const Arinc665::Utils::MediaSetDefaults &defaults )
{
  ui->mediaSetDefaultsCheckIntegrity->setChecked( defaults.checkFileIntegrity );
  ui->mediaSetDefaultsVersion->setCurrentIndex(
    supportedArinc665VersionModelV->supportedArinc665Version(
      defaults.version ) );
  ui->mediaSetDefaultsLoadHeaderFilePolicy->setCurrentIndex(
    fileCreationPolicyModelV->fileCreationPolicy(
      defaults.loadHeaderFileCreationPolicy ) );
  ui->mediaSetDefaultsBatchFilePolicy->setCurrentIndex(
    fileCreationPolicyModelV->fileCreationPolicy(
      defaults.batchFileCreationPolicy ) );

  QSettings settings{};
  ui->checkIntegrityOnStartup->setChecked(
    settings.value( "CheckIntegrityOnStartup", true ).toBool() );
}

}

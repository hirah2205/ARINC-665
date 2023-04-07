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

#include <arinc665_qt/SupportedArinc665VersionModel.hpp>
#include <arinc665_qt/FileCreationPolicyModel.hpp>

#include <arinc665/utils/MediaSetDefaults.hpp>

#include "ui_MediaSetManagerSettingsDialog.h"

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

  ui->arinc665MediaSetImporterVersion->setModel(
    supportedArinc665VersionModelV.get() );
  ui->arinc665MediaSetImporterBatchFilePolicy->setModel(
    fileCreationPolicyModelV.get() );
  ui->arinc665MediaSetImporterLoadHeaderFilePolicy->setModel(
    fileCreationPolicyModelV.get() );
}

MediaSetManagerSettingsDialog::~MediaSetManagerSettingsDialog() = default;

Arinc665::Utils::MediaSetDefaults
MediaSetManagerSettingsDialog::configuration() const
{
  Arinc665::Utils::MediaSetDefaults defaults{};

  defaults.checkFileIntegrity =
    ui->arinc665MediaSetImporterCheckIntegrity->isChecked();
  defaults.version =
    supportedArinc665VersionModelV->supportedArinc665Version(
      ui->arinc665MediaSetImporterVersion->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultVersion );
  defaults.loadHeaderFileCreationPolicy =
    fileCreationPolicyModelV->fileCreationPolicy(
      ui->arinc665MediaSetImporterLoadHeaderFilePolicy->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy );
  defaults.batchFileCreationPolicy =
    fileCreationPolicyModelV->fileCreationPolicy(
      ui->arinc665MediaSetImporterBatchFilePolicy->currentIndex() ).value_or(
        Arinc665::Utils::MediaSetDefaults::DefaultFileCreationPolicy );

  return defaults;
}

void MediaSetManagerSettingsDialog::configuration(
  const Arinc665::Utils::MediaSetDefaults &defaults )
{
  ui->arinc665MediaSetImporterCheckIntegrity->setChecked(
    defaults.checkFileIntegrity );
  ui->arinc665MediaSetImporterVersion->setCurrentIndex(
    supportedArinc665VersionModelV->supportedArinc665Version(
      defaults.version ) );
  ui->arinc665MediaSetImporterLoadHeaderFilePolicy->setCurrentIndex(
    fileCreationPolicyModelV->fileCreationPolicy(
      defaults.loadHeaderFileCreationPolicy ) );
  ui->arinc665MediaSetImporterBatchFilePolicy->setCurrentIndex(
    fileCreationPolicyModelV->fileCreationPolicy(
      defaults.batchFileCreationPolicy ) );
}

}

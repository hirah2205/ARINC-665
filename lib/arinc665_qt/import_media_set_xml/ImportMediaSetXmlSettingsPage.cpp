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

#include <arinc665/SupportedArinc665VersionDescription.hpp>
#include <arinc665/utils/FileCreationPolicyDescription.hpp>

namespace Arinc665Qt {

ImportMediaSetXmlSettingsPage::ImportMediaSetXmlSettingsPage(
  QWidget * const parent ) :
  QWizardPage{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlSettingsPage >() },
  selectXmlFileDialog{ std::make_unique< QFileDialog >( this ) },
  selectInputDirectoryDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );

  connect(
    ui->selectXmlFile,
    &QPushButton::clicked,
    this,
    &ImportMediaSetXmlSettingsPage::selectXmlFile );

  connect(
    ui->selectInputDirectory,
    &QPushButton::clicked,
    this,
    &ImportMediaSetXmlSettingsPage::selectInputDirectory );

  for ( const auto &versionInfo : Arinc665::SupportedArinc665VersionDescription::instance() )
  {
    ui->arinc665Version->addItem( QString::fromStdString( versionInfo.name ) );
  }

  for ( const auto &creationPolicy : Arinc665::Utils::FileCreationPolicyDescription::instance() )
  {
    ui->loadHeaderCreation->addItem( QString::fromStdString( creationPolicy.name ) );
    ui->batchFileCreation->addItem( QString::fromStdString( creationPolicy.name ) );
  }
}

ImportMediaSetXmlSettingsPage::~ImportMediaSetXmlSettingsPage() = default;

void ImportMediaSetXmlSettingsPage::selectXmlFile()
{
  selectXmlFileDialog->exec();
}

void ImportMediaSetXmlSettingsPage::selectInputDirectory()
{
  selectInputDirectoryDialog->exec();
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ExportMediaSetSettingsWidget.
 **/

#include "ExportMediaSetSettingsWidget.hpp"

#include <arinc665_qt/FileCreationPolicyModel.hpp>

#include <arinc665/SupportedArinc665VersionDescription.hpp>

#include "ui_ExportMediaSetSettingsWidget.h"

namespace Arinc665Qt {

ExportMediaSetSettingsWidget::ExportMediaSetSettingsWidget(
  QWidget * const parent ) :
  QGroupBox{ parent },
  ui{ std::make_unique< Ui::ExportMediaSetSettingsWidget >() },
  fileCreationPolicyModel{ std::make_unique< FileCreationPolicyModel >( this ) }
{
  ui->setupUi( this );

  connect(
    ui->arinc665Version,
    QOverload< int >::of( &QComboBox::currentIndexChanged ),
    this,
    &ExportMediaSetSettingsWidget::arinc665VersionIndexSelected );

  for ( const auto &versionInfo : Arinc665::SupportedArinc665VersionDescription::instance() )
  {
    ui->arinc665Version->addItem( QString::fromStdString( versionInfo.name ) );
  }

  connect(
    ui->batchFileCreation,
    QOverload< int >::of( &QComboBox::currentIndexChanged ),
    this,
    &ExportMediaSetSettingsWidget::createBatchFilesIndexSelected );
  connect(
    ui->loadHeaderCreation,
    QOverload< int >::of( &QComboBox::currentIndexChanged ),
    this,
    &ExportMediaSetSettingsWidget::createLoadHeadersFilesIndexSelected );

  ui->batchFileCreation->setModel( fileCreationPolicyModel.get() );
  ui->loadHeaderCreation->setModel( fileCreationPolicyModel.get() );
}

ExportMediaSetSettingsWidget::~ExportMediaSetSettingsWidget() = default;

bool ExportMediaSetSettingsWidget::completed() const
{
  return true;
}

void ExportMediaSetSettingsWidget::arinc665VersionIndexSelected( int index )
{
  emit arinc665Version(
    static_cast< Arinc665::SupportedArinc665Version >( index ) );
}

void ExportMediaSetSettingsWidget::createBatchFilesIndexSelected( int index )
{
  emit createBatchFiles(
    static_cast< Arinc665::Utils::FileCreationPolicy >( index ) );
}

void ExportMediaSetSettingsWidget::createLoadHeadersFilesIndexSelected(
  int index )
{
  emit createLoadHeaderFiles(
    static_cast< Arinc665::Utils::FileCreationPolicy >( index ) );
}

}

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
 * @brief Definition of Class Arinc665Qt::OpenMediaSetManagerAction.
 **/

#include "OpenMediaSetManagerAction.hpp"

#include <arinc665_qt/media_set_manager/LoadMediaSetManagerAction.hpp>

#include <arinc665_qt/media/MediaSetsModel.hpp>

#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>
#include <QSettings>

#include <boost/exception/all.hpp>

namespace Arinc665Qt::MediaSetManager {

OpenMediaSetManagerAction::OpenMediaSetManagerAction(
  QWidget * const parent ) :
  QObject{ parent },
  selectMediaSetDirectoryDialogV{ std::make_unique< QFileDialog >( parent ) },
  loadMediaSetManagerActionV{ std::make_unique< LoadMediaSetManagerAction >( this ) }
{
  QSettings settings{};

  selectMediaSetDirectoryDialogV->setWindowTitle(
    tr( "Select ARINC 665 Media Set Manager Configuration" ) );
  selectMediaSetDirectoryDialogV->setFileMode( QFileDialog::Directory );
  selectMediaSetDirectoryDialogV->setOption( QFileDialog::ShowDirsOnly );
  if ( auto lastDir{ settings.value( "LastMediaSetManagerDirectory" ) };
    lastDir.isValid() )
  {
    selectMediaSetDirectoryDialogV->setDirectory( lastDir.toString() );
  }

  connect(
    selectMediaSetDirectoryDialogV.get(),
    &QFileDialog::rejected,
    this,
    &OpenMediaSetManagerAction::rejected );
  connect(
    selectMediaSetDirectoryDialogV.get(),
    &QFileDialog::accepted,
    this,
    &OpenMediaSetManagerAction::directorySelected );

  connect(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::mediaSetManagerLoadProgress,
    this,
    &OpenMediaSetManagerAction::mediaSetManagerLoadProgress );
  connect(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::mediaSetManagerLoaded,
    this,
    &OpenMediaSetManagerAction::mediaSetManagerLoaded );
  connect(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::failed,
    this,
    &OpenMediaSetManagerAction::failed );
}

OpenMediaSetManagerAction::~OpenMediaSetManagerAction() = default;

void OpenMediaSetManagerAction::open()
{
  selectMediaSetDirectoryDialogV->open();
}

void OpenMediaSetManagerAction::directorySelected()
{
  auto directory{ selectMediaSetDirectoryDialogV->directory() };

  QSettings settings{};

  loadMediaSetManagerActionV->mediaSetDirectory(
    directory.path().toStdString() );
  loadMediaSetManagerActionV->checkMediaSetIntegrity(
    settings.value( "CheckIntegrityOnStartup", true ).toBool() );

  settings.setValue( "LastMediaSetManagerDirectory", directory.path() );

  progressDialogV = new QProgressDialog{};
  progressDialogV->setWindowTitle( tr( "Load ARINC 665 Media Set Manager" ) );
  progressDialogV->setAutoReset( false );

  connect(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::mediaSetManagerLoaded,
    [this]()
    {
      progressDialogV->reset();
      progressDialogV->deleteLater();
    } );
  connect(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::failed,
    [this]()
    {
      progressDialogV->reset();
      progressDialogV->deleteLater();
    } );

  progressDialogV->show();

  // call start asynchronous
  // NOLINTBEGIN(clang-analyzer-cplusplus.NewDeleteLeaks)
  QMetaObject::invokeMethod(
    loadMediaSetManagerActionV.get(),
    &LoadMediaSetManagerAction::start,
    Qt::ConnectionType::QueuedConnection );
  // NOLINTEND(clang-analyzer-cplusplus.NewDeleteLeaks)
}

void OpenMediaSetManagerAction::mediaSetManagerLoadProgress(
  size_t currentMediaSet,
  size_t numberOfMediaSets,
  const std::string &partNumber )
{
  if ( nullptr != progressDialogV )
  {
    progressDialogV->setMaximum( static_cast< int >( numberOfMediaSets ) );
    progressDialogV->setValue( static_cast< int >( currentMediaSet ) );
    progressDialogV->setLabelText(
      QString{ tr( "Media Set <tt>%1</tt" ) }
        .arg( QString::fromStdString( partNumber ) ) );
  }
}

}

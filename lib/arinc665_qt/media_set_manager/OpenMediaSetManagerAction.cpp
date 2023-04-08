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

#include <arinc665_qt/media_set_manager/MediaSetManagerDialog.hpp>

#include <arinc665_qt/media/MediaSetsModel.hpp>

#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>
#include <QSettings>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt::MediaSetManager {

OpenMediaSetManagerAction::OpenMediaSetManagerAction(
  QWidget * const parent ) :
  QObject{ parent },
  selectMediaSetDirectoryDialogV{ std::make_unique< QFileDialog >( parent ) }
{
  QSettings settings{};

  selectMediaSetDirectoryDialogV->setWindowTitle(
    tr( "Select ARINC 665 Media Set Manager Configuration" ) );
  selectMediaSetDirectoryDialogV->setFileMode( QFileDialog::Directory );
  selectMediaSetDirectoryDialogV->setOption( QFileDialog::ShowDirsOnly );
  if ( auto lastDir{ settings.value( "MediaSetManagerDirectory" ) };
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
}

OpenMediaSetManagerAction::~OpenMediaSetManagerAction() = default;

const Arinc665::Utils::MediaSetManagerPtr& OpenMediaSetManagerAction::mediaSetManager() const
{
  return mediaSetManagerV;
}

void OpenMediaSetManagerAction::open()
{
  selectMediaSetDirectoryDialogV->open();
}

void OpenMediaSetManagerAction::directorySelected()
{
  auto directory{ selectMediaSetDirectoryDialogV->directory() };

  try
  {
    QSettings settings{};

    mediaSetManagerV = Arinc665::Utils::MediaSetManager::loadOrCreate(
      directory.path().toStdString(),
      settings.value( "CheckIntegrityOnStartup", true ).toBool() );

    settings.setValue( "MediaSetManagerDirectory", directory.path() );

    emit accepted();
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Cannot open Media Set Manager" ),
      QString{ tr(
        "<b>Media Set Directory:</b><br/><i>%1</i><br/>"
           "<b>Error:</b><br/><tt>%2</tt>" ) }
        .arg( directory.path(), QString::fromStdString( info ) ) );

    emit rejected();
  }
}

}

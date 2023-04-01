/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class
 *   Arinc665Qt::MediaSetManager::RemoveMediaSetController.
 **/

#include "RemoveMediaSetController.hpp"

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/FilesystemMediaSetRemover.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt::MediaSetManager {

RemoveMediaSetController::RemoveMediaSetController( QWidget * const parent  ) :
  QObject{ parent }
{
}

RemoveMediaSetController::~RemoveMediaSetController() = default;

void RemoveMediaSetController::start(
  const Arinc665::Utils::MediaSetManagerPtr &mediaSetManager,
  Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  const auto answer{ QMessageBox::question(
      nullptr,
      tr( "Remove ARINC 665 Media Set" ),
      tr( "Remove Media Set" ) ) };

  switch ( answer )
  {
    case QMessageBox::StandardButton::Yes:
      break;

    case QMessageBox::StandardButton::No:
    default:
      emit finished();
      return;
  }

  try
  {
    auto mediaSetPaths{
      mediaSetManager->deregisterMediaSet( mediaSet->partNumber() ) };
    mediaSetPaths.first = mediaSetManager->directory() / mediaSetPaths.first;

    auto remover{ Arinc665::Utils::FilesystemMediaSetRemover::create() };
    assert( remover );

    remover->mediaSetPaths( mediaSetPaths );
    ( *remover )();
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during removal" ),
      QString::fromStdString( info ) );
  }

  mediaSetManager->saveConfiguration();

  emit finished();
}

}

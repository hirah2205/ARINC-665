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
 *   Arinc665Qt::MediaSetManager::LoadMediaSetManagerAction.
 **/

#include "LoadMediaSetManagerAction.hpp"

#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

namespace Arinc665Qt::MediaSetManager {

LoadMediaSetManagerAction::LoadMediaSetManagerAction( QObject * const parent ) :
  threadV{ new QThread{ parent } }
{
  qRegisterMetaType< size_t >( "size_t" );
  qRegisterMetaType< std::string >( "std::string" );
  qRegisterMetaType< Arinc665::Utils::MediaSetManagerPtr >(
    "Arinc665::Utils::MediaSetManagerPtr" );

  moveToThread( threadV );

  connect( threadV, &QThread::finished, threadV, &QThread::deleteLater );

  threadV->start();
}

LoadMediaSetManagerAction::~LoadMediaSetManagerAction()
{
  threadV->quit();
}

void LoadMediaSetManagerAction::mediaSetDirectory(
  std::filesystem::path mediaSetDirectory )
{
  mediaSetDirectoryV = std::move( mediaSetDirectory );
}

void LoadMediaSetManagerAction::checkMediaSetIntegrity(
  const bool checkMediaSetIntegrity )
{
  checkMediaSetIntegrityV = checkMediaSetIntegrity;
}

void LoadMediaSetManagerAction::start()
{
  try
  {
    auto mediaSetManager{ Arinc665::Utils::MediaSetManager::loadOrCreate(
      mediaSetDirectoryV,
      checkMediaSetIntegrityV,
      std::bind_front(
        &LoadMediaSetManagerAction::loadProgress,
        this ) ) };

    emit mediaSetManagerLoaded( mediaSetManager );
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
        .arg(
          QString::fromStdString( mediaSetDirectoryV.string() ),
          QString::fromStdString( info ) ) );

    emit failed();
  }
  catch ( const std::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Cannot open Media Set Manager" ),
      QString{ tr(
       "<b>Media Set Directory:</b><br/><i>%1</i><br/>"
       "<b>Error:</b><br/><tt>%2</tt>" ) }
        .arg(
          QString::fromStdString( mediaSetDirectoryV.string() ),
          QString::fromStdString( info ) ) );

    emit failed();
  }
}

void LoadMediaSetManagerAction::loadProgress(
  const std::pair< std::size_t, std::size_t > mediaSet,
  const std::string_view partNumber,
  [[maybe_unused]] const std::pair< Arinc665::MediumNumber, Arinc665::MediumNumber > medium )
{
  emit mediaSetManagerLoadProgress(
    mediaSet.first,
    mediaSet.second,
    std::string{ partNumber } );
}

}

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
 * @brief ARINC 665 Media Set Manager QT Application.
 **/

#include <arinc665_qt/resources/Resources.hpp>

#include <arinc665_qt/media_set_manager/OpenMediaSetManagerAction.hpp>
#include <arinc665_qt/media_set_manager/MediaSetManagerDialog.hpp>

#include <arinc665/utils/MediaSetManager.hpp>

#include <qt_icon_resources/QtIconResources.hpp>

#include <helper/Logger.hpp>

#include <QApplication>
#include <QIcon>

#include <boost/exception/diagnostic_information.hpp>

#include <cstdlib>
#include <iostream>

/**
 * @brief Application Entry Point.
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Application exit status.
 **/
int main( int argc, char * argv[] );

int main( int argc, char * argv[] )
try
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  QtIconResources::initialise();
  Arinc665Qt::Resources::initialise();

  QApplication application{ argc, argv };
  QApplication::setApplicationDisplayName(
    QObject::tr( "ARINC 665 Media Set Manager" ) );
  QApplication::setOrganizationName( "Thomas Vogt" );
  QApplication::setOrganizationDomain( "thomas-vogt.de" );
  QApplication::setWindowIcon( QIcon{ ":/fa/solid/database.svg" } );

  Arinc665Qt::MediaSetManager::MediaSetManagerDialog mediaSetManagerDialog{};

  Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction
    mediaSetManagerAction{};

  QObject::connect(
    &mediaSetManagerAction,
    &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::rejected,
    &application,
    &QApplication::quit );
  QObject::connect(
    &mediaSetManagerAction,
    &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::mediaSetManagerLoaded,
    [&]( const Arinc665::Utils::MediaSetManagerPtr &mediaSetManager )
    {
      mediaSetManagerDialog.setWindowTitle(
        QString::fromStdString( mediaSetManager->directory().string() ) );

      mediaSetManagerDialog.mediaSetManger( mediaSetManager );

      mediaSetManagerDialog.show();
    } );

  mediaSetManagerAction.open();

  return QApplication::exec();
}
catch ( const boost::exception &e )
{
  BOOST_LOG_TRIVIAL( error ) << boost::diagnostic_information( e );
  return EXIT_FAILURE;
}
catch ( const std::exception &e )
{
  BOOST_LOG_TRIVIAL( error ) << e.what();
  return EXIT_FAILURE;
}
catch ( ... )
{
  BOOST_LOG_TRIVIAL( error ) << "Unknown exception";
  return EXIT_FAILURE;
}

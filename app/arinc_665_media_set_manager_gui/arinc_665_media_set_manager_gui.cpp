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
 * @brief ARINC 665 Media Set Manager Qt Application.
 **/

#include <arinc_665_qt/resources/Resources.hpp>

#include <arinc_665_qt/media_set_manager/OpenMediaSetManagerAction.hpp>
#include <arinc_665_qt/media_set_manager/MediaSetManagerWindow.hpp>

#include <arinc_665/utils/MediaSetManager.hpp>

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
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  try
  {
    QtIconResources::initialise();
    Arinc665Qt::Resources::initialise();

    QApplication application{ argc, argv };
    QApplication::setApplicationDisplayName(
      QObject::tr( "ARINC 665 Media Set Manager" ) );
    QApplication::setOrganizationName( "Thomas Vogt" );
    QApplication::setOrganizationDomain( "thomas-vogt.de" );
    QApplication::setWindowIcon( QIcon{ ":/fa/solid/database.svg" } );

    Arinc665Qt::MediaSetManager::MediaSetManagerWindow mediaSetManagerWindow{};

    Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction
      mediaSetManagerAction{};

    QObject::connect(
      &mediaSetManagerAction,
      &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::rejected,
      &application,
      &QApplication::quit );
    QObject::connect(
      &mediaSetManagerAction,
      &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::
        mediaSetManagerLoaded,
      [&]( const Arinc665::Utils::MediaSetManagerPtr &mediaSetManager )
      {
        mediaSetManagerWindow.setWindowTitle(
          QString::fromStdString( mediaSetManager->directory().string() ) );

        mediaSetManagerWindow.mediaSetManger( mediaSetManager );

        mediaSetManagerWindow.show();
      } );

    mediaSetManagerAction.open();

    return QApplication::exec();
  }
  catch ( const boost::exception &e )
  {
    std::cerr
      << "Error: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( const std::exception &e )
  {
    std::cerr
      << "Error: "
      << boost::diagnostic_information( e ) << "\n";
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cerr << "Unknown exception occurred\n";
    return EXIT_FAILURE;
  }
}

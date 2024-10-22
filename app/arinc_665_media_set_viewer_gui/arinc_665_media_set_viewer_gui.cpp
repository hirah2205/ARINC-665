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
 * @brief ARINC 665 Media Set Viewer Qt Application.
 **/

#include <arinc_665_qt/resources/Resources.hpp>

#include <arinc_665_qt/media_set_viewer/MediaSetViewerWindow.hpp>

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
      QObject::tr( "ARINC 665 Media Set Viewer" ) );
    QApplication::setOrganizationName( "Thomas Vogt" );
    QApplication::setOrganizationDomain( "thomas-vogt.de" );
    QApplication::setWindowIcon( QIcon{ ":/fa/solid/eye.svg" } );

    Arinc665Qt::MediaSetViewerWindow mediaSetViewerWindow{};
    emit mediaSetViewerWindow.show();

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

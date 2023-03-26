/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set Viewer QT Application.
 **/

/**
 * @dir
 * @brief ARINC 665 Media Set Viewer QT Application.
 **/

#include <arinc665_qt/resources/Resources.hpp>

#include <arinc665_qt/view_media_set/ViewMediaSetAction.hpp>

#include <helper/Logger.hpp>

#include <QApplication>
#include <QIcon>

#include <cstdlib>

/**
 * @brief Entry Point of Application.
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   Arguments
 *
 * @return Success state of this operation.
 **/
int main( int argc, char * argv[] );

int main( int argc, char * argv[] )
{
  try
  {
    Helper::initLogging( Helper::Severity::info );

    Arinc665Qt::Resources::initialise();

    QApplication application{ argc, argv };
    QApplication::setApplicationName( "ARINC 665 Media Set Viewer" );
    QApplication::setApplicationDisplayName( "ARINC 665 Media Set Viewer" );
    QApplication::setWindowIcon( QIcon(
      ":/media_set_manager/arinc665_media_set_manager.svg" ) );

    Arinc665Qt::ViewMediaSetAction mediaSetController{};

    QObject::connect(
      &mediaSetController,
      &Arinc665Qt::ViewMediaSetAction::finished,
      &application,
      &QApplication::quit );

    emit mediaSetController.start();

    return QApplication::exec();
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
}

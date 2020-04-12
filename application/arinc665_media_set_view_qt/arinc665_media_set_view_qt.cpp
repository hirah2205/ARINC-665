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

#include <arinc665_qt/MediaSetController.hpp>

#include <helper/Logger.hpp>

#include <QApplication>

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
int main( int argc, char * argv[]);

int main( int argc, char * argv[])
{
  try
  {
    Helper::initLogging( Helper::Severity::info);

    QApplication application{ argc, argv };
    application.setApplicationName( "ARINC 665 Media Set Viewer");
    application.setApplicationDisplayName( "ARINC 665 Media Set Viewer");

    Arinc665Qt::MediaSetController mediaSetController{};

    QObject::connect(
      &mediaSetController,
      SIGNAL( finished()),
      &application,
      SLOT( quit()));

    emit mediaSetController.start();

    return application.exec();
  }
  catch ( std::exception &e)
  {
    BOOST_LOG_TRIVIAL( error) << e.what();
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    BOOST_LOG_TRIVIAL( error) << "Unknown exception";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

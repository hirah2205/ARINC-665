/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set Decompiler QT Application.
 **/

#include <arinc665_qt/resources/Resources.hpp>

#include <arinc665_qt/decompile_media_set/DecompileMediaSetAction.hpp>

#include <helper/Logger.hpp>

#include <QApplication>
#include <QIcon>

#include <boost/exception/diagnostic_information.hpp>

#include <cstdlib>
#include <iostream>

/**
 * @brief Program entry point
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
try
{
  BOOST_LOG_FUNCTION()

  Helper::initLogging( Helper::Severity::info );

  Arinc665Qt::Resources::initialise();

  QApplication application{ argc, argv };
  QApplication::setApplicationName( "ARINC 665 Media Set Decompiler" );
  QApplication::setApplicationDisplayName( "ARINC 665 Media Set Decompiler" );
  QApplication::setWindowIcon( QIcon(
    ":/media_set_manager/arinc665_media_set_manager.svg" ) );

  Arinc665Qt::DecompileMediaSetAction decompileMediaSetAction{};

  QObject::connect(
    &decompileMediaSetAction,
    &Arinc665Qt::DecompileMediaSetAction::finished,
    &application,
    &QApplication::quit );

  return QApplication::exec();
}
catch ( boost::exception &e )
{
  std::cerr << boost::diagnostic_information( e ) << "\n";
  return EXIT_FAILURE;
}
catch ( std::exception &e )
{
  std::cerr << e.what() << "\n";
  return EXIT_FAILURE;
}
catch ( ...)
{
  std::cerr << "Unknown exception\n";
  return EXIT_FAILURE;
}

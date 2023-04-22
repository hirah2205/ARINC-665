/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief ARINC 665 Media Set Compiler QT Application.
 **/

#include <arinc665_qt/resources/Resources.hpp>

#include <arinc665_qt/compile_media_set/CompileMediaSetWizard.hpp>

#include <qt_icon_resources/QtIconResources.hpp>

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

  QtIconResources::initialise();
  Arinc665Qt::Resources::initialise();

  QApplication application{ argc, argv };
  QApplication::setApplicationDisplayName(
    QObject::tr( "ARINC 665 Media Set Compiler" ) );
  QApplication::setOrganizationName( "Thomas Vogt" );
  QApplication::setOrganizationDomain( "thomas-vogt.de" );
  QApplication::setWindowIcon( QIcon(
    ":/media_set_manager/arinc665_media_set_compile.svg" ) );

  Arinc665Qt::CompileMediaSetWizard compileMediaSetWizard{};
  compileMediaSetWizard.show();

  return QApplication::exec();
}
catch ( const boost::exception &e )
{
  std::cerr << boost::diagnostic_information( e ) << "\n";
  return EXIT_FAILURE;
}
catch ( const std::exception &e )
{
  std::cerr << e.what() << "\n";
  return EXIT_FAILURE;
}
catch ( ... )
{
  std::cerr << "Unknown exception\n";
  return EXIT_FAILURE;
}

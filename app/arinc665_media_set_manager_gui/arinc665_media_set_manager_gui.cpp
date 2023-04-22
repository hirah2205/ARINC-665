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
  QApplication::setWindowIcon( QIcon(
    ":/media_set_manager/arinc665_media_set_manager.svg" ) );

  Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction
    mediaSetManagerAction{};

  QObject::connect(
    &mediaSetManagerAction,
    &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::rejected,
    &application,
    &QApplication::quit );
  QObject::connect(
    &mediaSetManagerAction,
    &Arinc665Qt::MediaSetManager::OpenMediaSetManagerAction::accepted,
    [&]()
    {
      Arinc665Qt::MediaSetManager::MediaSetManagerDialog * dialog{
        new Arinc665Qt::MediaSetManager::MediaSetManagerDialog(
          mediaSetManagerAction.mediaSetManager() ) };

      dialog->setWindowTitle(
        QString::fromStdString(
          mediaSetManagerAction.mediaSetManager()->directory().string() ) );

      Arinc665Qt::MediaSetManager::MediaSetManagerDialog::connect(
        dialog,
        &Arinc665Qt::MediaSetManager::MediaSetManagerDialog::finished,
        dialog,
        &Arinc665Qt::MediaSetManager::MediaSetManagerDialog::deleteLater );
      Arinc665Qt::MediaSetManager::MediaSetManagerDialog::connect(
        dialog,
        &Arinc665Qt::MediaSetManager::MediaSetManagerDialog::finished,
        &application,
        &QApplication::quit );

      dialog->show();
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

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetAction.
 **/

#include "ImportMediaSetAction.hpp"

#include <arinc665_qt/import_media_set/ImportMediaSetWizard.hpp>

#include <arinc665_qt/MediaPathsModel.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/utils/FilesystemMediaSetCopier.hpp>
#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/Utils.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

ImportMediaSetAction::ImportMediaSetAction(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  QObject{ parent },
  wizardV{ std::make_unique< ImportMediaSetWizard >( parent ) },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  copierV{ Arinc665::Utils::FilesystemMediaSetCopier::create() },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >() }
{
  wizardV->mediaPathsModel( mediaPathsModelV.get() );

  connect(
    wizardV.get(),
    &ImportMediaSetWizard::checkFileIntegrity,
    this,
    &ImportMediaSetAction::checkFileIntegrity );
  connect(
    wizardV.get(),
    &ImportMediaSetWizard::start,
    this,
    &ImportMediaSetAction::start );
  connect(
    wizardV.get(),
    &ImportMediaSetWizard::finished,
    this,
    &ImportMediaSetAction::finished );

  wizardV->show();
}

ImportMediaSetAction::~ImportMediaSetAction() = default;

void ImportMediaSetAction::checkFileIntegrity( bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
}

void ImportMediaSetAction::start()
{
  BOOST_LOG_FUNCTION()

  try
  {
    const auto mediaInformation{
      Arinc665::Utils::getMediumInformation(
        mediaPathsModelV->mediaPaths().begin()->second ) };

    std::filesystem::path mediaSetPath{ mediaInformation->partNumber };

    if ( std::filesystem::exists( mediaSetManagerV->directory() / mediaSetPath ) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set Directory already exist" } );
    }

    // create media set directory
    std::filesystem::create_directories(
      mediaSetManagerV->directory() / mediaSetPath );

    copierV
      ->mediaPaths( mediaPathsModelV->mediaPaths() )
      .mediaSetBasePath(
        mediaSetManagerV->directory() / mediaInformation->partNumber );

    mediaSetManagerV->registerMediaSet(
      { mediaInformation->partNumber, mediaPathsModelV->mediaPaths() },
      checkFileIntegrityV );
  }
  catch ( const boost::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString::fromStdString( info ) );
    return;
  }
  catch ( const std::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString::fromStdString( info ) );
    return;
  }
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetManager::ImportMediaSetWizard.
 **/

#include "ImportMediaSetWizard.hpp"

#include "ui_ImportMediaSetWizard.h"

#include <arinc665_qt/MediaPathsModel.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/utils/FilesystemMediaSetCopier.hpp>
#include <arinc665/utils/MediaSetManager.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt::MediaSetManager {

ImportMediaSetWizard::ImportMediaSetWizard(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
  QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetWizard >() },
  mediaSetManagerV{ std::move( mediaSetManager ) },
  copierV{ Arinc665::Utils::FilesystemMediaSetCopier::create() },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >() }
{
  ui->setupUi( this );
  ui->settings->setCommitPage( true );
  ui->settings->mediaPathsModel( mediaPathsModelV.get() );

  connect(
    this,
    &QWizard::currentIdChanged,
    this,
    &ImportMediaSetWizard::pageChanged );
  connect(
    ui->settings,
    &ImportMediaSetSettingsPage::checkFileIntegrity,
    this,
    &ImportMediaSetWizard::checkFileIntegrity );
}

ImportMediaSetWizard::~ImportMediaSetWizard() = default;

void ImportMediaSetWizard::pageChanged( const int id )
{
  if ( ui->settings->nextId() == id )
  {
    importMediaSet();
  }
}

void ImportMediaSetWizard::checkFileIntegrity( bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
}

void ImportMediaSetWizard::importMediaSet()
{
  BOOST_LOG_FUNCTION()

  try
  {
    const auto mediaInformation{
      Arinc665::Utils::getMediumInformation(
        mediaPathsModelV->mediaPaths().begin()->second ) };

    if ( !mediaInformation )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Directory contains no medium" } );
    }

    if ( mediaSetManagerV->hasMediaSet( mediaInformation->partNumber) )
    {
      BOOST_THROW_EXCEPTION(
        Arinc665::Arinc665Exception()
        << Helper::AdditionalInfo{ "Media Set already exist" } );
    }

    assert( copierV );
    copierV
      ->mediaPaths( mediaPathsModelV->mediaPaths() )
      .outputBasePath( mediaSetManagerV->directory() )
      .mediaSetName( mediaInformation->partNumber );

    auto copyResult{ ( * copierV )() };

    mediaSetManagerV->registerMediaSet( copyResult, checkFileIntegrityV );
    mediaSetManagerV->saveConfiguration();
  }
  catch ( const boost::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString{ tr( "Error:<br/><tt>%1</tt>") }
        .arg( QString::fromStdString( info ) ) );
    return;
  }
  catch ( const std::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during import" ),
      QString{ tr( "Error:<br/><tt>%1</tt>") }
        .arg( QString::fromStdString( info ) ) );
    return;
  }
}

}

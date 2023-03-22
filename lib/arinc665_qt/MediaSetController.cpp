/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetController.
 **/

#include "MediaSetController.hpp"

#include <arinc665_qt/media/MediaSetModel.hpp>
#include <arinc665_qt/media/LoadsModel.hpp>
#include <arinc665_qt/media/BatchesModel.hpp>
#include <arinc665_qt/MediaSetDialog.hpp>
#include <arinc665_qt/Arinc665QtLogger.hpp>

#include <arinc665/utils/FilesystemMediaSetImporter.hpp>
#include <arinc665/utils/Utils.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/files/MediaSetInformation.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/MediumNumber.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Exception.hpp>

#include <helper_qt/String.hpp>
#include <QMessageBox>

#include <boost/log/trivial.hpp>
#include <boost/exception/all.hpp>

namespace Arinc665Qt {

MediaSetController::MediaSetController( QWidget * const parent ):
  QObject{ parent },
  mediaSetModel{ std::make_unique< Media::MediaSetModel >( this ) },
  selectDirectoryDialog{ std::make_unique< QFileDialog>(
    parent,
    tr( "Select ARINC 665 Medium" ) ) },
  mediaSetDialog{ std::make_unique< MediaSetDialog >( parent ) }
{
  selectDirectoryDialog->setFileMode( QFileDialog::Directory );
  selectDirectoryDialog->setOption( QFileDialog::ShowDirsOnly );

  connect(
    selectDirectoryDialog.get(),
    &QFileDialog::rejected,
    this,
    &MediaSetController::finished );
  connect(
    selectDirectoryDialog.get(),
    &QFileDialog::directoryEntered,
    this,
    &MediaSetController::directoryEntered );
  connect(
    selectDirectoryDialog.get(),
    &QFileDialog::accepted,
    this,
    &MediaSetController::directorySelected );

  mediaSetDialog->mediaSetModel( mediaSetModel.get() );

  connect(
    mediaSetDialog.get(),
    &MediaSetDialog::finished,
    this,
    &MediaSetController::finished );
}

MediaSetController::~MediaSetController() = default;

void MediaSetController::start()
{
  selectDirectoryDialog->open();
}

void MediaSetController::start( Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  mediaSetModel->root( mediaSet );

  // Set window title
  mediaSetDialog->setWindowTitle(
    HelperQt::toQString( mediaSet->partNumber() ) );

  mediaSetDialog->open();
}

void MediaSetController::directoryEntered( const QString &path )
{
  const auto mediumInformation{
    Arinc665::Utils::getMediumInformation( path.toStdString() ) };
  if ( !mediumInformation )
  {
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::info )
      << path.toStdString() << " is not a ARIN 665 medium";
    return;
  }

  BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::info )
    << path.toStdString() << " Medium "
    << mediumInformation->partNumber << " "
    << mediumInformation->mediaSequenceNumber << "/"
    << mediumInformation->numberOfMediaSetMembers;
}

void MediaSetController::directorySelected()
{
  try
  {
    auto importer{ Arinc665::Utils::FilesystemMediaSetImporter::create() };

    //! @todo At the moment only single medium media sets are supported
    importer
      ->mediaPaths( { {
        Arinc665::MediumNumber{ 1 },
        selectDirectoryDialog->directory().absolutePath().toStdString() } } );

    const auto &[ mediaSet, checkValues ]{ (*importer)() };

    mediaSetModel->root( mediaSet );

    // Set window title
    mediaSetDialog->setWindowTitle(
      selectDirectoryDialog->directory().absolutePath() );

    // Open Dialog
    mediaSetDialog->open();
  }
  catch ( Arinc665::Arinc665Exception &e )
  {
    std::string const * const info =
      boost::get_error_info< Helper::AdditionalInfo>( e );

    QString description{};

    if ( nullptr == info )
    {
      description = e.what();
    }
    else
    {
      description = QString::fromStdString( *info );
    }

    BOOST_LOG_TRIVIAL( error ) << boost::diagnostic_information( e, true );

    QMessageBox::critical(
      nullptr,
      tr( "Load Media Set" ),
      tr( "Error loading Media Set: " ) + description );

    emit finished();
    return;
  }
}

}

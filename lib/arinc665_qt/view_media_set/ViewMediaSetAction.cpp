/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetAction.
 **/

#include "ViewMediaSetAction.hpp"

#include <arinc665_qt/view_media_set/ViewMediaSetDialog.hpp>

#include <arinc665_qt/media/MediaSetModel.hpp>
#include <arinc665_qt/media/LoadsModel.hpp>
#include <arinc665_qt/media/BatchesModel.hpp>

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

ViewMediaSetAction::ViewMediaSetAction( QWidget * const parent ):
  QObject{ parent },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) },
  selectDirectoryDialogV{ std::make_unique< QFileDialog>(
    parent,
    tr( "Select ARINC 665 Medium" ) ) },
  viewMediaSetDialogV{ std::make_unique< ViewMediaSetDialog >( parent ) }
{
  selectDirectoryDialogV->setFileMode( QFileDialog::Directory );
  selectDirectoryDialogV->setOption( QFileDialog::ShowDirsOnly );

  connect(
    selectDirectoryDialogV.get(),
    &QFileDialog::rejected,
    this,
    &ViewMediaSetAction::finished );
  connect(
    selectDirectoryDialogV.get(),
    &QFileDialog::directoryEntered,
    this,
    &ViewMediaSetAction::directoryEntered );
  connect(
    selectDirectoryDialogV.get(),
    &QFileDialog::accepted,
    this,
    &ViewMediaSetAction::directorySelected );

  viewMediaSetDialogV->mediaSetModel( mediaSetModelV.get() );

  connect(
    viewMediaSetDialogV.get(),
    &ViewMediaSetDialog::finished,
    this,
    &ViewMediaSetAction::finished );
}

ViewMediaSetAction::~ViewMediaSetAction() = default;

void ViewMediaSetAction::start()
{
  selectDirectoryDialogV->open();
}

void ViewMediaSetAction::start( Arinc665::Media::ConstMediaSetPtr mediaSet )
{
  mediaSetModelV->root( mediaSet );

  // Set window title
  viewMediaSetDialogV->setWindowTitle(
    HelperQt::toQString( mediaSet->partNumber() ) );

  viewMediaSetDialogV->open();
}

void ViewMediaSetAction::directoryEntered( const QString &path )
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

void ViewMediaSetAction::directorySelected()
{
  try
  {
    auto importer{ Arinc665::Utils::FilesystemMediaSetImporter::create() };

    //! @todo At the moment only single medium media sets are supported
    importer
      ->mediaPaths( { {
        Arinc665::MediumNumber{ 1 },
        selectDirectoryDialogV->directory().absolutePath().toStdString() } } );

    const auto &[ mediaSet, checkValues ]{ (*importer)() };

    mediaSetModelV->root( mediaSet );

    // Set window title
    viewMediaSetDialogV->setWindowTitle(
      selectDirectoryDialogV->directory().absolutePath() );

    // Open Dialog
    viewMediaSetDialogV->open();
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

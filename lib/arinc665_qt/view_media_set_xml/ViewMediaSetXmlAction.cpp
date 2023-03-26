/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetXmlAction.
 **/

#include "ViewMediaSetXmlAction.hpp"

#include <arinc665_qt/view_media_set_xml/ViewMediaSetXmlDialog.hpp>

#include <arinc665_qt/media/MediaSetModel.hpp>
#include <arinc665_qt/media/LoadsModel.hpp>
#include <arinc665_qt/media/BatchesModel.hpp>

#include <arinc665_qt/Arinc665QtLogger.hpp>
#include <arinc665_qt/FilePathMappingModel.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>

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

ViewMediaSetXmlAction::ViewMediaSetXmlAction( QWidget * const parent ):
  QObject{ parent },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) },
  filePathMappingModelV{ std::make_unique< FilePathMappingModel >( this ) },
  selectMediaSetXmlDialogV{ std::make_unique< QFileDialog>( parent ) },
  viewMediaSetXmlDialogV{ std::make_unique< ViewMediaSetXmlDialog >( parent ) }
{
  selectMediaSetXmlDialogV->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectMediaSetXmlDialogV->setNameFilter(tr("ARINC 665 Media Set XML (*.xml)" ) );
  selectMediaSetXmlDialogV->setFileMode( QFileDialog::FileMode::ExistingFile );

  connect(
    selectMediaSetXmlDialogV.get(),
    &QFileDialog::rejected,
    this,
    &ViewMediaSetXmlAction::finished );
  connect(
    selectMediaSetXmlDialogV.get(),
    &QFileDialog::fileSelected,
    this,
    &ViewMediaSetXmlAction::fileSelected );

  viewMediaSetXmlDialogV->mediaSetModel( mediaSetModelV.get() );
  viewMediaSetXmlDialogV->filePathMappingModel( filePathMappingModelV.get() );

  connect(
    viewMediaSetXmlDialogV.get(),
    &ViewMediaSetXmlDialog::finished,
    this,
    &ViewMediaSetXmlAction::finished );
}

ViewMediaSetXmlAction::~ViewMediaSetXmlAction() = default;

void ViewMediaSetXmlAction::start()
{
  selectMediaSetXmlDialogV->open();
}

void ViewMediaSetXmlAction::start(
  Arinc665::Media::ConstMediaSetPtr mediaSet,
  Arinc665::Utils::FilePathMapping filePathMapping )
{
  mediaSetModelV->root( mediaSet );
  filePathMappingModelV->filePathMapping( std::move( filePathMapping ) );

  // Set window title
  viewMediaSetXmlDialogV->setWindowTitle(
    HelperQt::toQString( mediaSet->partNumber() ) );

  viewMediaSetXmlDialogV->open();
}

void ViewMediaSetXmlAction::fileSelected( const QString &file )
{
  try
  {
    auto [ mediaSet, filePathMapping ]{
      Arinc665::Utils::Arinc665Xml_load( file.toStdString() ) };

    mediaSetModelV->root( mediaSet );
    filePathMappingModelV->filePathMapping( std::move( filePathMapping ) );

    // Set window title
    viewMediaSetXmlDialogV->setWindowTitle(
      HelperQt::toQString( mediaSet->partNumber() ) );

    // Open Dialog
    viewMediaSetXmlDialogV->open();
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
      tr( "Load Media Set XML" ),
      tr( "Error loading Media Set: " ) + description );

    emit finished();
    return;
  }
}

}

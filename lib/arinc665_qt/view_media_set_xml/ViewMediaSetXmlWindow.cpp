/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ViewMediaSetXmlWindow.
 **/

#include "ViewMediaSetXmlWindow.hpp"

#include "ui_ViewMediaSetXmlWindow.h"

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665_qt/Arinc665QtLogger.hpp>
#include <arinc665_qt/FilePathMappingModel.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <helper/Exception.hpp>

#include <helper_qt/String.hpp>

#include <QMessageBox>

#include <boost/log/trivial.hpp>
#include <boost/exception/all.hpp>

namespace Arinc665Qt {

ViewMediaSetXmlWindow::ViewMediaSetXmlWindow( QWidget * const parent ):
  QMainWindow{ parent },
  ui{ std::make_unique< Ui::ViewMediaSetXmlWindow >() },
  selectMediaSetXmlDialogV{ std::make_unique< QFileDialog>( this ) },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) },
  filePathMappingModelV{ std::make_unique< FilePathMappingModel >( this ) }
{
  ui->setupUi( this );

  selectMediaSetXmlDialogV->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectMediaSetXmlDialogV->setNameFilter(tr("ARINC 665 Media Set XML (*.xml)" ) );
  selectMediaSetXmlDialogV->setFileMode( QFileDialog::FileMode::ExistingFile );

  ui->mediaSetView->mediaSetModel( mediaSetModelV.get() );
  ui->filePathMapping->setModel( filePathMappingModelV.get() );

  connect(
    ui->actionOpenMediaSetXml,
    &QAction::triggered,
    selectMediaSetXmlDialogV.get(),
    &QFileDialog::exec );
  connect(
    selectMediaSetXmlDialogV.get(),
    &QFileDialog::fileSelected,
    this,
    &ViewMediaSetXmlWindow::xmlFileSelected );
}

ViewMediaSetXmlWindow::~ViewMediaSetXmlWindow() = default;

void ViewMediaSetXmlWindow::xmlFileSelected( const QString &file )
{
  try
  {
    auto [ mediaSet, filePathMapping ]{
      Arinc665::Utils::Arinc665Xml_load( file.toStdString() ) };

    mediaSetModelV->root( mediaSet );
    filePathMappingModelV->filePathMapping( std::move( filePathMapping ) );

    // Set window title
    setWindowTitle( HelperQt::toQString( mediaSet->partNumber() ) );
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
    return;
  }
}

}

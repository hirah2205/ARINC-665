/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::DecompileMediaSetWindow.
 **/

#include "DecompileMediaSetWindow.hpp"

#include "ui_DecompileMediaSetWindow.h"

#include <arinc665_qt/decompile_media_set/DecompileMediaSetWizard.hpp>

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665_qt/MediaPathsModel.hpp>
#include <arinc665_qt/FilePathMappingModel.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Logger.hpp>
#include <helper/Exception.hpp>

#include <helper_qt/String.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

DecompileMediaSetWindow::DecompileMediaSetWindow( QWidget * const parent ) :
  QMainWindow{ parent },
  ui{ std::make_unique< Ui::DecompileMediaSetWindow >() },
  wizardV{ std::make_unique< DecompileMediaSetWizard >( this ) },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >( this ) },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) },
  filePathMappingModelV{ std::make_unique< FilePathMappingModel >( this ) },
  sortedFilePathMappingModelV{ std::make_unique< QSortFilterProxyModel >( this ) },
  selectXmlFileDialog{ std::make_unique< QFileDialog >( this ) }
{
  ui->setupUi( this );

  ui->mediaSetView->mediaSetModel( mediaSetModelV.get() );
  ui->filePathMapping->setModel( sortedFilePathMappingModelV.get() );
  sortedFilePathMappingModelV->setSourceModel( filePathMappingModelV.get() );
  sortedFilePathMappingModelV->sort(
    static_cast< int >( FilePathMappingModel::Columns::MediaSetFile ) );

  wizardV->mediaPathsModel( mediaPathsModelV.get() );

  selectXmlFileDialog->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectXmlFileDialog->setNameFilter(tr( "ARINC 665 Media Set XML (*.xml)" ) );
  selectXmlFileDialog->setFileMode( QFileDialog::FileMode::AnyFile );
  selectXmlFileDialog->setAcceptMode( QFileDialog::AcceptMode::AcceptSave );
  selectXmlFileDialog->setDefaultSuffix( "xml" );

  connect(
    ui->actionDecompileMediaSet,
    &QAction::triggered,
    wizardV.get(),
    &QWizard::show );
  connect(
    wizardV.get(),
    &DecompileMediaSetWizard::checkFileIntegrity,
    this,
    &DecompileMediaSetWindow::checkFileIntegrity );
  connect(
    wizardV.get(),
    &DecompileMediaSetWizard::accepted,
    this,
    &DecompileMediaSetWindow::startDecompilation );

  connect(
    ui->actionSaveMediaSetXml,
    &QAction::triggered,
    selectXmlFileDialog.get(),
    &QFileDialog::exec );
  connect(
    selectXmlFileDialog.get(),
    &QFileDialog::fileSelected,
    this,
    &DecompileMediaSetWindow::saveXmlFile );

  ui->actionSaveMediaSetXml->setEnabled( false );
}

DecompileMediaSetWindow::~DecompileMediaSetWindow() = default;

void DecompileMediaSetWindow::checkFileIntegrity( bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
}

void DecompileMediaSetWindow::startDecompilation()
{
  BOOST_LOG_FUNCTION()

  try
  {
    auto decompiler{ Arinc665::Utils::FilesystemMediaSetDecompiler::create() };

    decompiler
      ->checkFileIntegrity( checkFileIntegrityV )
      .mediaPaths( mediaPathsModelV->mediaPaths() );

    auto [ mediaSet, checkValues ]{ ( *decompiler )() };

    Arinc665::Utils::FilePathMapping fileMapping{};

    // iterate over all files to add file-mapping
    for ( const auto &file : mediaSet->recursiveFiles() )
    {
      std::filesystem::path filePath(
        mediaPathsModelV->mediaPaths().at( file->effectiveMediumNumber() )
          / file->path().relative_path() );

      fileMapping.try_emplace( file, std::move( filePath ) );
    }

    mediaSetV = std::move( mediaSet );
    mediaSetModelV->root( mediaSetV );
    filePathMappingModelV->filePathMapping( std::move( fileMapping ) );

    selectXmlFileDialog->selectFile(
      HelperQt::toQString( mediaSetV->partNumber() ) + ".xml" );

    setWindowTitle( HelperQt::toQString( mediaSetV->partNumber() ) );
    ui->actionSaveMediaSetXml->setEnabled( true );
  }
  catch ( const boost::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during decompilation" ),
      QString::fromStdString( info ) );
    return;
  }
  catch ( const std::exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during decompilation" ),
      QString::fromStdString( info ) );
    return;
  }
}

void DecompileMediaSetWindow::saveXmlFile( const QString &file )
{
  if ( !mediaSetV )
  {
    return;
  }

  try
  {
    // export to ARINC 665 XML file
    Arinc665::Utils::Arinc665Xml_save(
      *mediaSetV,
      filePathMappingModelV->filePathMapping(),
      file.toStdString() );
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Save Media Set XML" ),
      QString::fromStdString( info ) );
    return;
  }
}

}

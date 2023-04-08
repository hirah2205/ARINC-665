/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaSetViewWindow.
 **/

#include "MediaSetViewWindow.hpp"

#include "ui_MediaSetViewWindow.h"

#include <arinc665_qt/decompile_media_set/DecompileMediaSetWizard.hpp>

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665_qt/Arinc665QtLogger.hpp>
#include <arinc665_qt/FilePathMappingModel.hpp>
#include <arinc665_qt/MediaPathsModel.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetDecompiler.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>

#include <helper/Exception.hpp>

#include <helper_qt/String.hpp>

#include <QMessageBox>

#include <boost/log/trivial.hpp>
#include <boost/exception/all.hpp>

namespace Arinc665Qt {

MediaSetViewWindow::MediaSetViewWindow( QWidget * const parent ):
  QMainWindow{ parent },
  ui{ std::make_unique< Ui::MediaSetViewWindow >() },
  decompileMediaSetWizardV{ std::make_unique< DecompileMediaSetWizard >( this ) },
  selectLoadMediaSetXmlDialogV{ std::make_unique< QFileDialog>( this ) },
  selectSaveMediaSetXmlDialogV{ std::make_unique< QFileDialog>( this ) },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >( this ) },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >( this ) },
  filePathMappingModelV{ std::make_unique< FilePathMappingModel >( this ) },
  sortedFilePathMappingModelV{ std::make_unique< QSortFilterProxyModel >( this ) }
{
  ui->setupUi( this );

  ui->mediaSetView->mediaSetModel( mediaSetModelV.get() );
  sortedFilePathMappingModelV->setSourceModel( filePathMappingModelV.get() );
  sortedFilePathMappingModelV->sort(
    static_cast< int >( FilePathMappingModel::Columns::MediaSetFile ) );
  ui->filePathMapping->setModel( sortedFilePathMappingModelV.get() );

  decompileMediaSetWizardV->mediaPathsModel( mediaPathsModelV.get() );
  connect(
    ui->actionDecompileMediaSet,
    &QAction::triggered,
    decompileMediaSetWizardV.get(),
    &QWizard::open );
  connect(
    decompileMediaSetWizardV.get(),
    &DecompileMediaSetWizard::checkFileIntegrity,
    this,
    &MediaSetViewWindow::checkFileIntegrity );
  connect(
    decompileMediaSetWizardV.get(),
    &DecompileMediaSetWizard::accepted,
    this,
    &MediaSetViewWindow::startMediaSetDecompilation );


  selectLoadMediaSetXmlDialogV->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectLoadMediaSetXmlDialogV->setNameFilter(tr( "ARINC 665 Media Set XML (*.xml)" ) );
  selectLoadMediaSetXmlDialogV->setFileMode( QFileDialog::FileMode::ExistingFile );

  connect(
    ui->actionOpenMediaSetXml,
    &QAction::triggered,
    selectLoadMediaSetXmlDialogV.get(),
    qOverload<>( &QFileDialog::open ) );
  connect(
    selectLoadMediaSetXmlDialogV.get(),
    &QFileDialog::fileSelected,
    this,
    &MediaSetViewWindow::loadXmlFile );


  selectSaveMediaSetXmlDialogV->setWindowTitle( tr( "Select ARINC 665 Media Set XML" ) );
  selectSaveMediaSetXmlDialogV->setNameFilter(tr( "ARINC 665 Media Set XML (*.xml)" ) );
  selectSaveMediaSetXmlDialogV->setFileMode( QFileDialog::FileMode::AnyFile );
  selectSaveMediaSetXmlDialogV->setAcceptMode( QFileDialog::AcceptMode::AcceptSave );
  selectSaveMediaSetXmlDialogV->setDefaultSuffix( "xml" );

  connect(
    ui->actionSaveMediaSetXml,
    &QAction::triggered,
    selectSaveMediaSetXmlDialogV.get(),
    qOverload<>( &QFileDialog::open ) );
  connect(
    selectSaveMediaSetXmlDialogV.get(),
    &QFileDialog::fileSelected,
    this,
    &MediaSetViewWindow::saveXmlFile );

  ui->actionSaveMediaSetXml->setEnabled( false );
}

MediaSetViewWindow::~MediaSetViewWindow() = default;

void MediaSetViewWindow::checkFileIntegrity( bool checkFileIntegrity )
{
  checkFileIntegrityV = checkFileIntegrity;
}

void MediaSetViewWindow::startMediaSetDecompilation()
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

    auto partNumber{ mediaSet->partNumber() };

    mediaSetModelV->root( std::move( mediaSet ) );
    filePathMappingModelV->filePathMapping( std::move( fileMapping ) );

    selectSaveMediaSetXmlDialogV->selectFile(
      HelperQt::toQString( partNumber ) + ".xml" );

    setWindowTitle( HelperQt::toQString( partNumber ) );

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

  decompileMediaSetWizardV->restart();
}

void MediaSetViewWindow::loadXmlFile( const QString &file )
{
  try
  {
    auto [ mediaSet, filePathMapping ]{
      Arinc665::Utils::Arinc665Xml_load( file.toStdString() ) };

    mediaSetModelV->root( mediaSet );
    filePathMappingModelV->filePathMapping( std::move( filePathMapping ) );

    // Set window title
    setWindowTitle( HelperQt::toQString( mediaSet->partNumber() ) );

    ui->actionSaveMediaSetXml->setEnabled( true );
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

void MediaSetViewWindow::saveXmlFile( const QString &file )
{
  if ( !mediaSetModelV->root() )
  {
    return;
  }

  try
  {
    // export to ARINC 665 XML file
    Arinc665::Utils::Arinc665Xml_save(
      *mediaSetModelV->root()->mediaSet(),
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

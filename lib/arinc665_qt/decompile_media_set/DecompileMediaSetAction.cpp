/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::DecompileMediaSetAction.
 **/

#include "DecompileMediaSetAction.hpp"

#include <arinc665_qt/decompile_media_set/DecompileMediaSetWizard.hpp>

#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665_qt/MediaPathsModel.hpp>
#include <arinc665_qt/FilePathMappingModel.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/File.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetImporter.hpp>

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

DecompileMediaSetAction::DecompileMediaSetAction( QWidget * const parent ) :
  wizardV{ std::make_unique< DecompileMediaSetWizard >( parent ) },
  importerV{ Arinc665::Utils::FilesystemMediaSetImporter::create() },
  mediaPathsModelV{ std::make_unique< MediaPathsModel >() },
  mediaSetModelV{ std::make_unique< Media::MediaSetModel >() },
  filePathMappingModelV{ std::make_unique< FilePathMappingModel >() }
{
  wizardV->mediaPathsModel( mediaPathsModelV.get() );
  wizardV->mediaSetModel( mediaSetModelV.get() );
  wizardV->filePathMappingModel( filePathMappingModelV.get() );

  wizardV->show();

  connect(
    wizardV.get(),
    &DecompileMediaSetWizard::start,
    this,
    &DecompileMediaSetAction::start );
  connect(
    wizardV.get(),
    &DecompileMediaSetWizard::save,
    this,
    &DecompileMediaSetAction::save );
  connect(
    wizardV.get(),
    &DecompileMediaSetWizard::finished,
    this,
    &DecompileMediaSetAction::finished );
}

DecompileMediaSetAction::~DecompileMediaSetAction() = default;

void DecompileMediaSetAction::start()
{
  try
  {
    importerV
      ->mediaPaths( mediaPathsModelV->mediaPaths() )
      .checkFileIntegrity( true );

    auto [ mediaSet, checkValues ]{ ( *importerV )() };

    Arinc665::Utils::FilePathMapping fileMapping{};

    // iterate over all files to add file-mapping
    for ( const auto &file : mediaSet->recursiveFiles() )
    {
      std::filesystem::path filePath(
        mediaPathsModelV->mediaPaths().at( file->effectiveMediumNumber() )
          / file->path().relative_path() );

      fileMapping.try_emplace( file, std::move( filePath ) );
    }

    mediaSetV = mediaSet;
    mediaSetModelV->root( mediaSet );
    filePathMappingModelV->filePathMapping( std::move( fileMapping ) );
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during decompilation" ),
      QString::fromStdString( info ) );
    return;
  }
}

void DecompileMediaSetAction::save( std::filesystem::path xmlFile )
{
  try
  {
    // export to ARINC 665 XML file
    Arinc665::Utils::Arinc665Xml_save(
      *mediaSetV,
      filePathMappingModelV->filePathMapping(),
      xmlFile );
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

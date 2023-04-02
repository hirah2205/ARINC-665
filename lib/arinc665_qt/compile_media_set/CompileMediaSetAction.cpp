/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::CompileMediaSetController.
 **/

#include "CompileMediaSetAction.hpp"

#include <arinc665_qt/compile_media_set/CompileMediaSetWizard.hpp>

#include <arinc665/media/MediaSet.hpp>

#include <arinc665/utils/Arinc665Xml.hpp>
#include <arinc665/utils/FilesystemMediaSetCompiler.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/MediumNumber.hpp>

#include <QMessageBox>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665Qt {

CompileMediaSetAction::CompileMediaSetAction( QWidget * const parent ) :
  wizardV{ std::make_unique< CompileMediaSetWizard >( parent ) },
  compilerV{ Arinc665::Utils::FilesystemMediaSetCompiler::create() }
{
  assert( compilerV );

  connect(
    wizardV.get(),
    &CompileMediaSetWizard::xmlFile,
    this,
    &CompileMediaSetAction::xmlFile );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::inputDirectory,
    this,
    &CompileMediaSetAction::inputDirectory );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::arinc665Version,
    this,
    &CompileMediaSetAction::arinc665Version );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::createBatchFiles,
    this,
    &CompileMediaSetAction::createBatchFiles );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::createLoadHeaderFiles,
    this,
    &CompileMediaSetAction::createLoadHeaderFiles );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::outputDirectory,
    this,
    &CompileMediaSetAction::outputDirectory );

  connect(
    wizardV.get(),
    &CompileMediaSetWizard::start,
    this,
    &CompileMediaSetAction::start );
  connect(
    wizardV.get(),
    &CompileMediaSetWizard::finished,
    this,
    &CompileMediaSetAction::finished );

  wizardV->show();
}

CompileMediaSetAction::~CompileMediaSetAction() = default;

void CompileMediaSetAction::xmlFile( std::filesystem::path xmlFile )
{
  xmlFileV = std::move( xmlFile );
}

void CompileMediaSetAction::inputDirectory( std::filesystem::path directory )
{
  compilerV->sourceBasePath( std::move( directory ) );
}

void CompileMediaSetAction::arinc665Version(
  Arinc665::SupportedArinc665Version version )
{
  compilerV->arinc665Version( version );
}

void CompileMediaSetAction::createBatchFiles(
  Arinc665::Utils::FileCreationPolicy createBatchFiles )
{
  compilerV->createBatchFiles( createBatchFiles );
}

void CompileMediaSetAction::createLoadHeaderFiles(
  Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles )
{
  compilerV->createLoadHeaderFiles( createLoadHeaderFiles );
}

void CompileMediaSetAction::outputDirectory( std::filesystem::path directory )
{
  outputDirectoryV = std::move( directory );
}

void CompileMediaSetAction::start()
{
  try
  {
    // load ARINC 665 XML file
    auto [ mediaSet, fileMapping ] =
      Arinc665::Utils::Arinc665Xml_load( xmlFileV );

    auto outputPath{ outputDirectoryV / mediaSet->partNumber() };

    if ( std::filesystem::exists( outputPath ) )
    {
      QMessageBox::critical(
        nullptr,
        tr( "Error during compilation" ),
        tr( "Media Set directory must not exist" ) );
      return;
    }

    std::filesystem::create_directory( outputPath );

    compilerV
      ->mediaSet( std::move( mediaSet ) )
      .filePathMapping( std::move( fileMapping ) )
      .mediaSetBasePath( std::move( outputPath ) );
    assert( compilerV );
    ( *compilerV )();
  }
  catch ( const Arinc665::Arinc665Exception &e )
  {
    const auto info{ boost::diagnostic_information( e ) };

    QMessageBox::critical(
      nullptr,
      tr( "Error during compilation" ),
      QString::fromStdString( info ) );
    return;
  }
}

}

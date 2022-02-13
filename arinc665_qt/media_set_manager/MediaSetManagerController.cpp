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

#include "MediaSetManagerController.hpp"

#include <arinc665_qt/media/MediaSetsModel.hpp>
#include <arinc665_qt/media_set_manager/MediaSetManagerDialog.hpp>


#include <arinc665/utils/MediaSetManager.hpp>
#include <arinc665/utils/MediaSetConfiguration.hpp>

#include <boost/property_tree/json_parser.hpp>

namespace Arinc665Qt::MediaSetManager {

MediaSetManagerController::MediaSetManagerController(
  QWidget * const parent ) :
  QObject{ parent },
  mediaSetsModelV{ std::make_unique< Media::MediaSetsModel >( this ) },
  mediaSetManagerDialogV{ std::make_unique< MediaSetManagerDialog >( parent ) },
  selectMediaSetConfigDialogV{ std::make_unique< QFileDialog >(
    parent,
    tr( "Select ARINC 665 Media Set Manager Configuration" ) ) },
  mediaSetManagerV{}
{
  mediaSetManagerDialogV->mediaSetsModel( mediaSetsModelV.get() );
  connect(
    mediaSetManagerDialogV.get(),
    &MediaSetManagerDialog::finished,
    this,
    &MediaSetManagerController::finished );

  connect(
    selectMediaSetConfigDialogV.get(),
    &QFileDialog::rejected,
    this,
    &MediaSetManagerController::finished );
  connect(
    selectMediaSetConfigDialogV.get(),
    &QFileDialog::accepted,
    this,
    &MediaSetManagerController::configurationSelected );
}

MediaSetManagerController::~MediaSetManagerController() = default;

void MediaSetManagerController::start()
{
  selectMediaSetConfigDialogV->open();
}

void MediaSetManagerController::start(
  Arinc665::Utils::MediaSetManagerPtr mediaSetManager )
{
  mediaSetManagerV = std::move( mediaSetManager );
  mediaSetsModelV->mediaSets( mediaSetManagerV->mediaSets() );
  mediaSetManagerDialogV->open();
}

void MediaSetManagerController::configurationSelected()
{
  auto file{ selectMediaSetConfigDialogV->selectedFiles().first() };

  std::filesystem::path jsonFile{ file.toStdString() };
  auto baseDir{ jsonFile.parent_path() };

  boost::property_tree::ptree ptree{};
  boost::property_tree::read_json( jsonFile.string(), ptree );
  Arinc665::Utils::MediaSetConfiguration config{ ptree };

  emit start( Arinc665::Utils::MediaSetManager::instance( baseDir, config, true )  );
}

void MediaSetManagerController::view()
{

}

void MediaSetManagerController::remove()
{

}

}

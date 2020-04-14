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

#include <arinc665_qt/MediaSetDialog.hpp>
#include <arinc665_qt/media/MediaSetModel.hpp>

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/utils/Arinc665Utils.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <QMessageBox>

#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>

namespace Arinc665Qt {

MediaSetController::MediaSetController( QWidget * const parent):
  QObject{ parent},
  mediaSetModel{ new Media::MediaSetModel( parent)},
  selectDirectoryDialog{ new QFileDialog(
    parent,
    tr( "Select ARINC 665 Medium"))},
  mediaSetDialog{ new MediaSetDialog( parent)}
{
  selectDirectoryDialog->setFileMode( QFileDialog::Directory);
  selectDirectoryDialog->setOption( QFileDialog::ShowDirsOnly);

  connect( selectDirectoryDialog, SIGNAL( rejected()), this, SIGNAL( finished()));
  connect( selectDirectoryDialog, SIGNAL( accepted()), this, SLOT( directorySelected()));

  mediaSetDialog->model( mediaSetModel);

  connect(
    mediaSetDialog,
    SIGNAL( finished( int)),
    this,
    SIGNAL( finished()));
}

MediaSetController::~MediaSetController()
{
  mediaSetDialog->deleteLater();
  selectDirectoryDialog->deleteLater();
  mediaSetModel->deleteLater();
}

void MediaSetController::start()
{
  selectDirectoryDialog->open();
}

void MediaSetController::directorySelected()
{
  try
  {
    auto importer{ Arinc665::Utils::Arinc665Utils::arinc665Importer(
      std::bind(
        &MediaSetController::loadFile,
        this,
        std::placeholders::_1,
        std::placeholders::_2))};

    auto mediaSet{ importer()};

    mediaSetModel->setMediaSet( mediaSet);

    // Set window title
    mediaSetDialog->setWindowTitle(
      selectDirectoryDialog->directory().absolutePath());
  }
  catch (Arinc665::Arinc665Exception &e)
  {
    std::string const * info = boost::get_error_info< Helper::AdditionalInfo>( e);

    QString description{};

    if (nullptr == info)
    {
      description = e.what();
    }
    else
    {
      description = QString::fromStdString( *info);
    }

    BOOST_LOG_TRIVIAL( error) << boost::diagnostic_information( e, true);

    QMessageBox::critical(
      nullptr,
      tr( "Load Media Set"),
      tr( "Error loading Media Set: ") + description);

    emit finished();
    return;
  }

  mediaSetDialog->show();
}

Arinc665::File::RawFile MediaSetController::loadFile(
  const uint8_t mediumNumber,
  const std::filesystem::path &path)
{
  if (1U != mediumNumber)
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "Multi Medium Media Sets not supported"));
  }

  auto filePath{
    selectDirectoryDialog->directory().absolutePath().toStdString() / path.relative_path()};

  if ( !std::filesystem::is_regular_file( filePath))
  {
    BOOST_THROW_EXCEPTION(
      Arinc665::Arinc665Exception()
        << boost::errinfo_file_name( filePath.string())
        << Helper::AdditionalInfo( "File not found"));
  }

  Arinc665::File::RawFile data( std::filesystem::file_size( filePath));

  std::ifstream file(
    filePath.string().c_str(),
    std::ifstream::binary | std::ifstream::in);

  if ( !file.is_open())
  {
    BOOST_THROW_EXCEPTION(Arinc665::Arinc665Exception()
      << Helper::AdditionalInfo( "Error opening files"));
  }

  // read the data to the buffer
  file.read( (char*) &data.at( 0), data.size());

  // return the buffer
  return data;
}

}

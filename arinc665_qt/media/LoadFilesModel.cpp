/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::LoadFilesModel.
 **/

#include "LoadFilesModel.hpp"

#include <arinc665/media/File.hpp>

#include <helper_qt/String.hpp>

namespace Arinc665Qt::Media {

LoadFilesModel::LoadFilesModel( QObject * const parent ) :
  QAbstractTableModel{ parent }
{
}

LoadFilesModel::~LoadFilesModel() = default;

int LoadFilesModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( loadFilesV.size() );
}

int LoadFilesModel::columnCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( Columns::ColumnsCount );
}

QVariant LoadFilesModel::data(
  const QModelIndex &index,
  const int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  // out of range access
  if ( static_cast< size_t>( index.row() ) >= loadFilesV.size() )
  {
    return {};
  }

  auto loadFile{ std::next( loadFilesV.begin(), index.row() ) };

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::Name:
          return HelperQt::toQString( std::get< 0 >( *loadFile )->name() );

        case Columns::PartNumber:
          return HelperQt::toQString( std::get< 1 >( *loadFile ) );

        default:
          return {};
      }

    case Qt::ItemDataRole::ToolTipRole:
    case Qt::ItemDataRole::TextAlignmentRole:
    default:
      return {};
  }
}

QVariant LoadFilesModel::headerData(
  int section,
  ::Qt::Orientation orientation,
  int role ) const
{
  if ( role != Qt::DisplayRole )
  {
    return {};
  }

  if ( orientation == Qt::Vertical )
  {
    return section;
  }

  switch ( static_cast< Columns>( section ) )
  {
    case Columns::Name:
      return QString{ tr( "Name" ) };

    case Columns::PartNumber:
      return QString{ tr( "Part Number" ) };

    default:
      return {};
  }
}

void LoadFilesModel::loadFiles( const Arinc665::Media::ConstLoadFiles &loadFiles )
{
  beginResetModel();
  loadFilesV = loadFiles;
  endResetModel();
}

}

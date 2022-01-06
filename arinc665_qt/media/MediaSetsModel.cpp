/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::MediaSetsModel.
 **/

#include "MediaSetsModel.hpp"

#include <arinc665/media/MediaSet.hpp>

#include <helper_qt/String.hpp>

namespace Arinc665Qt::Media {

MediaSetsModel::MediaSetsModel( QObject * const parent ) :
  QAbstractTableModel{ parent }
{
}

MediaSetsModel::~MediaSetsModel() = default;

int MediaSetsModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( mediaSetsV.size() );
}

int MediaSetsModel::columnCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( Columns::ColumnsCount );

}

QVariant MediaSetsModel::data(
  const QModelIndex &index,
  const int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  // out of range access
  if ( static_cast< size_t>( index.row() ) >= mediaSetsV.size() )
  {
    return {};
  }

  auto mediaSet{ std::next( mediaSetsV.begin(), index.row() ) };

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::PartNumber:
          return HelperQt::toQString( mediaSet->get()->partNumber() );

        default:
          return {};
      }

    case Qt::ItemDataRole::ToolTipRole:
    case Qt::ItemDataRole::TextAlignmentRole:
    default:
      return {};
  }
}

QVariant MediaSetsModel::headerData(
  const int section,
  const ::Qt::Orientation orientation,
  const int role ) const
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
    case Columns::PartNumber:
      return QString{ tr( "Part Number" ) };

    default:
      return {};
  }
}

Arinc665::Media::ConstMediaSetPtr MediaSetsModel::mediaSet(
  const QModelIndex &index ) const
{
  return {};
}

void MediaSetsModel::mediaSets(
  const Arinc665::Media::ConstMediaSets &mediaSets )
{
  beginResetModel();
  mediaSetsV = mediaSets;
  endResetModel();
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::BatchesModel.
 **/

#include "BatchesModel.hpp"

#include <arinc665/media/Batch.hpp>

namespace Arinc665Qt::Media {

BatchesModel::BatchesModel( QObject * const parent ) :
  QAbstractTableModel{ parent }
{
}

BatchesModel::~BatchesModel() = default;

int BatchesModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( batchesV.size() );
}

int BatchesModel::columnCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( Columns::ColumnsCount );
}

QVariant BatchesModel::data(
  const QModelIndex &index,
  const int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  // out of range access
  if ( static_cast< size_t>( index.row() ) >= batchesV.size() )
  {
    return {};
  }

  auto batch{ std::next( batchesV.begin(), index.row() ) };

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::Name:
          return QString::fromUtf8(
            batch->get()->name().data(),
            static_cast< int >( batch->get()->name().size() ) );

        case Columns::PartNumber:
          return QString::fromUtf8(
            batch->get()->partNumber().data(),
            static_cast< int >( batch->get()->partNumber().size() ) );

        case Columns::Comment:
          return QString::fromUtf8(
            batch->get()->comment().data(),
            static_cast< int >( batch->get()->comment().size() ) );

        default:
          return {};
      }

    case Qt::ItemDataRole::ToolTipRole:
    case Qt::ItemDataRole::TextAlignmentRole:
    default:
      return {};
  }
}

QVariant BatchesModel::headerData(
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

Arinc665::Media::BatchPtr BatchesModel::getBatch(
  const QModelIndex &index ) const
{
  return {};
}

void BatchesModel::setBatches( const Arinc665::Media::Batches &batches )
{
  beginResetModel();
  batchesV = batches;
  endResetModel();
}

}

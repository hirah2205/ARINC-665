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

#include <helper_qt/String.hpp>

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

  return static_cast< int >( numberOfBatches() );
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

  if ( index.row() < 0 )
  {
    return {};
  }

  auto batchPtr{ batch( batch( index.row() ) ) };

  // out of range access
  if ( !batchPtr )
  {
    return {};
  }

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::Name:
          return HelperQt::toQString( batchPtr->name() );

        case Columns::PartNumber:
          return HelperQt::toQString( batchPtr->partNumber() );

        case Columns::Comment:
          return HelperQt::toQString( batchPtr->comment() );

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
    case Columns::Name:
      return QString{ tr( "Name" ) };

    case Columns::PartNumber:
      return QString{ tr( "Part Number" ) };

    default:
      return {};
  }
}

BatchesModel::Batch BatchesModel::batch( const QModelIndex &index ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  if ( index.row() < 0 )
  {
    return {};
  }

  return batch( index.row() );
}

const BatchesModel::Batches BatchesModel::batches() const
{
  return batchesV;
}

void BatchesModel::batches( const Batches &batches )
{
  beginResetModel();
  batchesV = batches;
  endResetModel();
}

void BatchesModel::batches( Batches &&batches )
{
  beginResetModel();
  batchesV = std::move( batches );
  endResetModel();
}

size_t BatchesModel::numberOfBatches() const
{
  return std::visit(
    []( auto &batches ) { return batches.size(); },
    batchesV );
}

BatchesModel::Batch BatchesModel::batch( std::size_t index ) const
{
  if ( index >= numberOfBatches() )
  {
    return {};
  }

  return std::visit(
    [ index ]( auto &batches ) {
      auto batch{ std::next( batches.begin(), index ) };
      return Batch{ *batch };
    },
    batchesV );
}

Arinc665::Media::ConstBatchPtr BatchesModel::batch( const Batch &batch ) const
{
  return std::visit(
    []( const auto &batch ) {
      return Arinc665::Media::ConstBatchPtr { batch };
    },
    batch );
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::LoadsModel.
 **/

#include "LoadsModel.hpp"

#include <arinc665/media/Load.hpp>

namespace Arinc665Qt::Media {

LoadsModel::LoadsModel( QObject * const parent ) :
  QAbstractTableModel{ parent }
{
}

LoadsModel::~LoadsModel() = default;

int LoadsModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( loadsV.size() );
}

int LoadsModel::columnCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( Columns::ColumnsCount );
}

QVariant LoadsModel::data(
  const QModelIndex &index,
  const int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  // out of range access
  if ( static_cast< size_t>( index.row() ) >= loadsV.size() )
  {
    return {};
  }

  auto load{ std::next( loadsV.begin(), index.row() ) };

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::Name:
          return QString::fromUtf8(
            load->get()->name().data(),
            static_cast< int >( load->get()->name().size() ) );

        case Columns::PartNumber:
          return QString::fromUtf8(
            load->get()->partNumber().data(),
            static_cast< int >( load->get()->partNumber().size() ) );

        default:
          return {};
      }

    case Qt::ItemDataRole::ToolTipRole:
    case Qt::ItemDataRole::TextAlignmentRole:
    default:
      return {};
  }
}

QVariant LoadsModel::headerData(
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

Arinc665::Media::LoadPtr LoadsModel::getLoad(
  const QModelIndex &index ) const
{
  return {};
}

void LoadsModel::setLoads( const Arinc665::Media::Loads &loads )
{
  beginResetModel();
  loadsV = loads;
  endResetModel();
}

}

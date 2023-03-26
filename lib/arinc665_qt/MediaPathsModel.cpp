/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::MediaPathsModel.
 **/

#include "MediaPathsModel.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665Qt {

MediaPathsModel::MediaPathsModel( QObject * const parent ) :
  QAbstractTableModel{ parent }
{
}

MediaPathsModel::~MediaPathsModel() = default;

int MediaPathsModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( mediaPathsV.size() );
}

int MediaPathsModel::columnCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast< int >( Columns::ColumnsCount );
}

QVariant MediaPathsModel::data( const QModelIndex &index, const int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  auto medium{ std::next( mediaPathsV.begin(), index.row() ) };
  if ( mediaPathsV.end() == medium )
  {
    return {};
  }

  switch ( role )
  {
    case Qt::ItemDataRole::DisplayRole:
      switch ( static_cast< Columns>( index.column() ) )
      {
        case Columns::MediumNumber:
          return QString::fromStdString(
            static_cast< std::string >( medium->first ) ) ;

        case Columns::MediumPath:
          return QString::fromStdString( medium->second.string() );

        default:
          return {};
      }

    case Qt::ItemDataRole::ToolTipRole:
    case Qt::ItemDataRole::TextAlignmentRole:
    default:
      return {};
  }
}

QVariant MediaPathsModel::headerData(
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
    // return index as header
    return section;
  }

  switch ( static_cast< Columns>( section ) )
  {
    case Columns::MediumNumber:
      return QString{ tr( "Medium Number" ) };

    case Columns::MediumPath:
      return QString{ tr( "Medium Path" ) };

    default:
      return {};
  }
}

const Arinc665::Utils::MediaPaths& MediaPathsModel::mediaPaths() const
{
  return mediaPathsV;
}

const std::filesystem::path& MediaPathsModel::mediumPath(
  Arinc665::MediumNumber mediumNumber ) const
{
  auto medium{ mediaPathsV.find( mediumNumber ) };
  if ( mediaPathsV.end() == medium )
  {
    BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() );
  }

  return medium->second;
}

void MediaPathsModel::mediumPath(
  Arinc665::MediumNumber mediumNumber,
  std::filesystem::path path )
{
  beginResetModel();
  mediaPathsV.insert_or_assign( mediumNumber, std::move( path ) );
  endResetModel();
}

void MediaPathsModel::remove( const QModelIndex &index )
{
  if ( !index.isValid()
    || std::cmp_greater_equal( index.row(), mediaPathsV.size() ) )
  {
    return;
  }

  beginResetModel();
  auto pos{ std::next( mediaPathsV.begin(), index.row() ) };
  mediaPathsV.erase( pos );
  endResetModel();
}

}

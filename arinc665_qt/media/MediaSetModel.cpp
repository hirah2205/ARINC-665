/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::Media::MediaSetModel.
 **/

#include "MediaSetModel.hpp"

#include <arinc665_qt/Arinc665QtLogger.hpp>

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>

#include <iterator>

namespace Arinc665Qt::Media {

MediaSetModel::MediaSetModel(
  QObject * const parent,
  Arinc665::Media::MediaSetPtr mediaSet ) :
  QAbstractItemModel{ parent },
  mediaSetV{ std::move( mediaSet ) }
{
}

QModelIndex MediaSetModel::index(
  const int row,
  const int column,
  const QModelIndex &parent ) const
{
  // check if model contains valid media set
  if ( !mediaSetV)
  {
    return {};
  }

  // if parent is invalid, it is automatically the media set
  if ( !parent.isValid())
  {
    return createIndex( row, column, mediaSetV.get());
  }

  if ( parent.internalPointer() == nullptr)
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error)
      << "Invalid Model Index";

    // invalid model index
    return {};
  }

  // cast internal pointer of parent
  auto * parentBase{
    static_cast< Arinc665::Media::Base *>( parent.internalPointer())};

  switch ( parentBase->type())
  {
    case Arinc665::Media::Base::Type::MediaSet:
      // all children of media set are media (+1 because media are 1-indexed)
      return createIndex( row, column, mediaSetV->medium( row+1U ).get() );

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      auto * parentContainer{ static_cast< Arinc665::Media::ContainerEntity*>(
        parent.internalPointer())};

      if ( static_cast< size_t>( row) < parentContainer->numberOfSubDirectories())
      {
        auto dirIt{ std::next( parentContainer->subDirectories().begin(), row)};

        return createIndex( row, column, dirIt->get());
      }

      auto fileIt{std::next(
        parentContainer->files( false).begin(),
        row - static_cast< ptrdiff_t >( parentContainer->numberOfSubDirectories() ) ) };

      return createIndex( row, column, fileIt->get() );
    }

    case Arinc665::Media::Base::Type::File:
      // file has no children

    default:
      // Should not happen
      return {};
  }
}

QModelIndex MediaSetModel::parent( const QModelIndex &index ) const
{
  // invalid index has no parent
  if ( !index.isValid() )
  {
    return {};
  }

  if ( index.internalPointer() == nullptr )
  {
    // invalid model index
    return {};
  }

  auto * base{ static_cast< Arinc665::Media::Base *>( index.internalPointer() ) };

  switch ( base->type() )
  {
    case Arinc665::Media::Base::Type::MediaSet:
      // The media set has no parent
      return {};

    case Arinc665::Media::Base::Type::Medium:
     // A medium has the single media set as parent.
      return createIndex( 0, 0, base->mediaSet().get() );

    case Arinc665::Media::Base::Type::Directory:
    {
      auto * dir{ dynamic_cast< Arinc665::Media::ContainerEntity *>( base ) };

      if ( Arinc665::Media::Base::Type::Medium == dir->parent()->type() )
      {
        auto parent{ std::dynamic_pointer_cast< Arinc665::Media::Medium>(
          dir->parent() ) };

        // the medium number is the row index
        return createIndex( parent->mediumNumber()-1, 0, parent.get() );
      }

      auto grandParent( dir->parent()->parent() );

      auto subDirs( grandParent->subDirectories() );

      // find index of parent in grandparent list
      auto pos( std::find( subDirs.begin(), subDirs.end(), dir->parent() ) );

      return createIndex(
        static_cast< int >( std::distance( subDirs.begin(), pos ) ),
        0,
        dir->parent().get());
    }

    case Arinc665::Media::Base::Type::File:
    {
      auto * file{ dynamic_cast< Arinc665::Media::File *>( base) };

      if ( Arinc665::Media::Base::Type::Medium == file->parent()->type() )
      {
        auto parent( std::dynamic_pointer_cast< Arinc665::Media::Medium>(
          file->parent() ) );

        // the medium number is the row index
         return createIndex( parent->mediumNumber()-1, 0, parent.get() );
      }

      auto grandParent{ file->parent()->parent() };

      auto subDirs{ grandParent->subDirectories() };

      // find index of parent in grandparent list
      auto pos{ std::find( subDirs.begin(), subDirs.end(), file->parent() ) };

      return createIndex(
        static_cast< int >( std::distance( subDirs.begin(), pos ) ),
        0,
        file->parent().get() );
    }

    default:
      // Should not happen
      BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
        << "Invalid Container Type";
      return {};
  }
}

bool MediaSetModel::hasChildren( const QModelIndex &parent ) const
{
  // First level (media set)
  if ( !parent.isValid() )
  {
    // return true, if a media set is assigned
    return static_cast< bool>( mediaSetV );
  }

  if ( parent.internalPointer() == nullptr )
  {
    return false;
  }

  auto * base{ static_cast< Arinc665::Media::Base *>( parent.internalPointer() ) };

  switch ( base->type() )
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      // The media set has media
      auto * mediaSet{ dynamic_cast< Arinc665::Media::MediaSet *>( base ) };
      return ( mediaSet->numberOfMedia() !=0 );
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      auto * container{
        dynamic_cast< Arinc665::Media::ContainerEntity *>( base ) };
      return container->hasChildren();
    }

    case Arinc665::Media::Base::Type::File:
      // A file has no children.
      return false;

    default:
      // Should not happen
      BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
        << "Invalid Container Type";
      return false;
  }
}

int MediaSetModel::rowCount( const QModelIndex &parent ) const
{
  // First level (Media Set)
  if ( !parent.isValid() )
  {
    // return 1, if a media set is assigned
    return ( mediaSetV ? 1 : 0 );
  }

  if ( parent.internalPointer() == nullptr )
  {
    return 0;
  }

  auto * base{ static_cast< Arinc665::Media::Base *>( parent.internalPointer() ) };

  switch ( base->type() )
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      auto * mediaSet{
        dynamic_cast< Arinc665::Media::MediaSet *>( base ) };

      // The media set has media
      return mediaSet->numberOfMedia();
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      auto * container{
        dynamic_cast< Arinc665::Media::ContainerEntity *>( base ) };

      // Medium and Directories have subdirectories and files
      return static_cast< int>(
        container->numberOfSubDirectories() +
        container->numberOfFiles( false ) );
    }

    case Arinc665::Media::Base::Type::File:
      // A file has no children.
      return 0;

    default:
      // Should not happen
      BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
        << "Invalid Container Type";
      return 0;
  }
}

int MediaSetModel::columnCount( const QModelIndex & /*parent*/ ) const
{
  return static_cast< int>( Columns::Last );
}

QVariant MediaSetModel::data( const QModelIndex & index, int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  if ( role != Qt::DisplayRole )
  {
    return {};
  }

  if ( index.internalPointer() == nullptr )
  {
    return {};
  }

  auto * base{ static_cast< Arinc665::Media::Base *>( index.internalPointer() ) };

  switch ( Columns{ index.column() } )
  {
    case Columns::Name:
      switch ( base->type() )
      {
        case Arinc665::Media::Base::Type::MediaSet:
          return QString{ "Media Set" };

        case Arinc665::Media::Base::Type::Medium:
        {
          auto * medium{ dynamic_cast< Arinc665::Media::Medium *>( base)};
          return medium->mediumNumber();
        }

        case Arinc665::Media::Base::Type::Directory:
        {
          auto * directory{ dynamic_cast< Arinc665::Media::Directory *>( base)};
          return QString::fromUtf8( directory->name().data(), static_cast< int >( directory->name().length() ) );
        }

        case Arinc665::Media::Base::Type::File:
        {
          auto * file{ dynamic_cast< Arinc665::Media::File *>( base)};
          return QString::fromUtf8( file->name().data(), static_cast< int >( file->name().length() ) );
        }

        default:
          return QString{ "Invalid Type" };
      }

    case Columns::Type:
      switch ( base->type())
      {
        case Arinc665::Media::Base::Type::MediaSet:
          return QString{ "Media Set" };

        case Arinc665::Media::Base::Type::Medium:
          return QString{ "Medium" };

        case Arinc665::Media::Base::Type::Directory:
          return QString{ "Directory" };

        case Arinc665::Media::Base::Type::File:
        {
          auto * file = dynamic_cast< Arinc665::Media::File*>( base );
          switch ( file->fileType() )
          {
            case Arinc665::Media::File::FileType::RegularFile:
              return QString{ "Regular File" };

            case Arinc665::Media::File::FileType::LoadFile:
              return QString{ "Load" };

            case Arinc665::Media::File::FileType::BatchFile:
              return QString{ "Batch" };

            default:
              return QString{ "INVALID File Type" };
          }
        }
        /* no break -because inner switch always returns */

        default:
          // Should never happen
          return {};
      }
      /* no break -because inner switch always returns */

    default:
      return {};
  }
}

QVariant MediaSetModel::headerData(
  int section,
  Qt::Orientation orientation,
  int role ) const
{
  if ( orientation == Qt::Vertical )
  {
    return {};
  }

  if ( role != Qt::DisplayRole )
  {
    return {};
  }

  switch ( Columns{ section})
  {
    case Columns::Name:
      return QString{ "Name" };

    case Columns::Type:
      return QString{ "Type" };

    default:
      return {};
  }
}

Arinc665::Media::BasePtr MediaSetModel::element( const QModelIndex &index )
{
  if ( !index.isValid() )
  {
    return {};
  }

  if ( nullptr == index.internalPointer() )
  {
    return {};
  }

  return static_cast< Arinc665::Media::Base *>(
    index.internalPointer())->shared_from_this();
}

void MediaSetModel::setMediaSet( Arinc665::Media::MediaSetPtr mediaSet )
{
  if ( mediaSetV == mediaSet )
  {
    return;
  }

  beginResetModel();

  mediaSetV = std::move( mediaSet );

  endResetModel();
}

}

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

#include <helper_qt/String.hpp>

#include <QIcon>

#include <iterator>

namespace Arinc665Qt::Media {

MediaSetModel::MediaSetModel( QObject * const parent ) :
  QAbstractItemModel{ parent }
{
}

QModelIndex MediaSetModel::index(
  const int row,
  const int column,
  const QModelIndex &parent ) const
{
  // check if model contains valid Elements
  if ( !rootV )
  {
    return {};
  }

  // if parent is invalid, it is automatically the root element
  if ( !parent.isValid() )
  {
    return createIndex( row, column, (void*)rootV.get() );
  }

  // cast internal pointer of parent
  auto parentBase{ element( parent ) };

  if ( !parentBase )
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
      << "Invalid Element Pointer";
    return {};
  }

  switch ( parentBase->type() )
  {
    case Arinc665::Media::Type::MediaSet:
    {
      auto mediaSetParent{
        std::dynamic_pointer_cast< const Arinc665::Media::MediaSet >( parentBase ) };

      if ( !mediaSetParent )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Could not cast to Media Set";
        return {};
      }

      // all children of media set are media (+1 because media are 1-indexed)
      return createIndex(
        row,
        column,
        (void*)mediaSetParent->medium( row + 1U ).get() );
    }

    case Arinc665::Media::Type::Medium:
    case Arinc665::Media::Type::Directory:
    {
      auto containerParent{
        std::dynamic_pointer_cast< const Arinc665::Media::ContainerEntity >(
          parentBase ) };

      if ( !containerParent )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Could not cast to Container";
        return {};
      }

      if ( std::cmp_less( row, containerParent->numberOfSubdirectories() ) )
      {
        auto dirIt{ std::next( containerParent->subdirectories().begin(), row ) };

        return createIndex( row, column, (void*)dirIt->get() );
      }

      auto fileIt{ std::next(
        containerParent->files().begin(),
        row - static_cast< ptrdiff_t >( containerParent->numberOfSubdirectories() ) ) };

      return createIndex( row, column, (void*)fileIt->get() );
    }

    case Arinc665::Media::Type::File:
      // file has no children
      [[fallthrough]];

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
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::warning )
      << "Root Element has no parent";
    return {};
  }

  // cast internal pointer of parent
  auto base{ element( index ) };

  if ( !base )
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
      << "Invalid Element Pointer";
    return {};
  }

  switch ( base->type() )
  {
    case Arinc665::Media::Type::MediaSet:
      // Media set has no parent
      return {};

    case Arinc665::Media::Type::Medium:
      // A medium has the single media set as parent.
      return createIndex( 0, 0, (void*)base->mediaSet().get() );

    case Arinc665::Media::Type::Directory:
    {
      auto dir{
        std::dynamic_pointer_cast< const Arinc665::Media::ContainerEntity >( base ) };

      assert( dir );

      if ( Arinc665::Media::Type::Medium == dir->parent()->type() )
      {
        auto parent{ std::dynamic_pointer_cast< const Arinc665::Media::Medium >(
          dir->parent() ) };

        assert( parent );

        // the medium number is the row index
        return createIndex( parent->mediumNumber()-1, 0, (void*)parent.get() );
      }

      auto grandParent( dir->parent()->parent() );

      auto subDirs( grandParent->subdirectories() );

      // find index of parent in grandparent list
      auto pos( std::ranges::find( subDirs, dir->parent() ) );

      return createIndex(
        static_cast< int >( std::distance( subDirs.begin(), pos ) ),
        0,
        (void*)dir->parent().get() );
    }

    case Arinc665::Media::Type::File:
    {
      auto file{ std::dynamic_pointer_cast< const Arinc665::Media::File >( base) };

      if ( !file )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Could not cast to File";
        return {};
      }

      if ( Arinc665::Media::Type::Medium == file->parent()->type() )
      {
        auto parent( std::dynamic_pointer_cast< const Arinc665::Media::Medium>(
          file->parent() ) );

        assert( parent );

        // the medium number is the row index
         return createIndex( parent->mediumNumber()-1, 0, (void*)parent.get() );
      }

      auto grandParent{ file->parent()->parent() };

      auto subDirs{ grandParent->subdirectories() };

      // find index of parent in grandparent list
      auto pos{ std::ranges::find( subDirs, file->parent() ) };

      return createIndex(
        static_cast< int >( std::distance( subDirs.begin(), pos ) ),
        0,
        (void*)file->parent().get() );
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
    // return true, if an element is assigned
    return static_cast< bool>( rootV );
  }

  // cast internal pointer of parent
  auto base{ element( parent ) };

  if ( !base )
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
      << "Invalid Element Pointer";
    return {};
  }

  switch ( base->type() )
  {
    case Arinc665::Media::Type::MediaSet:
    {
      // The media set has media
      auto mediaSet{
        std::dynamic_pointer_cast< const Arinc665::Media::MediaSet >( base ) };

      if ( !mediaSet )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Invalid Cast to Media Set";
        return {};
      }

      return ( mediaSet->numberOfMedia() !=0 );
    }

    case Arinc665::Media::Type::Medium:
    case Arinc665::Media::Type::Directory:
    {
      auto container{
        std::dynamic_pointer_cast< const Arinc665::Media::ContainerEntity >( base ) };

      if ( !container )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Invalid Cast to Container";
        return {};
      }

      return container->hasChildren();
    }

    case Arinc665::Media::Type::File:
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
    // return 1, if a root element is assigned
    return ( rootV ? 1 : 0 );
  }

  // cast internal pointer of parent
  auto base{ element( parent ) };

  if ( !base )
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
      << "Invalid Element Pointer";
    return {};
  }

  switch ( base->type() )
  {
    case Arinc665::Media::Type::MediaSet:
    {
      auto mediaSet{
        std::dynamic_pointer_cast< const Arinc665::Media::MediaSet >( base ) };

      if ( !mediaSet )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Invalid Cast to Media Set";
        return {};
      }

      // The media set has media
      return mediaSet->numberOfMedia();
    }

    case Arinc665::Media::Type::Medium:
    case Arinc665::Media::Type::Directory:
    {
      auto container{
        std::dynamic_pointer_cast< const Arinc665::Media::ContainerEntity >( base ) };

      if ( !container )
      {
        // Should not happen
        BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
          << "Invalid Cast to Container";
        return {};
      }

      // Medium and Directories have subdirectories and files
      return static_cast< int>(
        container->numberOfSubdirectories() + container->numberOfFiles() );
    }

    case Arinc665::Media::Type::File:
      // A file has no children.
      return 0;

    default:
      // Should not happen
      BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
        << "Invalid Container Type";
      return 0;
  }
}

int MediaSetModel::columnCount(
  const QModelIndex & parent [[maybe_unused]] ) const
{
  return static_cast< int >( Columns::Last );
}

QVariant MediaSetModel::data( const QModelIndex & index, int role ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  // cast internal pointer of parent
  auto base{ element( index ) };

  if ( !base )
  {
    // should not happen
    return 0;
  }

  switch ( static_cast< Qt::ItemDataRole>( role ) )
  {
    case Qt::DecorationRole:
      switch ( Columns{ index.column() } )
      {
        case Columns::Name:
        {
          QIcon icon{};

          switch ( base->type() )
          {
            case Arinc665::Media::Type::MediaSet:
              icon.addFile(
                QString::fromUtf8(
                  ":/media_set/arinc665_media_set.svg" ),
                QSize{},
                QIcon::Normal,
                QIcon::Off );
              break;

            case Arinc665::Media::Type::Medium:
              icon.addFile(
                QString::fromUtf8(
                  ":/media_set/arinc665_medium.svg" ),
                QSize{},
                QIcon::Normal,
                QIcon::Off );
              break;

            case Arinc665::Media::Type::Directory:
              icon.addFile(
                QString::fromUtf8(
                  ":/media_set/arinc665_directory.svg" ),
                QSize{},
                QIcon::Normal,
                QIcon::Off );
              break;

            case Arinc665::Media::Type::File:
            {
              auto file{
                std::dynamic_pointer_cast< const Arinc665::Media::File >( base ) };

              if ( !file )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to File";
                return {};
              }

              switch ( file->fileType() )
              {
                case Arinc665::Media::FileType::RegularFile:
                  icon.addFile(
                    QString::fromUtf8(
                      ":/media_set/arinc665_file.svg" ),
                    QSize{},
                    QIcon::Normal,
                    QIcon::Off );
                  break;

                case Arinc665::Media::FileType::LoadFile:
                  icon.addFile(
                    QString::fromUtf8(
                      ":/media_set/arinc665_load.svg" ),
                    QSize{},
                    QIcon::Normal,
                    QIcon::Off );
                  break;

                case Arinc665::Media::FileType::BatchFile:
                  icon.addFile(
                    QString::fromUtf8(
                      ":/media_set/arinc665_batch.svg" ),
                    QSize{},
                    QIcon::Normal,
                    QIcon::Off );
                  break;

                default:
                  return {};
              }
            }
            break;

            default:
              return {};
          }

          return icon;
        }

        case Columns::Type:
        default:
          return {};
      }

    case Qt::EditRole:
    case Qt::DisplayRole:
      switch ( Columns{ index.column() } )
      {
        case Columns::Name:
          switch ( base->type() )
          {
            case Arinc665::Media::Type::MediaSet:
            {
              auto mediaSet{
                std::dynamic_pointer_cast< const Arinc665::Media::MediaSet >( base) };

              if ( !mediaSet )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to Media Set";
                return {};
              }

              return HelperQt::toQString( mediaSet->partNumber() );
            }

            case Arinc665::Media::Type::Medium:
            {
              auto medium{
                std::dynamic_pointer_cast< const Arinc665::Media::Medium >( base) };

              if ( !medium )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to Medium";
                return {};
              }

              return medium->mediumNumber();
            }

            case Arinc665::Media::Type::Directory:
            {
              auto directory{
                std::dynamic_pointer_cast< const Arinc665::Media::Directory >( base) };

              if ( !directory )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to Directory";
                return {};
              }

              return HelperQt::toQString( directory->name() );
            }

            case Arinc665::Media::Type::File:
            {
              auto file{
                std::dynamic_pointer_cast< const Arinc665::Media::File >( base) };

              if ( !file )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to File";
                return {};
              }

              return HelperQt::toQString( file->name() );
            }

            default:
              return QString{ "Invalid Type" };
          }

        case Columns::Type:
          switch ( base->type())
          {
            case Arinc665::Media::Type::MediaSet:
              return tr( "Media Set" );

            case Arinc665::Media::Type::Medium:
              return tr(  "Medium" );

            case Arinc665::Media::Type::Directory:
              return tr(  "Directory" );

            case Arinc665::Media::Type::File:
            {
              auto file{
                std::dynamic_pointer_cast< const Arinc665::Media::File >( base ) };

              if ( !file )
              {
                // Should not happen
                BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
                  << "Invalid Cast to File";
                return {};
              }

              switch ( file->fileType() )
              {
                case Arinc665::Media::FileType::RegularFile:
                  return tr( "Regular File" );

                case Arinc665::Media::FileType::LoadFile:
                  return tr( "Load" );

                case Arinc665::Media::FileType::BatchFile:
                  return tr( "Batch" );

                default:
                  return tr( "INVALID File Type" );
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

    default:
      return {};
  }
}

QVariant MediaSetModel::headerData(
  const int section,
  const Qt::Orientation orientation,
  const int role ) const
{
  if ( orientation == Qt::Vertical )
  {
    return {};
  }

  if ( role != Qt::DisplayRole )
  {
    return {};
  }

  switch ( Columns{ section } )
  {
    case Columns::Name:
      return tr( "Name" );

    case Columns::Type:
      return tr( "Type" );

    default:
      return {};
  }
}

Arinc665::Media::ConstBasePtr MediaSetModel::element(
  const QModelIndex &index ) const
{
  if ( !index.isValid() )
  {
    return {};
  }

  if ( nullptr == index.internalPointer() )
  {
    // Should not happen
    BOOST_LOG_SEV( Arinc665QtLogger::get(), Helper::Severity::error )
      << "Invalid Model Index";

    // empty element
    return {};
  }

  return static_cast< Arinc665::Media::Base *>(
    index.internalPointer() )->shared_from_this();
}

void MediaSetModel::root( Arinc665::Media::ConstBasePtr root )
{
  if ( rootV == root )
  {
    return;
  }

  beginResetModel();
  rootV = std::move( root );
  endResetModel();
}

}

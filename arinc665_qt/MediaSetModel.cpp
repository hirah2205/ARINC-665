/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665Qt::MediaSetModelModel.
 **/

#include "MediaSetModel.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <iterator>
#include <cassert>

namespace Arinc665Qt {

MediaSetModelModel::MediaSetModelModel(
  MediaSetPtr mediaSet,
  QObject * parent) :
  QAbstractItemModel( parent),
  mediaSet( mediaSet)
{
}

QModelIndex MediaSetModelModel::index(
  const int row,
  const int column,
  const QModelIndex &parent) const
{
  // check if model contains valid media set
  if (!mediaSet)
  {
    return QModelIndex();
  }

  // if parent is invalid, it is automatically the media set
  if ( parent == QModelIndex())
  {
    return createIndex( row, column, mediaSet.get());
  }

  if ( parent.internalPointer() == nullptr)
  {
    // invalid model index
    return QModelIndex();
  }

  // case internal pointer of parent
  Arinc665::Media::Base * parentBase(
    static_cast< Arinc665::Media::Base *>( parent.internalPointer()));

  switch (parentBase->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
      // all children of media set are medias
      return createIndex( row, column, mediaSet->getMedium( row).get());

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * parentContainer(
        static_cast< Arinc665::Media::ContainerEntity*>( parent.internalPointer()));

      if (static_cast< size_t>( row) < parentContainer->getNumberOfSubDirectories())
      {
        return createIndex( row, column, parentContainer->getSubDirectories().at( row).get());
      }

      return createIndex( row, column, parentContainer->getFiles( false).at(
        row - parentContainer->getNumberOfSubDirectories()).get());
    }

    case Arinc665::Media::Base::Type::File:
      // file has no children
      return QModelIndex();

    default:
      // Should not happen
      return QModelIndex();
  }
}

QModelIndex MediaSetModelModel::parent( const QModelIndex &index) const
{
  // invalid index has no parent
  if (!index.isValid())
  {
    return QModelIndex();
  }

  if ( index.internalPointer() == nullptr)
  {
    // invalid model index
    return QModelIndex();
  }

  Arinc665::Media::Base * base(
    static_cast< Arinc665::Media::Base *>( index.internalPointer()));

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
      // The media set has no parent
      return QModelIndex();

    case Arinc665::Media::Base::Type::Medium:
     // A medium has the single media set as parent.
      return createIndex( 0, 0, base->getMediaSet().get());

    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * dir(
        dynamic_cast< Arinc665::Media::ContainerEntity *>( base));

      if (Arinc665::Media::Base::Type::Medium == dir->getParent()->getType())
      {
        auto parent( std::dynamic_pointer_cast< Arinc665::Media::Medium>(
          dir->getParent()));

        // the medium number is the row index
        return createIndex( parent->getMediumNumber()-1, 0, parent.get());
      }

      auto grandParent( dir->getParent()->getParent());

      auto subDirs( grandParent->getSubDirectories());

      // find index of parent in grand-parent list
      auto pos( std::find( subDirs.begin(), subDirs.end(), dir->getParent()));

      return createIndex(
        std::distance( subDirs.begin(), pos),
        0,
        dir->getParent().get());
    }

    case Arinc665::Media::Base::Type::File:
    {
      Arinc665::Media::BaseFile * file(
        dynamic_cast< Arinc665::Media::BaseFile *>( base));

      if (Arinc665::Media::Base::Type::Medium == file->getParent()->getType())
      {
        auto parent( std::dynamic_pointer_cast< Arinc665::Media::Medium>(
          file->getParent()));

        // the medium number is the row index
         return createIndex( parent->getMediumNumber()-1, 0, parent.get());
      }

      auto grandParent( file->getParent()->getParent());

      auto subDirs( grandParent->getSubDirectories());

      // find index of parent in grand-parent list
      auto pos( std::find( subDirs.begin(), subDirs.end(), file->getParent()));

      return createIndex(
        std::distance( subDirs.begin(), pos),
        0,
        file->getParent().get());
    }

    default:
      // Should not happen
      return QModelIndex();
  }
}

bool MediaSetModelModel::hasChildren( const QModelIndex & parent) const
{
  // First level (media set)
  if (!parent.isValid())
  {
    // return true, if a media set is assigned
    return static_cast< bool>( mediaSet);
  }

  if( parent.internalPointer() == nullptr)
  {
    return false;
  }

  Arinc665::Media::Base * base(
    static_cast< Arinc665::Media::Base *>( parent.internalPointer()));

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      // The media set has medias
      Arinc665::Media::MediaSet * mediaSet(
        dynamic_cast< Arinc665::Media::MediaSet *>( base));
      return (mediaSet->getNumberOfMedia()!=0);
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * container(
        dynamic_cast< Arinc665::Media::ContainerEntity *>( base));
      return container->hasChildren();
    }

    case Arinc665::Media::Base::Type::File:
      // A file has no children.
      return false;

    default:
      // Should not happen
      return false;
  }
}

int MediaSetModelModel::rowCount( const QModelIndex & parent) const
{
  // First level (Message elements)
  if (!parent.isValid())
  {
    // return 1, if a media set is assigned
    return (mediaSet ? 1 : 0);
  }

  if( parent.internalPointer() == nullptr)
  {
    return 0;
  }

  Arinc665::Media::Base * base(
    static_cast< Arinc665::Media::Base *>( parent.internalPointer()));

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      // The media set has medias
      Arinc665::Media::MediaSet * mediaSet(
        dynamic_cast< Arinc665::Media::MediaSet *>( base));
      return mediaSet->getNumberOfMedia();
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * container(
        dynamic_cast< Arinc665::Media::ContainerEntity *>( base));
      return container->getNumberOfSubDirectories() +
        container->getNumberOfFiles( false);
    }

    case Arinc665::Media::Base::Type::File:
      // A file has no children.
      return 0;

    default:
      // Should not happen
      return 0;
  }
}

int MediaSetModelModel::columnCount( const QModelIndex & /*parent*/) const
{
  return 3;
}

QVariant MediaSetModelModel::data( const QModelIndex & index, int role) const
{
  if ( !index.isValid())
  {
    return QVariant();
  }

  if ( role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if( index.internalPointer() == nullptr)
  {
    return QVariant();
  }

  Arinc665::Media::Base * base(
    static_cast< Arinc665::Media::Base *>( index.internalPointer()));

  switch (index.column())
  {
    case 0:
      return QString::fromStdString( base->getName());

    case 1:
      return QString::fromStdString( base->getPartNumber());

    case 2:
      switch (base->getType())
      {
        case Arinc665::Media::Base::Type::MediaSet:
          return QString( "Media Set");

        case Arinc665::Media::Base::Type::Medium:
          return QString( "Medium");

        case Arinc665::Media::Base::Type::Directory:
          return QString( "Directory");

        case Arinc665::Media::Base::Type::File:
        {
          Arinc665::Media::BaseFile * file = dynamic_cast< Arinc665::Media::BaseFile*>( base);
          switch (file->getFileType())
          {
            case Arinc665::Media::BaseFile::FileType::RegularFile:
              return QString( "Regular File");

            case Arinc665::Media::BaseFile::FileType::LoadFile:
              return QString( "Load");

            case Arinc665::Media::BaseFile::FileType::BatchFile:
              return QString( "Batch");

            default:
              return QVariant();
          }
        }
        /* no break -because inner switch always returns */

        default:
          // Should never happen
          return QVariant();
      }
      /* no break -because inner switch always returns */

    default:
      return QVariant();
  }
}

QVariant MediaSetModelModel::headerData(
  int section,
  Qt::Orientation orientation,
  int role) const
{
  if ( orientation == Qt::Vertical)
  {
    return QVariant();
  }

  if ( role != Qt::DisplayRole)
  {
    return QVariant();
  }

  switch ( section)
  {
    case 0:
      return QString( "Name");

    case 1:
      return QString( "Part Number");

    case 2:
      return QString( "Type");

    default:
      return QVariant();
  }
}

void MediaSetModelModel::setMediaSet( MediaSetPtr mediaSet)
{
  if (this->mediaSet == mediaSet)
  {
    return;
  }

  beginResetModel();

  this->mediaSet = mediaSet;

  endResetModel();
}
}

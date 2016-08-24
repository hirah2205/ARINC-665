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
 * @brief Definition of class Arinc665Qt::QMediaSetModelModel.
 **/

#include "QMediaSetModel.hpp"

#include <arinc665/media/MediaSet.hpp>
#include <arinc665/media/Medium.hpp>
#include <arinc665/media/Directory.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <iterator>
#include <cassert>

namespace Arinc665Qt {

QMediaSetModelModel::QMediaSetModelModel(
  MediaSetPtr mediaSet,
  QObject * parent) :
  QAbstractItemModel( parent),
  mediaSet( mediaSet)
{
}

QModelIndex QMediaSetModelModel::index(
  int row,
  int column,
  const QModelIndex &parent) const
{
  // check if model contains valid media set
  if (!mediaSet)
  {
    return QModelIndex();
  }

  if ( parent == QModelIndex())
  {
    return createIndex( row, column, mediaSet.get());
  }

  assert( parent.internalPointer() != nullptr);

  Arinc665::Media::Base * base = static_cast< Arinc665::Media::Base *>( parent.internalPointer());

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
      return createIndex( row, column, mediaSet->getMedium( row).get());

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * container =
        static_cast< Arinc665::Media::ContainerEntity*>( parent.internalPointer());

      if (static_cast< size_t>( row) < container->getNumberOfSubDirectories())
      {
        return createIndex( row, column, container->getSubDirectories().at( row).get());
      }

      return createIndex( row, column, container->getFiles( false).at( row - container->getNumberOfSubDirectories()).get());
    }

    case Arinc665::Media::Base::Type::File:
      // file has no children
      return QModelIndex();

    default:
      // Should not happen
      return QModelIndex();
  }
}

QModelIndex QMediaSetModelModel::parent( const QModelIndex &index) const
{
  // invalid index has no parent
  if (!index.isValid())
  {
    return QModelIndex();
  }

  assert( index.internalPointer() != nullptr);

  Arinc665::Media::Base * base = static_cast< Arinc665::Media::Base *>( index.internalPointer());

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
      Arinc665::Media::ContainerEntity * dir = dynamic_cast< Arinc665::Media::ContainerEntity *>( base);

      if (Arinc665::Media::Base::Type::Medium == dir->getParent()->getType())
      {
        Arinc665::Media::MediumPtr parent = std::dynamic_pointer_cast< Arinc665::Media::Medium>( dir->getParent());
        return createIndex( parent->getMediumNumber()-1, 0, parent.get());
      }

      Arinc665::Media::ContainerEntityPtr grandParent( dir->getParent()->getParent());

      auto subDirs = grandParent->getSubDirectories();

      // find index of parent in grand-parent list
      auto pos = std::find( subDirs.begin(), subDirs.end(), dir->getParent());

      return createIndex(
        std::distance( subDirs.begin(), pos),
        0,
        dir->getParent().get());
    }

    case Arinc665::Media::Base::Type::File:
    {
      Arinc665::Media::BaseFile * file = dynamic_cast< Arinc665::Media::BaseFile *>( base);

      if (Arinc665::Media::Base::Type::Medium == file->getParent()->getType())
      {
        Arinc665::Media::MediumPtr parent = std::dynamic_pointer_cast< Arinc665::Media::Medium>( file->getParent());
        return createIndex( parent->getMediumNumber()-1, 0, parent.get());
      }

      Arinc665::Media::ContainerEntityPtr grandParent( file->getParent()->getParent());

      auto subDirs = grandParent->getSubDirectories();

      // find index of parent in grand-parent list
      auto pos = std::find( subDirs.begin(), subDirs.end(), file->getParent());

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

bool QMediaSetModelModel::hasChildren( const QModelIndex & parent) const
{
  // First level (Message elements)
  if (!parent.isValid())
  {
    // return true, if a media set is assigned
    return static_cast< bool>( mediaSet);
  }

  assert( parent.internalPointer() != nullptr);

  Arinc665::Media::Base * base = static_cast< Arinc665::Media::Base *>( parent.internalPointer());

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      // The media set has medias
      Arinc665::Media::MediaSet * mediaSet = dynamic_cast< Arinc665::Media::MediaSet *>( base);
      return (mediaSet->getNumberOfMedia()!=0);
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * container = dynamic_cast< Arinc665::Media::ContainerEntity *>( base);
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


int QMediaSetModelModel::rowCount( const QModelIndex & parent) const
{
  // First level (Message elements)
  if (!parent.isValid())
  {
    // return 1, if a media set is assigned
    return (mediaSet ? 1 : 0);
  }

  assert( parent.internalPointer() != nullptr);

  Arinc665::Media::Base * base = static_cast< Arinc665::Media::Base *>( parent.internalPointer());

  switch (base->getType())
  {
    case Arinc665::Media::Base::Type::MediaSet:
    {
      // The media set has medias
      Arinc665::Media::MediaSet * mediaSet = dynamic_cast< Arinc665::Media::MediaSet *>( base);
      return mediaSet->getNumberOfMedia();
    }

    case Arinc665::Media::Base::Type::Medium:
    case Arinc665::Media::Base::Type::Directory:
    {
      Arinc665::Media::ContainerEntity * container = dynamic_cast< Arinc665::Media::ContainerEntity *>( base);
      return container->getNumberOfSubDirectories() + container->getNumberOfFiles( false);
    }

    case Arinc665::Media::Base::Type::File:
      // A file has no children.
      return 0;

    default:
      // Should not happen
      return 0;
  }
}

int QMediaSetModelModel::columnCount( const QModelIndex & /*parent*/) const
{
  return 2;
}

QVariant QMediaSetModelModel::data( const QModelIndex & index, int role) const
{
  if ( !index.isValid())
  {
    return QVariant();
  }

  if ( role != Qt::DisplayRole)
  {
    return QVariant();
  }

  assert( index.internalPointer() != nullptr);

  Arinc665::Media::Base * base = static_cast< Arinc665::Media::Base *>( index.internalPointer());

  switch (index.column())
  {
    case 0:
      switch (base->getType())
      {
        case Arinc665::Media::Base::Type::MediaSet:
        {
          // The media set has medias
          Arinc665::Media::MediaSet * mediaSet = dynamic_cast< Arinc665::Media::MediaSet *>( base);
          return QString::fromStdString( mediaSet->getPartNumber());
        }

        case Arinc665::Media::Base::Type::Medium:
          return "Medium";

        case Arinc665::Media::Base::Type::Directory:
        {
          Arinc665::Media::Directory * directory = dynamic_cast< Arinc665::Media::Directory *>( base);
          return QString::fromStdString( directory->getName());
        }

        case Arinc665::Media::Base::Type::File:
        {
          Arinc665::Media::BaseFile * file = dynamic_cast< Arinc665::Media::BaseFile *>( base);
          return QString::fromStdString( file->getName());
        }

        default:
          // Should not happen
          return 0;
      }
      break;

    case 1:
      return QVariant();
      break;

    default:
      return QVariant();
  }
}

QVariant QMediaSetModelModel::headerData(
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
      return QVariant();

    default:
      return QVariant();
  }
}

void QMediaSetModelModel::setMediaSet( MediaSetPtr mediaSet)
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

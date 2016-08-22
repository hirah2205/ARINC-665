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

#include <iterator>

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
#if 0
  using Bhm::Asf::ConstAsfMessageElements;
  using Bhm::Asf::AsfMessageElement;

  // check if model contains valid message
  if (!message)
  {
    return QModelIndex();
  }

  // top-level elements directly use message - the other casts the internak pointer
  ConstAsfMessageElements elements{
    (parent == QModelIndex()) ?
      message->getChildElements() :
      getMessageElement( parent)->getChildElements()};

  // Check input data
  if ((row < 0)    || (static_cast< size_t>( row) >= elements.size()) ||
      (column < 0) || (column >= 2))
  {
    return QModelIndex();
  }

  // get the row_th element
  return createIndex(
    row,
    column,
    const_cast< void *>( static_cast< void const *>( elements.at( row).get())));
#endif
}

QModelIndex QMediaSetModelModel::parent( const QModelIndex &index) const
{
#if 0
  // invalid index has no parent
  if (!index.isValid())
  {
    return QModelIndex();
  }

  assert( getMessageElement( index));

  using Bhm::Asf::ConstAsfContainerPtr;

  ConstAsfContainerPtr parent = getMessageElement( index)->getParent();

  // parent must always be present
  assert( parent);

  ConstAsfContainerPtr grandParent = parent->getParent();

  // if element has no grand-parent, we are on top-level
  if (!grandParent)
  {
    // this must be the first message elements
    return QModelIndex();
  }

  auto elements = grandParent->getChildElements();
  // find index of parent in grand-parent list
  auto pos = std::find( elements.begin(), elements.end(), parent);

  return createIndex(
    std::distance( elements.begin(), pos),
    0,
    const_cast< void *>( static_cast< void const *>( parent.get())));
#endif
}

bool QMediaSetModelModel::hasChildren( const QModelIndex & parent) const
{
#if 0
  // First level (Message elements)
  if (!parent.isValid())
  {
    return true;
  }

  return getMessageElement( parent)->hasChilds();
#endif
}

int QMediaSetModelModel::rowCount( const QModelIndex & parent) const
{
#if 0
  if (!message)
  {
    return 0;
  }

  // First level (Message elements)
  if (!parent.isValid())
  {
    return message->getChildElements().size();
  }

  if (parent.column() != 0)
  {
    return 0;
  }

  return getMessageElement( parent)->getChildElements().size();
#endif
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
#if 0
  switch (index.column())
  {
    case 0:
      return QString::fromStdString( getMessageElement( index)->getName());

    case 1:
      return QString::fromStdString( getMessageElement( index)->getContent( true));
      break;

    default:
      return QVariant();
  }
#endif
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
#if 0
  switch ( section)
  {
    case 0:
      return QString( "Field");

    case 1:
      return QString( "Value");

    default:
      return QVariant();
  }
#endif
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

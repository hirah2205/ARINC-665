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
 * @brief Declaration of class Arinc665Qt::QMediaSetModelModel.
 **/

#ifndef ARINC665_QT_QMEDIASETMODEL_HPP
#define ARINC665_QT_QMEDIASETMODEL_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractItemModel>

namespace Arinc665Qt {

/**
 * @brief QT Adaption of ARINC 665 Media Set.
 **/
class QMediaSetModelModel: public QAbstractItemModel
{
  Q_OBJECT

  public:
    using MediaSetPtr = Arinc665::Media::MediaSetPtr;

    QMediaSetModelModel(
      MediaSetPtr mediaSet = MediaSetPtr(),
      QObject * parent = nullptr);

    virtual ~QMediaSetModelModel( void) = default;

    virtual QModelIndex index(
      int row,
      int column,
      const QModelIndex &parent) const override;

    virtual QModelIndex parent( const QModelIndex &index) const override;

    virtual bool hasChildren( const QModelIndex & parent) const override;

    virtual int rowCount( const QModelIndex & parent) const override;

    virtual int columnCount( const QModelIndex & parent) const override;

    virtual QVariant data( const QModelIndex & index, int role) const override;

    virtual QVariant headerData(
      int section,
      ::Qt::Orientation orientation,
      int role) const override;

  public slots:
    void setMediaSet( MediaSetPtr mediaSet = MediaSetPtr());

  private:
    MediaSetPtr mediaSet;
};

}

#endif

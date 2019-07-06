/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaSetModelModel.
 **/

#ifndef ARINC665_QT_MEDIASETMODEL_HPP
#define ARINC665_QT_MEDIASETMODEL_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractItemModel>

namespace Arinc665Qt {

/**
 * @brief QT Adaption of ARINC 665 Media Set.
 **/
class MediaSetModelModel: public QAbstractItemModel
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the model instance.
     *
     * @param[in] mediaSet
     *   Associated media set.
     * @param[in] parent
     *   Parent object
     **/
    MediaSetModelModel(
      Arinc665::Media::MediaSetPtr mediaSet = {},
      QObject * parent = nullptr);

    //! Default destructor
    virtual ~MediaSetModelModel() = default;

    /**
     * @brief Creates the index for the child identified by its parent and its
     *   row and column.
     *
     * @param[in] row
     * @param[in] column
     * @param[in] parent
     *
     * @return
     **/
    virtual QModelIndex index(
      int row,
      int column,
      const QModelIndex &parent) const override;

    /**
     * @brief Return the parent of the given index.
     *
     * @param index
     *
     * @return
     **/
    virtual QModelIndex parent( const QModelIndex &index) const override;

    /**
     * @brief Return if the given parent has children.
     *
     * @param parent
     *
     * @return
     **/
    virtual bool hasChildren( const QModelIndex &parent) const override;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   The parent.
     *
     * @return
     **/
    virtual int rowCount( const QModelIndex & parent) const override;

    /**
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   The parent.
     *
     * @return
     **/
    virtual int columnCount( const QModelIndex &parent) const override;

    /**
     * @brief Returns the data for the given index.
     *
     * @param index
     * @param role
     *
     * @return
     **/
    virtual QVariant data( const QModelIndex &index, int role) const override;

    /**
     * @brief Returns the header data.
     *
     * @param[in] section
     *   Section
     * @param[in] orientation
     *   Orientation
     * @param[in] role
     *   Display role.
     *
     * @return The corresponding Header Data.
     **/
    virtual QVariant headerData(
      int section,
      ::Qt::Orientation orientation,
      int role) const override;

  public slots:
    /**
     * @brief Updates the associated media set.
     *
     * @param[in] mediaSet
     *   New associated media set
     **/
    void setMediaSet( Arinc665::Media::MediaSetPtr mediaSet = {});

  private:
    //! the media set
    Arinc665::Media::MediaSetPtr mediaSet;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::MediaSetModel.
 **/

#ifndef ARINC665_QT_MEDIA_MEDIASETMODEL_HPP
#define ARINC665_QT_MEDIA_MEDIASETMODEL_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractItemModel>

namespace Arinc665Qt::Media {

/**
 * @brief QT Adaption of ARINC 665 %Media Set.
 **/
class MediaSetModel: public QAbstractItemModel
{
  Q_OBJECT

  public:
    //! Displayed Columns
    enum class Columns
    {
      Name,
      Type,

      Last
    };

    /**
     * @brief Initialises the model instance.
     *
     * @param[in] mediaSet
     *   Associated media set.
     * @param[in] parent
     *   Parent object
     **/
    explicit MediaSetModel(
      QObject * parent = nullptr,
      Arinc665::Media::MediaSetPtr mediaSet = {} );

    //! Destructor
    ~MediaSetModel() override = default;

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
    [[nodiscard]] QModelIndex index(
      int row,
      int column,
      const QModelIndex &parent ) const override;

    /**
     * @brief Return the parent of the given index.
     *
     * @param index
     *
     * @return
     **/
    [[nodiscard]] QModelIndex parent( const QModelIndex &index ) const override;

    /**
     * @brief Return if the given parent has children.
     *
     * @param parent
     *
     * @return
     **/
    [[nodiscard]] bool hasChildren( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   The parent.
     *
     * @return
     **/
    [[nodiscard]] int rowCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   The parent.
     *
     * @return
     **/
    [[nodiscard]] int columnCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the data for the given index.
     *
     * @param index
     * @param role
     *
     * @return
     **/
    [[nodiscard]] QVariant data(
      const QModelIndex &index,
      int role ) const override;

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
     * @return Corresponding Header Data.
     **/
    [[nodiscard]] QVariant headerData(
      int section,
      ::Qt::Orientation orientation,
      int role) const override;

    /**
     * @brief Returns the Media Set Element for the given @p index.
     *
     * @param[in] index
     *   Index of requested element.
     *
     * @return Corresponding Media Set Element.
     * @retval Arinc665::Media::BasePtr{}
     *   If index is invalid or no model stored.
     **/
    Arinc665::Media::BasePtr element( const QModelIndex &index);

  public slots:
    /**
     * @brief Updates the associated media set.
     *
     * @param[in] mediaSet
     *   New associated media set
     **/
    void setMediaSet( Arinc665::Media::MediaSetPtr mediaSet = {});

  private:
    //! Media Set
    Arinc665::Media::MediaSetPtr mediaSetV;
};

}

#endif

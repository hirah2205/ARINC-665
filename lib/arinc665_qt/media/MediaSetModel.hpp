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
 *
 * This is a hierarchical model giving access to %Media Sets, %Media,
 * Directories and Files.
 *
 * The Model-Index holds the Pointer to the Arinc665::Media::Base object.
 **/
class ARINC665_QT_EXPORT MediaSetModel : public QAbstractItemModel
{
    Q_OBJECT

  public:
    //! Displayed Columns
    enum class Columns
    {
      //! Element Name
      Name,
      //! Element Type
      Type,

      Last
    };

    /**
     * @brief Initialises the model instance.
     *
     * @param[in] parent
     *   Parent object
     **/
    explicit MediaSetModel( QObject * parent = nullptr );

    //! Destructor
    ~MediaSetModel() override = default;

    /**
     * @brief Creates the index for the child identified by its parent and its
     *   row and column.
     *
     * @param[in] row
     *   Index Row
     * @param[in] column
     *   Index Column
     * @param[in] parent
     *   Parent Model Index
     *
     * @return Model Index
     **/
    [[nodiscard]] QModelIndex index(
      int row,
      int column,
      const QModelIndex &parent = {} ) const override;

    /**
     * @brief Return the parent of the given index.
     *
     * @param[in] index
     *   Model Index
     *
     * @return Model Index of Parent.
     **/
    [[nodiscard]] QModelIndex parent( const QModelIndex &index ) const override;

    /**
     * @brief Return if the given parent has children.
     *
     * @param[in] parent
     *   Parent Model Index.
     *
     * @return If @p parent has children.
     **/
    [[nodiscard]] bool hasChildren( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   Parent Model Index.
     *
     * @return Number of Rows.
     **/
    [[nodiscard]] int rowCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   Parent Model Index.
     *
     * @return Number of Columns.
     **/
    [[nodiscard]] int columnCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the data for the given index.
     *
     * @param[in] index
     *   Model Index
     * @param[in] role
     *   Data Role
     *
     * @return Data corresponding to @p index and @p role.
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
      int role ) const override;

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
    [[nodiscard]] Arinc665::Media::ConstBasePtr element(
      const QModelIndex &index ) const;

    /**
     * @brief Returns the Model Index for the given element.
     *
     * @param[in] element
     *   Element of Model.
     *
     * @return Model Index representing @p element.
     **/
    [[nodiscard]] QModelIndex indexForElement(
      const Arinc665::Media::ConstBasePtr &element ) const;

    /**
     * @name Root Element
     *
     * The Root-Element of the %Media Set Model.
     *
     * @{
     **/

    /**
     * @brief Updates the associated Root Element.
     *
     * @param[in] root
     *   New Root Element.
     **/
    void root( Arinc665::Media::ConstBasePtr root );

    /**
     * @brief Returns Root Element
     *
     * @return Model Root element.
     **/
    [[nodiscard]] const Arinc665::Media::ConstBasePtr& root() const;

    /** @} **/

  private:
    //! Root Element
    Arinc665::Media::ConstBasePtr rootV{};
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::BatchesModel.
 **/

#ifndef ARINC665_QT_MEDIA_BATCHESMODEL_HPP
#define ARINC665_QT_MEDIA_BATCHESMODEL_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractTableModel>

namespace Arinc665Qt::Media {

/**
 * @brief QT model of List of Batches.
 **/
class ARINC665_QT_EXPORT BatchesModel : public QAbstractTableModel
{
    Q_OBJECT

  public:
    //! Columns of Model
    enum class Columns
    {
      Name,
      PartNumber,
      Comment,

      ColumnsCount
    };

    /**
     * @brief Initialises the Batches Model.
     *
     * @param[in] parent
     *   Parent QObject.
     **/
    explicit BatchesModel( QObject *parent = nullptr );

    //! Destructor
    ~BatchesModel() override;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   Index-parent - assumed to be the root element (invalid).
     *
     * @return The number of loads.
     * @retval 0
     *   If @p is valid (not root element)
     **/
    [[nodiscard]] int rowCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   Index-parent - assumed to be the root element (invalid).
     *
     * @return Always Columns::ColumnsCount.
     **/
    [[nodiscard]] int columnCount( const QModelIndex &parent ) const override;

    /**
     * @brief Returns the requested data.
     *
     * @param[in] index
     *   Index of the requested item.
     * @param[in] role
     *   Requested role.
     *
     * @return Data dependent of the index and role.
     **/
    [[nodiscard]] QVariant data(
      const QModelIndex &index,
      int role ) const override;

    /**
     * @brief Returns the data for the given role and section in the header with
     *   the specified orientation.
     *
     * @param[in] section
     *   Section number
     * @param[in] orientation
     *   Orientation
     * @param[in] role
     *   Item role.
     *
     * @return Header data for the given parameters.
     **/
    [[nodiscard]] QVariant headerData(
      int section,
      ::Qt::Orientation orientation,
      int role ) const override;

    /**
     * @brief Returns the Batch for the given index.
     *
     * @param[in] index
     *   Index of the requested item.
     *
     * @return Batch for the given index.
     **/
    virtual Arinc665::Media::ConstBatchPtr batch(
      const QModelIndex &index ) const = 0;

    /**
     * @brief Updates the Data Model with the given Batches.
     *
     * @param[in] batches
     *   Batches, contained by the model.
     **/
    void batches( const Arinc665::Media::ConstBatches &batches = {} );

  private:
    //! Batches List
    Arinc665::Media::ConstBatches batchesV;
};

}

#endif

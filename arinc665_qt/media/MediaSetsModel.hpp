/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::MediaSetsModel.
 **/

#ifndef ARINC665_ARINC665_QT_MEDIA_MEDIASETSMODEL_HPP
#define ARINC665_ARINC665_QT_MEDIA_MEDIASETSMODEL_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractTableModel>

namespace Arinc665Qt::Media {

/**
 * @brief QT model of list of %Media Sets.
 **/
class ARINC665_QT_EXPORT MediaSetsModel : public QAbstractTableModel
{
    Q_OBJECT

  public:
    //! Columns of Model
    enum class Columns
    {
      PartNumber,

      ColumnsCount
    };

    /**
     * @brief Initialises the loads model.
     *
     * @param[in] parent
     *   Parent QObject.
     **/
    explicit MediaSetsModel( QObject *parent = nullptr );

    //! Destructor
    ~MediaSetsModel() override;

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
     * @brief Returns the Media Set for the given index.
     *
     * @param[in] index
     *   Index of the requested item.
     *
     * @return Media Set for the given index.
     **/
    [[nodiscard]] Arinc665::Media::ConstMediaSetPtr mediaSet(
      const QModelIndex &index ) const;

    /**
     * @brief Updates the data model with the given Media Sets.
     *
     * @param[in] mediaSets
     *   Media Sets, contained by the model.
     **/
    void mediaSets( const Arinc665::Media::ConstMediaSets &mediaSets = {} );

  private:

    //! loads list
    Arinc665::Media::ConstMediaSets mediaSetsV;
};

}

#endif

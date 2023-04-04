/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::FileCreationPolicyModel.
 **/

#ifndef ARINC665_QT_FILECREATIONPOLICYMODEL_H
#define ARINC665_QT_FILECREATIONPOLICYMODEL_H

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QAbstractListModel>

namespace Arinc665Qt {

/**
 * @brief QT File Creation Policy Model.
 **/
class ARINC665_QT_EXPORT FileCreationPolicyModel final :
  public QAbstractListModel
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Batches Model.
     *
     * @param[in] parent
     *   Parent QObject.
     **/
    explicit FileCreationPolicyModel( QObject * parent = nullptr );

    //! Destructor
    ~FileCreationPolicyModel() override;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   Index-parent - assumed to be the root element (invalid).
     *
     * @return Number of Media.
     * @retval 0
     *   If @p is valid (not root element)
     **/
    [[nodiscard]] int rowCount( const QModelIndex &parent ) const override;

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
     * @brief Return File Creation Policy for @p index.
     *
     * @param[in] index
     *   Model Index
     *
     * @return File Creation Policy for @p index
     * @retval {}
     *   If @p index is out of range.
     **/
    [[nodiscard]] std::optional< Arinc665::Utils::FileCreationPolicy >
    fileCreationPolicy( const QModelIndex &index ) const;

    /**
     * @brief Return File Creation Policy for @p index.
     *
     * @param[in] row
     *   Row Index
     *
     * @return File Creation Policy for @p index
     * @retval {}
     *   If @p index is out of range.
     **/
    [[nodiscard]] std::optional< Arinc665::Utils::FileCreationPolicy >
    fileCreationPolicy( int row ) const;
};

}

#endif

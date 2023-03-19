/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaPathsModel.
 **/

#ifndef ARINC665_QT_MEDIAPATHSMODEL_HPP
#define ARINC665_QT_MEDIAPATHSMODEL_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/MediumNumber.hpp>

#include <QAbstractTableModel>

#include <filesystem>
#include <map>

namespace Arinc665Qt {

/**
 * @brief QT Media Paths Model.
 *
 * This Model Provides a Media Paths Model for
 **/
class ARINC665_QT_EXPORT MediaPathsModel final : public QAbstractTableModel
{
    Q_OBJECT
  public:
    //! Media Paths
    using MediaPaths =
      std::map< Arinc665::MediumNumber, std::filesystem::path >;

    //! Columns of Model
    enum class Columns
    {
      //! Medium Number
      MediumNumber,
      //! Medium Path
      MediumPath,

      ColumnsCount
    };

    /**
     * @brief Initialises the Batches Model.
     *
     * @param[in] parent
     *   Parent QObject.
     **/
    explicit MediaPathsModel( QObject * parent = nullptr );

    //! Destructor
    ~MediaPathsModel() override;

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
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   Index-parent - assumed to be the root element (invalid).
     *
     * @return Columns::ColumnsCount.
     * @retval 0
     *   If @p is valid (not root element)
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
     *   Section Number
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
      int role = Qt::DisplayRole ) const override;

    /**
     * @brief Return all media paths.
     *
     * @return Media Paths.
     **/
    [[nodiscard]] const MediaPaths& mediaPaths() const;

    /**
     * @brief Return Medium Path for Medium Number.
     *
     * @param[in] mediumNumber
     *   Medium Number
     *
     * @return Medium Path for Medium Number.
     **/
    [[nodiscard]] const std::filesystem::path& mediumPath(
      Arinc665::MediumNumber mediumNumber ) const;

    //! @copydoc mediumPath(Arinc665::MediumNumber) const
    [[nodiscard]] std::filesystem::path& mediumPath(
      Arinc665::MediumNumber mediumNumber );

    /**
     * @brief Assigns Medium Path
     *
     * @param[in] mediumNumber
     *   Medium Number
     * @param[in] path
     *   Medium Path
     **/
    void mediumPath(
      Arinc665::MediumNumber mediumNumber,
      std::filesystem::path path );

  private:
    //! Media Paths
    MediaPaths mediaPathsV;
};

}

#endif

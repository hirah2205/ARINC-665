/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::LoadsModel.
 **/

#ifndef ARINC665_QT_LOADSMODEL_HPP
#define ARINC665_QT_LOADSMODEL_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QAbstractTableModel>

namespace Arinc665Qt {

/**
 * @brief QT model of list of loads.
 **/
class LoadsModel : public QAbstractTableModel
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the loads model.
     *
     * @param[in] parent
     *   The parent QObject.
     **/
    LoadsModel( QObject * parent = nullptr);

    //! Default destructor
    virtual ~LoadsModel() = default;

    /**
     * @brief Returns the number of rows.
     *
     * @param[in] parent
     *   The index-parent - assumed to be the root element (invalid).
     *
     * @return The number of loads.
     **/
    virtual int rowCount( const QModelIndex &parent) const override;

    /**
     * @brief Returns the number of columns.
     *
     * @param[in] parent
     *   The index-parent - assumed to be the root element (invalid).
     *
     * @return Always 2.
     **/
    int columnCount( const QModelIndex &parent) const override;

    /**
     * @brief Returns the requested data.
     *
     * @param[in] index
     *   Index of the requested item.
     * @param[in] role
     *   Requested role.
     *
     * @return The data dependent of the index and role.
     **/
    QVariant data( const QModelIndex &index, int role) const override;

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
     * @return
     *   Header data for the given parameters.
     **/
    QVariant headerData(
      int section,
      ::Qt::Orientation orientation,
      int role) const override;

    /**
     * @brief Returns the load for the given index.
     *
     * @param[in] index
     *   Index of the requested item.
     *
     * @return The load for the given index.
     **/
    virtual Arinc665::Media::LoadPtr getLoad( const QModelIndex &index) const = 0;

    /**
     * @brief Return the number of Loads.
     *
     * @return The number of Loads
     **/
    virtual size_t getLoadCount() const = 0;

  public slots:
    /**
     * @brief Updates the data model with the given ASF messages.
     *
     * @param[in] loads
     *   Loads, contained by the model.
     **/
    void setLoads( const Arinc665::Media::Loads &loads = {});

  private:
    //! loads list
    Arinc665::Media::Loads loads;
};

}

#endif

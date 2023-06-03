// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::LoadWidget.
 **/

#ifndef ARINC665_QT_MEDIA_LOADWIDGET_HPP
#define ARINC665_QT_MEDIA_LOADWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QGroupBox>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class LoadWidget;
}

//! Widget for Load Presentation
class ARINC665_QT_EXPORT LoadWidget final : public QGroupBox
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Load Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit LoadWidget( QWidget * parent = nullptr );

    //! Destructor
    ~LoadWidget() override;

  signals:
    /**
     * @brief Signal Emitted when user selected a Data or Support File.
     *
     * @param[in] file
     *   Selected Data or Support File.
     **/
    void activatedFile( Arinc665::Media::ConstRegularFilePtr file );

    /**
     * @brief Signal Emitted when user selected a Batch.
     *
     * @param[in] batch
     *   Selected Batch.
     **/
    void activatedBatch( Arinc665::Media::ConstBatchPtr batch );

  public slots:
    /**
     * @brief Called when a Load has been selected.
     *
     * @param[in] load
     *   Selected Load
     **/
    void selectLoad( Arinc665::Media::ConstLoadPtr load );

  private slots:
    /**
     * @brief Slot Called, when a Data File has been selected
     *
     * @param index
     **/
    void selectDataFile( const QModelIndex &index );

    /**
     * @brief Slot Called, when a Support File has been selected
     *
     * @param index
     **/
    void selectSupportFile( const QModelIndex &index );

    /**
     * @brief Slot Called, when a Batch has been selected.
     *
     * @param index
     **/
    void selectBatch( const QModelIndex &index );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::LoadWidget> ui{};
    //! THW ID Positions Model
    std::unique_ptr< TargetHardwareIdsPositionsModel >
      targetHardwareIdsPositionsModel{};
    //! Data Files Model
    std::unique_ptr< LoadFilesModel > dataFilesModelV{};
    //! Support Files Model
    std::unique_ptr< LoadFilesModel > supportFilesModelV{};
    //! Used in Batches Model
    std::unique_ptr< BatchesModel > usedInBatchesModelV{};
    //! Load
    Arinc665::Media::ConstLoadPtr loadV{};
};

}

#endif

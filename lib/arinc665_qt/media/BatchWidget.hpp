/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::BatchWidget.
 **/

#ifndef ARINC665_QT_MEDIA_BATCHWIDGET_HPP
#define ARINC665_QT_MEDIA_BATCHWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class BatchWidget;
}

//! Widget for Batch Presentation
class ARINC665_QT_EXPORT BatchWidget : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Batch Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit BatchWidget( QWidget * parent = nullptr );

    //! Destructor
    ~BatchWidget() override;

  public slots:
    /**
     * @brief Called when a batch has been selected.
     *
     * @param[in] batch
     *   Selected Batch
     **/
    void selectedBatch( Arinc665::Media::ConstBatchPtr batch );

  private slots:
    /**
     * @brief Slot Called, when user selects a Target.
     *
     * @param[in] index
     *   Model Index of selected Target
     **/
    void activatedTarget( const QModelIndex &index );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::BatchWidget > ui;

    //! Batch Target Information Model
    std::unique_ptr< BatchInfoModel > batchInfoModel;
    //! Batch Target Loads Model
    std::unique_ptr< LoadsModel > targetLoadsModel;

    //! Batch
    Arinc665::Media::ConstBatchPtr batchV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::BatchWidget.
 **/

#ifndef ARINC665_QT_BATCHWIDGET_HPP
#define ARINC665_QT_BATCHWIDGET_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

namespace Ui {
class BatchWidget;
}

//! Widget for Batch Presentation
class BatchWidget : public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Batch Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit BatchWidget( QWidget *parent = nullptr);

    //! Destructor
    ~BatchWidget() override;

  public slots:
    /**
     * @brief Called when a batch has been selected.
     *
     * @param[in] model
     *   Model owning @p batch
     * @param[in] batch
     *   Selected Batch
     **/
    void selectedBatch( MediaSetModel * model, Arinc665::Media::BatchPtr batch);

  private:
    //! UI (designer)
    std::unique_ptr< Ui::BatchWidget> ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! Batch
    Arinc665::Media::BatchPtr batchV;
};

}

#endif

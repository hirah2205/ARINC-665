/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::LoadWidget.
 **/

#ifndef ARINC665_QT_LOADWIDGET_HPP
#define ARINC665_QT_LOADWIDGET_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

namespace Ui {
class LoadWidget;
}

//! Widget for Load Presentation
class LoadWidget : public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Load Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit LoadWidget( QWidget *parent = nullptr);

    //! Destructor
    ~LoadWidget() override;

  public slots:
    /**
     * @brief Called when a Load has been selected.
     *
     * @param[in] model
     *   Model owning @p load
     * @param[in] load
     *   Selected Load
     **/
    void selectedLoad( MediaSetModel * model, Arinc665::Media::LoadPtr load);

  private:
    //! UI (designer)
    std::unique_ptr< Ui::LoadWidget> ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! Load
    Arinc665::Media::LoadPtr loadV;
};

}

#endif

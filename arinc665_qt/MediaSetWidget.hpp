/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaSetWidget.
 **/

#ifndef ARINC665_QT_MEDIASETWIDGET_HPP
#define ARINC665_QT_MEDIASETWIDGET_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

namespace Ui {
class MediaSetWidget;
}

//! Widget for Media Set Presentation
class MediaSetWidget: public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetWidget( QWidget *parent = nullptr);

    //! Destructor
    ~MediaSetWidget() override;

  public slots:
    /**
     * @brief Called when a Media Set has been selected.
     *
     * @param[in] model
     *   Model owning @p mediaSet
     * @param[in] mediaSet
     *   Selected Media Set
     **/
    void selectedMediaSet( MediaSetModel * model, Arinc665::Media::MediaSetPtr mediaSet);

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetWidget> ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! Media Set
    Arinc665::Media::MediaSetPtr mediaSetV;
};

}

#endif

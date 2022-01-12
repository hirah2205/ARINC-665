/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::MediumWidget.
 **/

#ifndef ARINC665_QT_MEDIA_MEDIUMWIDGET_HPP
#define ARINC665_QT_MEDIA_MEDIUMWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class MediumWidget;
}

//! Widget for Medium Presentation
class ARINC665_QT_EXPORT MediumWidget : public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Medium Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediumWidget( QWidget *parent = nullptr );

    //! Destructor
    ~MediumWidget() override;

  public slots:
    /**
     * @brief Called when a Medium has been selected.
     *
     * @param[in] model
     *   Model owning @p medium
     * @param[in] medium
     *   Selected Medium
     **/
    void selectedMedium(
      Arinc665Qt::Media::MediaSetModel * model,
      Arinc665::Media::MediumPtr medium );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediumWidget > ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! Medium
    Arinc665::Media::MediumPtr mediumV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::MediaSetWidget.
 **/

#ifndef ARINC665_QT_MEDIA_MEDIASETWIDGET_HPP
#define ARINC665_QT_MEDIA_MEDIASETWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class MediaSetWidget;
}

//! Widget for %Media Set Presentation
class ARINC665_QT_EXPORT MediaSetWidget : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetWidget( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetWidget() override;

    /**
     * @brief Sets the Media Set Model.
     *
     * @param[in] model
     *   Media Set Model.
     **/
    void mediaSetModel( Arinc665Qt::Media::MediaSetModel * model );

    /**
     * @brief Sets the Loads Model.
     *
     * @param[in] model
     *   Loads Model.
     **/
    void loadsModel( Arinc665Qt::Media::LoadsModel * model );

    /**
     * @brief Sets the Batches Model.
     *
     * @param[in] model
     *   Batches Model.
     **/
    void batchesModel( Arinc665Qt::Media::BatchesModel * model );

  public slots:
    /**
     * @brief Called when a Media Set has been selected.
     *
     * @param[in] mediaSet
     *   Selected Media Set
     **/
    void selectedMediaSet( Arinc665::Media::ConstMediaSetPtr mediaSet );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetWidget> ui;

    //! Media Set Model
    MediaSetModel * mediaSetModelV;
    //! Loads Model
    LoadsModel * loadsModelV;
    //! Batches Model
    BatchesModel * batchesModelV;

    //! Media Set
    Arinc665::Media::ConstMediaSetPtr mediaSetV;
};

}

#endif

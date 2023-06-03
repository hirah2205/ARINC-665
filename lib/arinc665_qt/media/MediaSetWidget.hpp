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
     * @brief Initialises the Media Set Widget.
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

  signals:
    /**
     * @brief Emitted when an element in one of the widgets ist activated.
     *
     * @param element
     **/
    void activatedElement( Arinc665::Media::ConstBasePtr element );

  public slots:
    /**
     * @brief Called when a Media Set has been selected.
     *
     * @param[in] mediaSet
     *   Selected Media Set
     **/
    void selectMediaSet( Arinc665::Media::ConstMediaSetPtr mediaSet );

  private slots:
    /**
     * @brief Slot Called, when an Element has been selected
     *
     * @param[in] index
     *   Model Index
     **/
    void selectElement( const QModelIndex &index );

    /**
     * @brief Slot Called, when a Load has been selected within Load Table
     *
     * @param[in] index
     *   Model Index
     **/
    void selectLoad( const QModelIndex &index );

    /**
     * @brief Slot Called, when an Batch has been selected within Batch Table
     *
     * @param[in] index
     *   Model Index
     **/
    void selectBatch( const QModelIndex &index );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetWidget> ui{};

    //! Loads Model
    std::unique_ptr< Media::LoadsModel > loadsModelV{};
    //! Batches Model
    std::unique_ptr< Media::BatchesModel > batchesModelV{};

    //! Media Set Model
    MediaSetModel * mediaSetModelV{ nullptr };

    //! Media Set
    Arinc665::Media::ConstMediaSetPtr mediaSetV{};
};

}

#endif

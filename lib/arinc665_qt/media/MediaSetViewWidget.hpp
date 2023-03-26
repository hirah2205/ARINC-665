/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::MediaSetViewWidget.
 **/

#ifndef ARINC665_QT_MEDIA_MEDIASETVIEWWIDGET_HPP
#define ARINC665_QT_MEDIA_MEDIASETVIEWWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class MediaSetViewWidget;
}

/**
 * @brief Widget for %Media Set Presentation.
 *
 * This provides a split view of a tree and a detailed window.
 **/
class ARINC665_QT_EXPORT MediaSetViewWidget : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetViewWidget( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetViewWidget() override;

    /**
     * @brief Sets the Media Set Model.
     *
     * @param[in] model
     *   Media Set Model.
     **/
    void mediaSetModel( MediaSetModel * model );

    /**
     * @brief Returns the part number of the model root.
     *
     * @return Part Number
     **/
    [[nodiscard]] std::string_view partNumber() const;

  private slots:
    /**
     * @brief Called when a selection has been made within tree-view.
     *
     * @param[in] index
     *   Index of selected element.
     **/
    void itemSelected( const QModelIndex &index );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetViewWidget > ui;
    //! Media Set Model
    MediaSetModel * mediaSetModelV;
};

}

#endif

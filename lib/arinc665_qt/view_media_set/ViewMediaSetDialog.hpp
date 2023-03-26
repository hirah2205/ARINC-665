/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ViewMediaSetDialog.
 **/

#ifndef ARINC665_QT_VIEW_MEDIA_SET_VIEWMEDIASETDIALOG_HPP
#define ARINC665_QT_VIEW_MEDIA_SET_VIEWMEDIASETDIALOG_HPP

#include <arinc665_qt/view_media_set/ViewMediaSet.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <QDialog>

#include <memory>

namespace Arinc665Qt{

namespace Ui{
class ViewMediaSetDialog;
}

//! %Media Set Dialog
class ARINC665_QT_EXPORT ViewMediaSetDialog : public QDialog
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ViewMediaSetDialog( QWidget * parent = nullptr );

    //! Destructor
    ~ViewMediaSetDialog() override;

    /**
     * @brief Sets the Media Set Model.
     *
     * @param[in] model
     *   Media Set Model.
     **/
    void mediaSetModel( Media::MediaSetModel * model );

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
    std::unique_ptr< Ui::ViewMediaSetDialog > ui;
    //! Media Set Model
    Media::MediaSetModel * mediaSetModelV;
};

}

#endif

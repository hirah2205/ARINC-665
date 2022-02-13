/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::Directory Widget.
 **/

#ifndef ARINC665_QT_MEDIA_DIRECTORYWIDGET_HPP
#define ARINC665_QT_MEDIA_DIRECTORYWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class DirectoryWidget;
}

//! Widget for Directory Presentation
class ARINC665_QT_EXPORT DirectoryWidget : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Directory Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit DirectoryWidget( QWidget * parent = nullptr );

    //! Destructor
    ~DirectoryWidget() override;

    /**
     * @brief Sets the Media Set Model.
     *
     * @param[in] model
     *   Media Set Model.
     **/
    void mediaSetModel( Arinc665Qt::Media::MediaSetModel * model );

  public slots:
    /**
     * @brief Slot called, when a Directory is selected.
     *
     * Updated the Table View to show content of selected directory.
     *
     * @param[in] index
     *   Model Index of selected Medium.
     **/
    void selectedDirectory( const QModelIndex &index );

    /**
     * @brief Called when a Directory has been selected.
     *
     * @param[in] directory
     *   Selected Directory
     **/
    void selectedDirectory( Arinc665::Media::ConstDirectoryPtr directory );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DirectoryWidget > ui;
    //! Media Set Model
    MediaSetModel * mediaSetModelV;
    //! Directory
    Arinc665::Media::ConstDirectoryPtr directoryV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ViewMediaSetXmlWindow.
 **/

#ifndef ARINC665_QT_VIEW_MEDIA_SET_XML_VIEWMEDIASETXMLWINDOW_HPP
#define ARINC665_QT_VIEW_MEDIA_SET_XML_VIEWMEDIASETXMLWINDOW_HPP


#include <arinc665_qt/view_media_set_xml/ViewMediaSetXml.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QMainWindow>
#include <QFileDialog>

#include <filesystem>
#include <memory>

namespace Arinc665Qt {

namespace Ui{
class ViewMediaSetXmlWindow;
}

//! %Media Set XML Window
class ARINC665_QT_EXPORT ViewMediaSetXmlWindow : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Window.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ViewMediaSetXmlWindow( QWidget * parent = nullptr );

    //! Destructor
    ~ViewMediaSetXmlWindow() override;

  private slots:
    /**
     * @brief Called, when the user has selected a file.
     *
     * @param[in] file
     *   Selected file.
     **/
    void xmlFileSelected( const QString &file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::ViewMediaSetXmlWindow > ui{};
    //! Select Media Set XML file dialog
    std::unique_ptr< QFileDialog > selectMediaSetXmlDialogV{};

    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModelV{};
    //! File Path Mapping Model
    std::unique_ptr< FilePathMappingModel > filePathMappingModelV{};
};

}

#endif

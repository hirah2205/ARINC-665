/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::FileWidget.
 **/

#ifndef ARINC665_QT_MEDIA_FILEWIDGET_HPP
#define ARINC665_QT_MEDIA_FILEWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class FileWidget;
}

//! Widget for Basic File Presentation
class ARINC665_QT_EXPORT FileWidget : public QWidget
{
    Q_OBJECT

  public:
    //! Enumeration identify the Widgets
    enum class FileStackedWidget : int
    {
      RegularFile = 0,
      LoadFile = 1,
      BatchFile = 2
    };

    /**
     * @brief Initialises the File Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit FileWidget( QWidget * parent = nullptr );

    //! Destructor
    ~FileWidget() override;

  signals:
    /**
     * @brief Emitted when a file in one of the widgets ist activated.
     *
     * @param file
     **/
    void activatedFile( Arinc665::Media::ConstFilePtr file );

  public slots:
    /**
     * @brief Called when a file has been selected.
     *
     * @param[in] file
     *   Selected File
     **/
    void selectFile( Arinc665::Media::ConstFilePtr file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::FileWidget> ui{};
    //! File
    Arinc665::Media::ConstFilePtr fileV{};
};

}

#endif

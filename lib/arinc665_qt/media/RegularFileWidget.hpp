/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::Media::RegularFileWidget.
 **/

#ifndef ARINC665_QT_MEDIA_REGULARFILEWIDGET_HPP
#define ARINC665_QT_MEDIA_REGULARFILEWIDGET_HPP

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt::Media {

namespace Ui {
class RegularFileWidget;
}

//! Widget for Regular File Presentation
class ARINC665_QT_EXPORT RegularFileWidget : public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the Regular File Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit RegularFileWidget( QWidget * parent = nullptr );

    //! Destructor
    ~RegularFileWidget() override;

  public slots:
    /**
     * @brief Called when a File has been selected.
     *
     * @param[in] file
     *   Selected File
     **/
    void selectFile( Arinc665::Media::ConstFilePtr file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::RegularFileWidget > ui{};
    //! File
    Arinc665::Media::ConstFilePtr fileV{};
};

}

#endif

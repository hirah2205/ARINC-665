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
class FileWidget: public QWidget
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the File Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit FileWidget( QWidget * parent = nullptr );

    //! Destructor
    ~FileWidget() override;

  public slots:
    /**
     * @brief Called when a file has been selected.
     *
     * @param[in] model
     *   Model owning @p file
     * @param[in] file
     *   Selected File
     **/
    void selectedFile(
      Arinc665Qt::Media::MediaSetModel * model,
      Arinc665::Media::BaseFilePtr file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::FileWidget> ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! File
    Arinc665::Media::BaseFilePtr fileV;
};

}

#endif

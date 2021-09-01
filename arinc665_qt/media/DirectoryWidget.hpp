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
class DirectoryWidget : public QWidget
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

  public slots:
    /**
     * @brief Called when a directory has been selected.
     *
     * @param[in] model
     *   Model owning @p directory
     * @param[in] directory
     *   Selected Directory
     **/
    void selectedDirectory(
      MediaSetModel * model,
      Arinc665::Media::DirectoryPtr directory );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DirectoryWidget> ui;
    //! Media Set Model
    MediaSetModel * modelV;
    //! Directory
    Arinc665::Media::DirectoryPtr directoryV;
};

}

#endif

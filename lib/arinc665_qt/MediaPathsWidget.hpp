/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaPathsWidget.
 **/

#ifndef ARINC665_QT_MEDIAPATHSPAGE_HPP
#define ARINC665_QT_MEDIAPATHSPAGE_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <QGroupBox>
#include <QFileDialog>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class MediaPathsWidget;
}

/**
 * @brief Media Paths Widget.
 *
 * This Widget provides a dialog to add and remove medias.
 * The media are held by a @ref MediaPathsModel.
 **/
class ARINC665_QT_EXPORT MediaPathsWidget final : public QGroupBox
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Widget.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaPathsWidget( QWidget * parent = nullptr );

    //! Destructor
    ~MediaPathsWidget() override;

    /**
     * @brief Sets the Media Paths Model.
     *
     * @param[in] model
     *   Media Paths Model
     **/
    void mediaPathsModel( MediaPathsModel * model );

  private slots:
    /**
     * @brief Slot when user wants to add medium directory.
     *
     * This slot is connected to the add-button QPushButton::pressed signal.
     * The select directory dilaog is shown.
     **/
    void addMediumDirectory();

    /**
     * @brief Slot when user wants to delete selcted medium directory.
     *
     * This slot is connected to the delete-button QPushButton::pressed signal.
     * The selected medium is removed from the model.
     **/
    void removeMediumDirectory();

    /**
     * @brief Slot called, when input directory has been selected.
     *
     * This slot is connected to the QFileDialog::fileSelected signal.
     * This slot checks if the selcted directory is a media directory and adds
     * it to the model.
     *
     * @param[in] file
     *   Selected File
     **/
    void mediumDirectorySelected( const QString &file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaPathsWidget > ui;
    //! Select XML File Dialog
    std::unique_ptr< QFileDialog > selectMediaPathDialog;
    //! Media Paths Model
    MediaPathsModel * mediaPathsModelV{ nullptr };
};

}

#endif

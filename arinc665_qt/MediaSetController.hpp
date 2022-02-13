/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaSetController.
 **/

#ifndef ARINC665_QT_MEDIASETCONTROLLER_HPP
#define ARINC665_QT_MEDIASETCONTROLLER_HPP

#include <arinc665_qt/Arinc665Qt.hpp>
#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/files/Files.hpp>

#include <QObject>
#include <QFileDialog>
#include <QWidget>

#include <filesystem>
#include <memory>

namespace Arinc665Qt {

/**
 * @brief Media Set Load and Display Controller.
 **/
class ARINC665_QT_EXPORT MediaSetController : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Controller.
     *
     * @param[in] parent
     *   Parent Widget.
     **/
    explicit MediaSetController( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetController() override;

  signals:
    /**
     * @brief Signal, which is emitted when the dialog has been closed or the
     *   operation aborted.
     **/
    void finished();

  public slots:
    /**
     * @brief Starts the BHM analysing session.
     **/
    void start();

    /**
     * @brief Starts the Media Set View with the given Media Set.
     *
     * @param[in] mediaSet
     *   Media Set
     **/
    void start( Arinc665::Media::ConstMediaSetPtr mediaSet );

  private slots:
    /**
     * @brief Called, when the user has selected a dialog
     **/
    void directorySelected();

  private:
    /**
     * @brief Loads the requested file.
     *
     * Is a callback od the media set importer.
     *
     * @param[in] mediumNumber
     *   Medium Number.
     * @param[in] path
     *   Path of file.
     *
     * @return Loaded file data.
     **/
    Arinc665::Files::RawFile loadFile(
      uint8_t mediumNumber,
      const std::filesystem::path &path );

    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModel;
    //! Loads Model
    std::unique_ptr< Media::LoadsModel > loadsModel;
    //! Batches Model
    std::unique_ptr< Media::BatchesModel > batchesModel;

    //! Select Media Set directory dialog
    std::unique_ptr< QFileDialog > selectDirectoryDialog;
    //! Media Set Dialog
    std::unique_ptr< MediaSetDialog > mediaSetDialog;
};

}

#endif

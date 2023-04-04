/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaSetViewWindow.
 **/

#ifndef ARINC665_QT_MEDIA_SET_VIEW_WMEDIASETVIEWINDOW_HPP
#define ARINC665_QT_MEDIA_SET_VIEW_WMEDIASETVIEWINDOW_HPP


#include <arinc665_qt/media_set_view/MediaSetViewWindow.hpp>

#include <arinc665_qt/decompile_media_set/DecompileMediaSet.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QMainWindow>
#include <QFileDialog>
#include <QSortFilterProxyModel>

#include <filesystem>
#include <memory>

namespace Arinc665Qt {

namespace Ui{
class MediaSetViewWindow;
}

/**
 * @brief %Media Set XML Window
 **/
class ARINC665_QT_EXPORT MediaSetViewWindow : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Window.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetViewWindow( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetViewWindow() override;

  private slots:
    /**
     * @brief Slot called, when user changed the check file integrity handling
     *
     * @param[in] checkFileIntegrity
     *   Check File Integrity indicator
     **/
    void checkFileIntegrity( bool checkFileIntegrity );

    /**
     * @brief Slot for Media Set Decompilation.
     **/
    void startMediaSetDecompilation();


    /**
     * @brief Called, when the user has selected a file.
     *
     * @param[in] file
     *   Selected file.
     **/
    void loadXmlFile( const QString &file );

    /**
     * @brief Save XML file.
     *
     * @param[in] file
     *   XML Filename
     **/
    void saveXmlFile( const QString &file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetViewWindow > ui{};
    //! Wizard Dialog
    std::unique_ptr< DecompileMediaSetWizard > decompileMediaSetWizardV{};
    //! Select Load Media Set XML file dialog
    std::unique_ptr< QFileDialog > selectLoadMediaSetXmlDialogV{};
    //! Select Save Media Set XML file dialog
    std::unique_ptr< QFileDialog > selectSaveMediaSetXmlDialogV{};

    //! Media Paths Model
    std::unique_ptr< MediaPathsModel > mediaPathsModelV{};
    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModelV{};
    //! File Path Mapping Model
    std::unique_ptr< FilePathMappingModel > filePathMappingModelV{};
    //! Sorted File Mapping Model
    std::unique_ptr< QSortFilterProxyModel > sortedFilePathMappingModelV{};

    //! Check File Integrity
    bool checkFileIntegrityV{ false };
};

}

#endif

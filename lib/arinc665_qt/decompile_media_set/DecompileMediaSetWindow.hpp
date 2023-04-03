/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::DecompileMediaSetWindow.
 **/

#ifndef ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETWINDOW_HPP
#define ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETWINDOW_HPP

#include <arinc665_qt/decompile_media_set/DecompileMediaSet.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QMainWindow>
#include <QFileDialog>
#include <QSortFilterProxyModel>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class DecompileMediaSetWindow;
}

/**
 * @brief Decompile ARINC 665 %Media Set to ARINC 665 %Media Set XML.
 **/
class ARINC665_QT_EXPORT DecompileMediaSetWindow final : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Decompile %Media Set Window.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit DecompileMediaSetWindow( QWidget * parent = nullptr );

    //! Destructor
    ~DecompileMediaSetWindow() override;

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
    void startDecompilation();

    /**
     * @brief Save XML file.
     *
     * @param[in] file
     *   XML Filename
     **/
    void saveXmlFile( const QString &file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DecompileMediaSetWindow > ui;
    //! Wizard Dialog
    std::unique_ptr< DecompileMediaSetWizard > wizardV{};
    //! Media Paths Model
    std::unique_ptr< MediaPathsModel > mediaPathsModelV{};
    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModelV{};
    //! File Path Mapping Model
    std::unique_ptr< FilePathMappingModel > filePathMappingModelV{};
    //! Sorted File Mapping Model
    std::unique_ptr< QSortFilterProxyModel > sortedFilePathMappingModelV{};
    //! Select XML File Dialog
    std::unique_ptr< QFileDialog > selectXmlFileDialog{};
    //! Check File Integrity
    bool checkFileIntegrityV{ false };
    //! Media Set
    Arinc665::Media::ConstMediaSetPtr mediaSetV{};
};

}

#endif

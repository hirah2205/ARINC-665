/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::DecompileMediaSetWizard.
 **/

#ifndef ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETWIZARD_HPP
#define ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETWIZARD_HPP

#include <arinc665_qt/decompile_media_set/DecompileMediaSet.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <QWizard>
#include <QFileDialog>

#include <memory>
#include <filesystem>

namespace Arinc665Qt {

namespace Ui{
class DecompileMediaSetWizard;
}

//! Decompile ARINC 665 %Media Set Wizard
class ARINC665_QT_EXPORT DecompileMediaSetWizard final : public QWizard
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Decompile Media Set Wizard.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit DecompileMediaSetWizard( QWidget * parent = nullptr );

    //! Destructor
    ~DecompileMediaSetWizard() override;

    /**
     * @brief Sets the Media Paths Model.
     *
     * @param[in] model
     *   Media Paths Model
     **/
    void mediaPathsModel( MediaPathsModel * model );

    /**
     * @brief Sets the Media Set Model.
     *
     * @param[in] model
     *   Media Set Model
     **/
    void mediaSetModel( Media::MediaSetModel * model );

    /**
     * @brief Sets the File Path Mapping Model.
     *
     * @param[in] model
     *   File Path Mapping Model
     **/
    void filePathMappingModel( FilePathMappingModel * model );

  signals:
    /**
     * @brief Signal emitted, when the check file integrity field has been
     *   selected by user.
     *
     * @param[in] checkFileIntegrity
     *   Check file integrity indicator.
     **/
    void checkFileIntegrity( bool checkFileIntegrity );

    //! Signal emitted when operation should be started.
    void start();

    /**
     * @brief Signal emitted, when the user selects the XML file to save.
     *
     * @param[in] xmlFile
     *   XML File to store.
     **/
    void save( std::filesystem::path xmlFile );

  private slots:
    /**
     * @brief Slot for currentIdChanged() signal.
     *
     * @param[in] id
     *   new current ID
     **/
    void pageChanged( int id );

    /**
     * @brief Save XML file.
     *
     * @param[in] file
     *   XML Filename
     **/
    void saveXmlFile( const QString &file );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DecompileMediaSetWizard > ui;
    //! Select XML File Dialog
    std::unique_ptr< QFileDialog > selectXmlFileDialog;
};

}

#endif

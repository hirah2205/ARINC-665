/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class
 *   Arinc665Qt::MediaSetManager::ImportMediaSetXmlAction.
 **/

#ifndef ARINC665_QT_MEDIA_SET_MANAGER_IMPORTMEDIASETXMLWIZARD_HPP
#define ARINC665_QT_MEDIA_SET_MANAGER_IMPORTMEDIASETXMLWIZARD_HPP

#include <arinc665_qt/media_set_manager/MediaSetManager.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWizard>

#include <memory>
#include <filesystem>

namespace Arinc665Qt::MediaSetManager {

namespace Ui{
class ImportMediaSetXmlWizard;
}

//! Import %Media Set XML Wizard
class ARINC665_QT_EXPORT ImportMediaSetXmlWizard : public QWizard
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Import Media Set XML Wizard.
     *
     * @param[in] mediaSetManager
     *   Media Set Manager
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ImportMediaSetXmlWizard(
      Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
      QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlWizard() override;

  private slots:
    /**
     * @brief Slot for currentIdChanged() signal.
     *
     * @param[in] id
     *   new current ID
     **/
    void pageChanged( int id );

    /**
     * @brief Called when a ARINC 665 Media Set XML file has been selected.
     *
     * @param[in] xmlFile
     *   Selected ARINC 665 Media Set XML file.
     **/
    void xmlFile( std::filesystem::path xmlFile );

    /**
     * @brief Called when an input directory has been selected.
     *
     * @param[in] directory
     *   Selected input directory.
     **/
    void inputDirectory( std::filesystem::path directory );

    /**
     * @brief Called the ARINC 665 Version Flag.
     *
     * @param[in] version
     *   ARINC 665 version used for exporting.
     **/
    void arinc665Version( Arinc665::SupportedArinc665Version version );

    /**
     * @brief Called the Create Batch Files Flag.
     *
     * @param[in] createBatchFiles
     *   Defines, if Batch Files are created by exporter or pre-existing ones
     *   are used.
     **/
    void createBatchFiles(
      Arinc665::Utils::FileCreationPolicy createBatchFiles );

    /**
     * @brief Called the Create Load Header Files Flag.
     *
     * @param[in] createLoadHeaderFiles
     *   Defines, if Load Header Files are created by exporter or pre-existing
     *   ones are used.
     **/
    void createLoadHeaderFiles(
      Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles );

  private:
    /**
     * @brief Start Media Set XML Import.
     **/
    void importMediaSetXml();

    //! UI (designer)
    std::unique_ptr< Ui::ImportMediaSetXmlWizard > ui;
    //! Media Set Manager
    Arinc665::Utils::MediaSetManagerPtr mediaSetManagerV;
    //! ARINC 665 Media Set Exporter
    Arinc665::Utils::FilesystemMediaSetCompilerPtr compilerV;
    //! XML File
    std::filesystem::path xmlFileV;
};

}

#endif

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLACTION_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLACTION_HPP

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXml.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

/**
 * @brief Import %Media Set XML Controller.
 *
 * This class Compiles an ARINC 665 Media Set XML to an ARINC 665 Media Set and
 * imports it to the Media Set Manager.
 **/
class ARINC665_QT_EXPORT ImportMediaSetXmlAction final : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the %Media Set Import XML Controller.
     *
     * @param[in] mediaSetManager
     *   Media Set Manager
     * @param[in] parent
     *   Parent Widget
     **/
    explicit ImportMediaSetXmlAction(
      Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
      QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlAction() override;

  signals:
    //! Emitted, when action is finished.
    void finished();

  private slots:
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

    /**
     * @brief Slot for conversion.
     **/
    void start();

  private:
    //! Wizard Dialog
    std::unique_ptr< ImportMediaSetXmlWizard > wizard;
    //! Media Set Manager
    Arinc665::Utils::MediaSetManagerPtr mediaSetManagerV;
    //! ARINC 665 Media Set Exporter
    Arinc665::Utils::FilesystemMediaSetExporterPtr exporter;
    //! XML File
    std::filesystem::path xmlFileV;
};

}

#endif

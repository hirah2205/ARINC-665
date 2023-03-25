/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::CompileMediaSetAction.
 **/

#ifndef ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETACTION_HPP
#define ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETACTION_HPP

#include <arinc665_qt/compile_media_set/CompileMediaSet.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

/**
 * @brief Compile ARINC 665 %Media Set XML to ARINC 665 %Media Set.
 *
 * This Controller guides through the process of generation of an ARINC 665
 * %Media Set by:
 *  - Ask the User for:
 *    - ARINC 665 %Media Set XML,
 *    - Input Source Path
 *    - Output Directory
 *    - %Media Set Parameters (Version, Load Header generation and Batch File
 *      generation)
 *  - Perform actual compilation of ARINC 665 %Media Set.
 **/
class ARINC665_QT_EXPORT CompileMediaSetAction final : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Compile %Media Set Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit CompileMediaSetAction( QWidget * parent = nullptr );

    //! Destructor
    ~CompileMediaSetAction() override;

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
     * @brief Called when an output directory has been selected.
     *
     * @param[in] directory
     *   Selected output directory.
     **/
    void outputDirectory( std::filesystem::path directory );

    /**
     * @brief Slot for conversion.
     **/
    void start();

  private:
    //! Wizard Dialog
    std::unique_ptr< CompileMediaSetWizard > wizard;
    //! ARINC 665 Media Set Exporter
    Arinc665::Utils::FilesystemMediaSetExporterPtr exporter;
    //! XML File
    std::filesystem::path xmlFileV;
    //! Output Base Path
    std::filesystem::path outputDirectoryV;
};

}

#endif

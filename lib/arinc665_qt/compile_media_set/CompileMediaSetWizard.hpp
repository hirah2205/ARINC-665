/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::CompileMediaSetWizard.
 **/

#ifndef ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETWIZARD_HPP
#define ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETWIZARD_HPP

#include <arinc665_qt/compile_media_set/CompileMediaSet.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWizard>

#include <memory>
#include <filesystem>

namespace Arinc665Qt {

namespace Ui{
class CompileMediaSetWizard;
}

//! Compile ARINC 665 %Media Set Wizard
class ARINC665_QT_EXPORT CompileMediaSetWizard final : public QWizard
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Compile Media Set Wizard.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit CompileMediaSetWizard( QWidget * parent = nullptr );

    //! Destructor
    ~CompileMediaSetWizard() override;

  signals:
    /**
     * @brief Emitted when a ARINC 665 Media Set XML file has been selected.
     *
     * @param[in] xmlFile
     *   Selected ARINC 665 Media Set XML file.
     **/
    void xmlFile( std::filesystem::path xmlFile );

    /**
     * @brief Emitted when an input directory has been selected.
     *
     * @param[in] directory
     *   Selected input directory.
     **/
    void inputDirectory( std::filesystem::path directory );

    /**
     * @brief Emitted the ARINC 665 Version Flag.
     *
     * @param[in] version
     *   ARINC 665 version used for exporting.
     **/
    void arinc665Version( Arinc665::SupportedArinc665Version version );

    /**
     * @brief Emitted the Create Batch Files Flag.
     *
     * @param[in] createBatchFiles
     *   Defines, if Batch Files are created by exporter or pre-existing ones
     *   are used.
     **/
    void createBatchFiles(
      Arinc665::Utils::FileCreationPolicy createBatchFiles );

    /**
     * @brief Emitted the Create Load Header Files Flag.
     *
     * @param[in] createLoadHeaderFiles
     *   Defines, if Load Header Files are created by exporter or pre-existing
     *   ones are used.
     **/
    void createLoadHeaderFiles(
      Arinc665::Utils::FileCreationPolicy createLoadHeaderFiles );

    /**
     * @brief Emitted when an output directory has been selected.
     *
     * @param[in] directory
     *   Selected output directory.
     **/
    void outputDirectory( std::filesystem::path directory );

    //! Signal emitted when operation should be started.
    void start();

  private slots:
    /**
     * @brief Slot for currentIdChanged() signal.
     *
     * @param[in] id
     *   new current ID
     **/
    void pageChanged( int id );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::CompileMediaSetWizard > ui;
};

}

#endif

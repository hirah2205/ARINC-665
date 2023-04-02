/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::DecompileMediaSetAction.
 **/

#ifndef ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETACTION_HPP
#define ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETACTION_HPP

#include <arinc665_qt/decompile_media_set/DecompileMediaSet.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

/**
 * @brief Decompile ARINC 665 %Media Set to ARINC 665 %Media Set XML.
 **/
class ARINC665_QT_EXPORT DecompileMediaSetAction final : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Decompile %Media Set Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit DecompileMediaSetAction( QWidget * parent = nullptr );

    //! Destructor
    ~DecompileMediaSetAction() override;

  signals:
    //! Emitted, when action is finished.
    void finished();

  private slots:
    /**
     * @brief Slot called, when user changed the check file integrity handling
     *
     * @param[in] checkFileIntegrity
     *   Check File Integrity indicator
     **/
    void checkFileIntegrity( bool checkFileIntegrity );

    /**
     * @brief Slot for conversion.
     **/
    void start();

    /**
     * @brief Slot for XML Save operation.
     *
     * @param[in] xmlFile
     *   XML Filename.
     **/
    void save( std::filesystem::path xmlFile );

  private:
    //! Wizard Dialog
    std::unique_ptr< DecompileMediaSetWizard > wizardV{};
    //! ARINC 665 Media Set Importer
    Arinc665::Utils::FilesystemMediaSetImporterPtr importerV{};
    //! Media Paths Model
    std::unique_ptr< MediaPathsModel > mediaPathsModelV{};
    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModelV{};
    //! File Path Mapping Model
    std::unique_ptr< FilePathMappingModel > filePathMappingModelV{};
    //! Media Set
    Arinc665::Media::ConstMediaSetPtr mediaSetV{};
};

}

#endif

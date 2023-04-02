/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetAction.
 **/

#ifndef ARINC665_QT_INPORT_MEDIA_SET_IMPORTMEDIASETACTION_HPP
#define ARINC665_QT_INPORT_MEDIA_SET_IMPORTMEDIASETACTION_HPP

#include <arinc665_qt/import_media_set/ImportMediaSet.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

/**
 * @brief Imports an ARINC 665 %Media Set to the %Media Set Manager.
 **/
class ARINC665_QT_EXPORT ImportMediaSetAction final : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Import %Media Set Action.
     *
     * @param[in] mediaSetManager
     *   Media Set Manager
     * @param[in] parent
     *   Parent Widget
     **/
    explicit ImportMediaSetAction(
      Arinc665::Utils::MediaSetManagerPtr mediaSetManager,
      QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetAction() override;

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

  private:
    //! Wizard Dialog
    std::unique_ptr< ImportMediaSetWizard > wizardV{};
    //! Media Set Manager
    Arinc665::Utils::MediaSetManagerPtr mediaSetManagerV{};
    //! ARINC 665 Media Set Copier
    Arinc665::Utils::FilesystemMediaSetCopierPtr copierV{};
    //! Media Paths Model
    std::unique_ptr< MediaPathsModel > mediaPathsModelV{};
    //! Check File Integrity
    bool checkFileIntegrityV{ false };
};

}

#endif

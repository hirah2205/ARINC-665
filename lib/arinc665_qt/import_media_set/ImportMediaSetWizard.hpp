/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetWizard.
 **/

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_IMPORTMEDIASETWIZARD_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_IMPORTMEDIASETWIZARD_HPP

#include <arinc665_qt/import_media_set/ImportMediaSet.hpp>

#include <QWizard>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class ImportMediaSetWizard;
}

/**
 * @brief Import ARINC 665 %Media Set Wizard
 *
 * Imports a ARINC 665 Media Set into the Media Set Manager.
 **/
class ARINC665_QT_EXPORT ImportMediaSetWizard final : public QWizard
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Decompile Media Set Wizard.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ImportMediaSetWizard( QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetWizard() override;

    /**
     * @brief Sets the Media Paths Model.
     *
     * @param[in] model
     *   Media Paths Model
     **/
    void mediaPathsModel( MediaPathsModel * model );

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
    std::unique_ptr< Ui::ImportMediaSetWizard > ui;
};

}

#endif

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

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class DecompileMediaSetWizard;
}

/**
 * @brief Decompile ARINC 665 %Media Set Wizard.
 *
 * This wizard is used to query the Decompile ARINC 665 Media Set settings from
 * the user.
 *
 * When the dialog is accepted the operation must be initiated by the caller.
 *
 * Media Paths are accessible via the MediaPathsModel.
 * The Check File Integrity Setting is provided cia the signal
 * @ref checkFileIntegrity.
 **/
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

  signals:
    /**
     * @brief Signal emitted, when the check file integrity field has been
     *   selected by user.
     *
     * @param[in] checkFileIntegrity
     *   Check file integrity indicator.
     **/
    void checkFileIntegrity( bool checkFileIntegrity );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DecompileMediaSetWizard > ui{};
};

}

#endif

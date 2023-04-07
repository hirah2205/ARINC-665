/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::DecompileMediaSetSettingsPage.
 **/

#ifndef ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETSETTINGSPAGE_HPP
#define ARINC665_QT_DECOMPILE_MEDIA_SET_DECOMPILEMEDIASETSETTINGSPAGE_HPP

#include <arinc665_qt/decompile_media_set/DecompileMediaSet.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWizardPage>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class DecompileMediaSetSettingsPage;
}

//! Decompile %Media Set Settings Page
class ARINC665_QT_EXPORT DecompileMediaSetSettingsPage final : public QWizardPage
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Page.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit DecompileMediaSetSettingsPage( QWidget * parent = nullptr );

    //! Destructor
    ~DecompileMediaSetSettingsPage() override;

    /**
     * @brief Indicates if the page is completed.
     *
     * checks all input fields for validity.
     *
     * @return if page is completed
     **/
    [[nodiscard]] bool isComplete() const override;

    /**
     * @brief Initialises Page.
     *
     * Clears content of file path model.
     **/
    void initializePage() override;

    /**
     * @brief Sets the Media Paths Model.
     *
     * @param[in] model
     *   Media Paths Model
     **/
    void mediaPathsModel( MediaPathsModel * model );

    /**
     * @brief Initialises the page with its defaults.
     *
     * @param[in] defaults
     *   Media Set Defaults
     **/
    void defaults( const Arinc665::Utils::MediaSetDefaults &defaults );

  signals:
    /**
     * @brief Signal emitted, when the check file integrity field has been
     *   selected by user.
     *
     * @param[in] checkFileIntegrity
     *   Check file integrity indicator.
     **/
    void checkFileIntegrity( bool checkFileIntegrity );

  private slots:
    /**
     * @brief Slot called, when the file integrity changed checkbox has been
     *   changed.
     *
     * check state is determined and signal @ref checkFileIntegrity() is
     * emitted.
     *
     * @param[in] state
     *   Check State.
     **/
    void checkFileIntegrityStateChanged( int state );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::DecompileMediaSetSettingsPage > ui;
};

}

#endif

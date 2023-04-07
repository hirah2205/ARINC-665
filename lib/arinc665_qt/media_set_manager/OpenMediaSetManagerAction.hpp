/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::OpenMediaSetManagerAction.
 **/

#ifndef ARINC665_QT_MEDIA_SET_MANAGER_OPENMEDIASETMANAGERACTION_HPP
#define ARINC665_QT_MEDIA_SET_MANAGER_OPENMEDIASETMANAGERACTION_HPP

#include <arinc665_qt/media_set_manager/MediaSetManager.hpp>
#include <arinc665_qt/media/Media.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>
#include <QFileDialog>

#include <memory>

namespace Arinc665Qt::MediaSetManager {

/**
 * @brief Open %Media Set Manager Action
 *
 * Asks the User for selecting the Media Set Manager Directory and tries to open
 * it.
 **/
class ARINC665_QT_EXPORT OpenMediaSetManagerAction : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the %Media Set Manager Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit OpenMediaSetManagerAction( QWidget * parent = nullptr );

    //! Destructor
    ~OpenMediaSetManagerAction() override;

    /**
     * @brief Returns the Media Set Manager
     *
     * @return Media Set Manager
     **/
    const Arinc665::Utils::MediaSetManagerPtr& mediaSetManager() const;

  public slots:
    /**
     * @brief Querries for %Media Set Configuration.
     *
     * Ask the user for the location of the Media Set Manager Configuration and
     * loads them.
     **/
    void open();

  signals:
    //! Emitted, when controller is finished.
    void accepted();

    //! Operation canceled by user
    void rejected();

  private slots:
    /**
     * @brief Slot called, when user has selected %Media Set Configuration.
     **/
    void directorySelected();

  private:
    //! Select Media Set Manager Directory Dialog
    std::unique_ptr< QFileDialog > selectMediaSetDirectoryDialogV{};
    //! Media Set Manager
    Arinc665::Utils::MediaSetManagerPtr mediaSetManagerV{};
};

}

#endif

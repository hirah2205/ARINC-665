// SPDX-License-Identifier: MPL-2.0
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
 *   Arinc665Qt::MediaSetManager::RemoveMediaSetAction.
 **/

#ifndef ARINC_665_QT_MEDIA_SET_MANAGER_REMOVEMEDIASETACTION_HPP
#define ARINC_665_QT_MEDIA_SET_MANAGER_REMOVEMEDIASETACTION_HPP

#include <arinc_665_qt/media_set_manager/MediaSetManager.hpp>

#include <arinc_665/media/Media.hpp>

#include <arinc_665/utils/MediaSetManager.hpp>

#include <QWidget>

namespace Arinc665Qt::MediaSetManager {

/**
 * @brief Remove %Media Set Action.
 *
 * Ask for confirmation of deletion.
 **/
class ARINC_665_QT_EXPORT RemoveMediaSetAction final : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Remove %Media Set Manager Action.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit RemoveMediaSetAction( QWidget * parent = nullptr );

    //! Destructor
    ~RemoveMediaSetAction() override;

  public slots:
    /**
     * @brief Use given %Media Set Configuration.
     *
     * @param[in] mediaSetManager
     *   Media Set Manager
     * @param[in] mediaSet
     *   %Media Set Manager to use
     **/
    void start(
      const Arinc665::Utils::MediaSetManagerPtr &mediaSetManager,
      Arinc665::Media::ConstMediaSetPtr mediaSet );

  signals:
    //! Emitted, when controller is finished.
    void finished();
};

}

#endif

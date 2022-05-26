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
 *   Arinc665Qt::MediaSetManager::RemoveMediaSetController.
 **/

#ifndef ARINC665_QT_MEDIA_SET_MANAGER_REMOVEMEDIASETCONTROLLER_HPP
#define ARINC665_QT_MEDIA_SET_MANAGER_REMOVEMEDIASETCONTROLLER_HPP

#include <arinc665_qt/media_set_manager/MediaSetManager.hpp>

#include <arinc665/media/Media.hpp>

#include <QWidget>

namespace Arinc665Qt::MediaSetManager {

class ARINC665_QT_EXPORT RemoveMediaSetController : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Remove %Media Set Manager Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit RemoveMediaSetController( QWidget * parent = nullptr );

    //! Destructor
    ~RemoveMediaSetController() override;

  public slots:
    /**
     * @brief Use given %Media Set Configuration.
     *
     * @param[in] mediaSet
     *   %Media Set Manager to use
     **/
    void start( Arinc665::Media::MediaSetPtr mediaSet );

  signals:
    //! Emitted, when controller is finished.
    void finished();
};

}

#endif

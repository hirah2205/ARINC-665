/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaSetController.
 **/

#ifndef ARINC665_QT_MEDIA_SET_MANAGER_MEDIASETMANAGERCONTROLLER_HPP
#define ARINC665_QT_MEDIA_SET_MANAGER_MEDIASETMANAGERCONTROLLER_HPP

#include <arinc665_qt/media_set_manager/MediaSetManager.hpp>

#include <QObject>

namespace Arinc665Qt::MediaSetManager {

class MediaSetController : public QObject
{
    Q_OBJECT

  public:
    MediaSetController( QObject * parent );

  private slots:
    //void mediaSetOpen();

    //void mediaSetAdd();

    //void mediaSetDelete();
};

}

#endif

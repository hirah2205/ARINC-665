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
#include <arinc665_qt/media/Media.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QWidget>
#include <QFileDialog>

namespace Arinc665Qt::MediaSetManager {

class MediaSetManagerController : public QObject
{
    Q_OBJECT

  public:
    MediaSetManagerController( QWidget * parent = nullptr );

    ~MediaSetManagerController() override;

  public slots:
    /**
     * @brief Querries fro Media Set Configuration,
     **/
    void start();

    /**
     * @brief Use given Media Set Configuration.
     *
     * @param mediaSetManager
     **/
    void start( Arinc665::Utils::MediaSetManagerPtr mediaSetManager );

  signals:
    void finished();

  private slots:
    void configurationSelected();

    void viewMediaSet( const QModelIndex &index );

    void importMediaSet();

    void importMediaSetXml();

    void removeMediaSet( const QModelIndex &index);

  private:
    //! Media Sets Model
    std::unique_ptr< Media::MediaSetsModel > mediaSetsModelV;
    //! Media Set Manager Dialog
    std::unique_ptr< MediaSetManagerDialog > mediaSetManagerDialogV;
    //! Select Media Set File Dialog
    std::unique_ptr< QFileDialog > selectMediaSetConfigDialogV;

    //! Media Set Manager
    Arinc665::Utils::MediaSetManagerPtr mediaSetManagerV;
};

}

#endif

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
 *   Arinc665Qt::MediaSetManager::MediaSetManagerDialog.
 **/

#ifndef ARINC665_QT_MEDIA_SET_MANAGER_MEDIASETMANAGERDIALOG_HPP
#define ARINC665_QT_MEDIA_SET_MANAGER_MEDIASETMANAGERDIALOG_HPP

#include <arinc665_qt/media_set_manager/MediaSetManager.hpp>
#include <arinc665_qt/media/Media.hpp>

#include <QDialog>

#include <memory>

namespace Arinc665Qt::MediaSetManager {

namespace Ui{
class MediaSetManagerDialog;
}

class ARINC665_QT_EXPORT MediaSetManagerDialog : public QDialog
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Media Set Manager Dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetManagerDialog( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetManagerDialog() override;

    void mediaSetsModel( Media::MediaSetsModel * model );

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetManagerDialog > ui;
    //! Media Set Model
    Media::MediaSetsModel * mediaSetsModelV;
};

}

#endif

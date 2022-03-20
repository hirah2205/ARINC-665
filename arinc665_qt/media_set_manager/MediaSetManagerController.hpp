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

/**
 * @brief %Media Set Manager Controller
 *
 * This controller supports operation by using a predefined media set manager
 * by calling @ref start(Arinc665::Utils::JsonMediaSetManagerPtr).
 * When calling @ref start(), the user is asked to select a media set
 * configuration to load it.
 **/
class ARINC665_QT_EXPORT MediaSetManagerController : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the %Media Set Manager Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit MediaSetManagerController( QWidget * parent = nullptr );

    //! Destructor
    ~MediaSetManagerController() override;

  public slots:
    /**
     * @brief Querries for %Media Set Configuration,
     **/
    void start();

    /**
     * @brief Use given %Media Set Configuration.
     *
     * @param[in] mediaSetManager
     *   %Media Set Manager to use
     **/
    void start( Arinc665::Utils::JsonMediaSetManagerPtr mediaSetManager );

  signals:
    //! Emitted, when controller is finished.
    void finished();

  private slots:
    /**
     * @brief Slot called, when user has selected %Media Set Configuration.
     **/
    void directorySelected();

    /**
     * @brief View %Media Set Button
     *
     * @param[in] index
     *   Model Index of Selected %Media Set.
     **/
    void viewMediaSet( const QModelIndex &index );

    /**
     * @brief Import Media Set Slot
     **/
    void importMediaSet();

    /**
     * @brief Import Media Set XML Slot
     **/
    void importMediaSetXml();

    /**
     * @brief Remove Media Set XML.
     *
     * @param[in] index
     *   Model Index of Selected %Media Set.
     **/
    void removeMediaSet( const QModelIndex &index );

  private:
    //! Media Sets Model
    std::unique_ptr< Media::MediaSetsModel > mediaSetsModelV;
    //! Media Set Manager Dialog
    std::unique_ptr< MediaSetManagerDialog > mediaSetManagerDialogV;
    //! Select Media Set Manager Directory Dialog
    std::unique_ptr< QFileDialog > selectMediaSetDirectoryDialogV;

    //! Media Set Manager
    Arinc665::Utils::JsonMediaSetManagerPtr mediaSetManagerV;
};

}

#endif

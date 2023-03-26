/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ViewMediaSetXmlAction.
 **/

#ifndef ARINC665_QT_VIEW_MEDIA_SET_XML_VIEWMEDIASETXMLACTION_HPP
#define ARINC665_QT_VIEW_MEDIA_SET_XML_VIEWMEDIASETXMLACTION_HPP

#include <arinc665_qt/view_media_set_xml/ViewMediaSetXml.hpp>

#include <arinc665_qt/media/Media.hpp>

#include <arinc665/media/Media.hpp>

#include <arinc665/files/Files.hpp>

#include <arinc665/utils/Utils.hpp>

#include <QObject>
#include <QFileDialog>
#include <QWidget>

#include <filesystem>
#include <memory>

namespace Arinc665Qt {

/**
 * @brief %Media Set XML Load and Display Action.
 **/
class ARINC665_QT_EXPORT ViewMediaSetXmlAction : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Controller.
     *
     * @param[in] parent
     *   Parent Widget.
     **/
    explicit ViewMediaSetXmlAction( QWidget * parent = nullptr );

    //! Destructor
    ~ViewMediaSetXmlAction() override;

  signals:
    /**
     * @brief Signal, which is emitted when the dialog has been closed or the
     *   operation aborted.
     **/
    void finished();

  public slots:
    /**
     * @brief Starts the BHM analysing session.
     **/
    void start();

    /**
     * @brief Starts the Media Set View with the given Media Set.
     *
     * @param[in] mediaSet
     *   Media Set
     * @param[in] filePathMapping
     *   File Path Mapping
     **/
    void start(
      Arinc665::Media::ConstMediaSetPtr mediaSet,
      Arinc665::Utils::FilePathMapping filePathMapping );

  private slots:
    /**
     * @brief Called, when the user has selected a file.
     *
     * @param[in] file
     *   Selected file.
     **/
    void fileSelected( const QString &file );

  private:
    //! Media Set Model
    std::unique_ptr< Media::MediaSetModel > mediaSetModelV;
    //! File Path Mapping Model
    std::unique_ptr< FilePathMappingModel > filePathMappingModelV;

    //! Select Media Set XML file dialog
    std::unique_ptr< QFileDialog > selectMediaSetXmlDialogV;
    //! Media Set XML Dialog
    std::unique_ptr< ViewMediaSetXmlDialog > viewMediaSetXmlDialogV;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetXmlController.
 **/

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLCONTROLLER_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLCONTROLLER_HPP

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXml.hpp>

#include <QWidget>

#include <memory>

namespace Arinc665Qt {

//! Import Media Set XML Controller
class ARINC665_QT_EXPORT ImportMediaSetXmlController : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the %Media Set Import XML Controller.
     *
     * @param[in] parent
     *   Parent Widget
     **/
    explicit ImportMediaSetXmlController( QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlController() override;

  signals:
    //! Emitted, when controller is finished.
    void finished();

  private:
    //! Wizard Dialog
    std::unique_ptr< ImportMediaSetXmlWizard> wizard;
};

}

#endif

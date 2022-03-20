/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetXmlWizard.
 **/

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLWIZARD_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLWIZARD_HPP

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXml.hpp>

#include <QWizard>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class ImportMediaSetXmlWizard;
}

//! Import Media Set XML Wizard
class ARINC665_QT_EXPORT ImportMediaSetXmlWizard : public QWizard
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Import Media Set XML Wizard.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ImportMediaSetXmlWizard( QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlWizard() override;

  private:
    //! UI (designer)
    std::unique_ptr< Ui::ImportMediaSetXmlWizard > ui;
};

}

#endif

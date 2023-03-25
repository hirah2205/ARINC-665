/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetXmlFinalPage.
 **/

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_IMPORTMEDIASETFINALPAGE_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_IMPORTMEDIASETFINALPAGE_HPP

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXml.hpp>

#include <QWizardPage>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class ImportMediaSetXmlFinalPage;
}

//! Compile %Media Set Final Page
class ARINC665_QT_EXPORT ImportMediaSetXmlFinalPage final : public QWizardPage
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Page.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ImportMediaSetXmlFinalPage( QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlFinalPage() override;

  private:
    //! UI (designer)
    std::unique_ptr< Ui::ImportMediaSetXmlFinalPage > ui;
};

}

#endif

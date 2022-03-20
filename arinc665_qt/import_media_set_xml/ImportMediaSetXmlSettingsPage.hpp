/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::ImportMediaSetXmlSettingsPage.
 **/

#ifndef ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLSETTINGSPAGE_HPP
#define ARINC665_QT_IMPORT_MEDIA_SET_XML_IMPORTMEDIASETXMLSETTINGSPAGE_HPP

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXml.hpp>

#include <QWizardPage>
#include <QFileDialog>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class ImportMediaSetXmlSettingsPage;
}

//! Import %Media Set XML Settings Page
class ARINC665_QT_EXPORT ImportMediaSetXmlSettingsPage : public QWizardPage
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Import Media Set XML Settings Page.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit ImportMediaSetXmlSettingsPage( QWidget * parent = nullptr );

    //! Destructor
    ~ImportMediaSetXmlSettingsPage() override;

  private slots:
    //! Slot Select XML File
    void selectXmlFile();

    //! Slot Select Input Directory
    void selectInputDirectory();

  private:
    //! UI (designer)
    std::unique_ptr< Ui::ImportMediaSetXmlSettingsPage > ui;
    //! Select XML File Dialog
    std::unique_ptr< QFileDialog > selectXmlFileDialog;
    //! Select Input Directory Dialog
    std::unique_ptr< QFileDialog > selectInputDirectoryDialog;
};

}

#endif

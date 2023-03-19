/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::MediaPathsPage.
 **/

#ifndef ARINC665_QT_MEDIAPATHSPAGE_HPP
#define ARINC665_QT_MEDIAPATHSPAGE_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <QWizardPage>
#include <QFileDialog>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class MediaPathsPage;
}

//! Import %Media Set XML Settings Page
class ARINC665_QT_EXPORT MediaPathsPage final : public QWizardPage
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Import Media Set XML Settings Page.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaPathsPage( QWidget * parent = nullptr );

    //! Destructor
    ~MediaPathsPage() override;

  private slots:

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaPathsPage > ui;
    //! Select XML File Dialog
    std::unique_ptr< QFileDialog > selectMediaPathDialog;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665Qt::CompileMediaSetFinalPage.
 **/

#ifndef ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETFINALPAGE_HPP
#define ARINC665_QT_COMPILE_MEDIA_SET_COMPILEMEDIASETFINALPAGE_HPP

#include <arinc665_qt/compile_media_set/CompileMediaSet.hpp>

#include <QWizardPage>

#include <memory>

namespace Arinc665Qt {

namespace Ui{
class CompileMediaSetFinalPage;
}

//! Compile %Media Set Final Page
class ARINC665_QT_EXPORT CompileMediaSetFinalPage final : public QWizardPage
{
    Q_OBJECT

  public:
    /**
     * @brief Initialises the Page.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit CompileMediaSetFinalPage( QWidget * parent = nullptr );

    //! Destructor
    ~CompileMediaSetFinalPage() override;

  private:
    //! UI (designer)
    std::unique_ptr< Ui::CompileMediaSetFinalPage > ui;
};

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::CompileMediaSetFinalPage.
 **/

#include "CompileMediaSetFinalPage.hpp"

#include "ui_CompileMediaSetFinalPage.h"

namespace Arinc665Qt {

CompileMediaSetFinalPage::CompileMediaSetFinalPage(
  QWidget * const parent ) :
  QWizardPage{ parent },
  ui{ std::make_unique< Ui::CompileMediaSetFinalPage >() }
{
  ui->setupUi( this );
}

CompileMediaSetFinalPage::~CompileMediaSetFinalPage() = default;

}

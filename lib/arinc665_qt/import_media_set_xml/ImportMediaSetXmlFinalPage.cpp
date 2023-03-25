/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlFinalPage.
 **/

#include "ImportMediaSetXmlFinalPage.hpp"

#include "ui_ImportMediaSetXmlFinalPage.h"

namespace Arinc665Qt {

ImportMediaSetXmlFinalPage::ImportMediaSetXmlFinalPage(
  QWidget * const parent ) :
  QWizardPage{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlFinalPage >() }
{
  ui->setupUi( this );
}

ImportMediaSetXmlFinalPage::~ImportMediaSetXmlFinalPage() = default;

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlWizard.
 **/

#include "ImportMediaSetXmlWizard.hpp"

#include "ui_ImportMediaSetXmlWizard.h"

namespace Arinc665Qt {

ImportMediaSetXmlWizard::ImportMediaSetXmlWizard( QWidget * const parent ) :
  QWizard{ parent },
  ui{ std::make_unique< Ui::ImportMediaSetXmlWizard >() }
{
  ui->setupUi( this );
}

ImportMediaSetXmlWizard::~ImportMediaSetXmlWizard() = default;

}

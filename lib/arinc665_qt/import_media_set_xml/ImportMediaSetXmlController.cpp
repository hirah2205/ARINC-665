/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665Qt::ImportMediaSetXmlController.
 **/

#include "ImportMediaSetXmlController.hpp"

#include <arinc665_qt/import_media_set_xml/ImportMediaSetXmlWizard.hpp>

namespace Arinc665Qt {

ImportMediaSetXmlController::ImportMediaSetXmlController(
  QWidget * const parent ) :
  wizard{ std::make_unique< ImportMediaSetXmlWizard >( parent ) }
{
  wizard->show();

  connect(
    wizard.get(),
    &ImportMediaSetXmlWizard::finished,
    this,
    &ImportMediaSetXmlController::finished );
}

ImportMediaSetXmlController::~ImportMediaSetXmlController() = default;

}

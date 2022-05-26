/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class
 *   Arinc665Qt::MediaSetManager::RemoveMediaSetController.
 **/

#include "RemoveMediaSetController.hpp"

namespace Arinc665Qt::MediaSetManager {

RemoveMediaSetController::RemoveMediaSetController( QWidget * const parent  ) :
  QObject{ parent }
{
}

RemoveMediaSetController::~RemoveMediaSetController() = default;

void RemoveMediaSetController::start( Arinc665::Media::MediaSetPtr mediaSet )
{
}

}

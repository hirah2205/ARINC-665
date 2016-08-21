/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Importer::Arinc665Importer.
 **/

#include "Arinc665Importer.hpp"

#include <arinc665/utils/implementation/Arinc665ImporterImpl.hpp>

namespace Arinc665 {
namespace Utils {

Arinc665ImporterPtr Arinc665Importer::create( void)
{
  return std::make_shared< Arinc665ImporterImpl>();
}

}
}

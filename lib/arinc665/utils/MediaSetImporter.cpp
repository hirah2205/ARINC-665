/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetImporter.
 **/

#include "MediaSetImporter.hpp"

#include <arinc665/utils/implementation/MediaSetImporterImpl.hpp>

namespace Arinc665::Utils {

MediaSetImporterPtr MediaSetImporter::create()
{
  return std::make_unique< MediaSetImporterImpl >();
}

}

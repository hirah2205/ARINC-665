/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetManager.
 **/

#include "MediaSetManager.hpp"

#include <arinc665/utils/implementation/MediaSetManagerImpl.hpp>

namespace Arinc665::Utils {

MediaSetManagerPtr MediaSetManager::instance(
  const std::filesystem::path &basePath,
  MediaSetManagerConfiguration &configuration,
  const bool checkFileIntegrity )
{
  return std::make_shared< MediaSetManagerImpl >(
    basePath,
    configuration,
    checkFileIntegrity );
}

}

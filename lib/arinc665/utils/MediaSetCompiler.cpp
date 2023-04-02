/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::MediaSetCompiler.
 **/

#include "MediaSetCompiler.hpp"

#include <arinc665/utils/implementation/MediaSetCompilerImpl.hpp>

namespace Arinc665::Utils {

MediaSetCompilerPtr MediaSetCompiler::create()
{
  return std::make_unique< MediaSetCompilerImpl >();
}

}

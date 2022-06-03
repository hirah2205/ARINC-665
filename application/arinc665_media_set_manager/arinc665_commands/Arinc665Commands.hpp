/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Namespace Arinc665Commands.
 **/

#ifndef ARINC665_COMMANDS_ARINC665COMMANDS_HPP
#define ARINC665_COMMANDS_ARINC665COMMANDS_HPP

#include <commands/Commands.hpp>

/**
 * @brief ARINC 665 Commands.
 *
 * Commands, which provides ARINC 665 Media Set Manager functionality.
 **/
namespace Arinc665Commands {

/**
 * @brief Registers the ARINC 665 Commands to the registry.
 *
 * @param[in,out] registry
 *   Registry, where the Commands shall be registered to.
 **/
void registerCommands( const Commands::CommandRegistryPtr& registry );

}

#endif

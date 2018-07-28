/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief ARINC 665 compiler.
 **/

#include "Arinc665CompilerApplication.hpp"

#include <helper/Logger.hpp>

#include <cstdlib>
#include <memory>

/**
 * @brief Program entry point
 *
 * @param[in] argc
 *   Number of arguments.
 * @param[in] argv
 *   The arguments
 *
 * @return The success state of this operation.
 **/
int main( int argc, char ** argv);

int main( int argc, char ** argv)
{
  initLogging( severity_level::info);

  Arinc665CompilerApplication app;

  return app( argc, argv);
}

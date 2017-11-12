/*
 * $Date: 2017-05-21 18:23:14 +0200 (So, 21. Mai 2017) $
 * $Revision: 2037 $
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
 * @brief ARINC 665 decompiler.
 **/

#include "Arinc665DecompilerApplication.hpp"

#include <helper/Logger.hpp>

#include <boost/application.hpp>

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

  boost::application::context context;

  Arinc665DecompilerApplication app( context);

  context.insert< boost::application::args>(
    std::make_shared< boost::application::args >( argc, argv));

  return boost::application::launch< boost::application::common>(
    app,
    context);
}

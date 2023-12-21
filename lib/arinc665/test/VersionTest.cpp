// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Unit Tests for Class Arinc665::Version.
 **/

#include <arinc665/Version.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( Arinc665Test )
BOOST_AUTO_TEST_SUITE( Arinc665VersionTest )

//! Version Test
BOOST_AUTO_TEST_CASE( version )
{
  std::cout
    << Version::Key << " " << Version::Name
    << " Version: "
    << Version::Major << "."
    << Version::Minor << "."
    << Version::Patch << "-"
    << Version::Addition << "\n"
    << Version::Url << " " << Version::License << "\n";

  std::cout
    << "ARINC 665 Version Information: " << Version::VersionString << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}

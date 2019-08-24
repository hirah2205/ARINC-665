/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration/ definition of unit tests of class
 *   Arinc665::TargetHardwareId.
 **/

#include <arinc665/TargetHardwareId.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( TargetHardwareIdTest)

BOOST_AUTO_TEST_CASE( constructor1)
{
  std::string thwIdStr{ "12345678"};

  TargetHardwareId thwId( thwIdStr);

  BOOST_CHECK( thwId.get() == "12345678");
}

BOOST_AUTO_TEST_CASE( constructor2)
{
  TargetHardwareId thwId( "12345678"sv);

  BOOST_CHECK( thwId.get() == "12345678");
}

BOOST_AUTO_TEST_CASE( set)
{
  TargetHardwareId thwId( "12345678"sv);
  BOOST_CHECK( thwId.get() == "12345678");

  BOOST_CHECK_NO_THROW( thwId.set( ""sv));
  BOOST_CHECK( thwId.get() == "");

  BOOST_CHECK_NO_THROW( thwId.set( "1"sv));
  BOOST_CHECK( thwId.get() == "1");
}

BOOST_AUTO_TEST_SUITE_END()

}

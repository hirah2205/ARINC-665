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
 * @brief Declaration/ definition of unit tests of class
 *   Arinc665::ManufacturerCode.
 **/

#include <arinc665/ManufacturerCode.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( ManufacturerCodeTest)

BOOST_AUTO_TEST_CASE( constructor)
{
  ManufacturerCode manufacturerCode( "AAA");

  BOOST_CHECK( manufacturerCode.get() == "AAA");

  BOOST_CHECK_THROW( ManufacturerCode( ""), Arinc665Exception);
  BOOST_CHECK_THROW( ManufacturerCode( "A"), Arinc665Exception);
  BOOST_CHECK_THROW( ManufacturerCode( "AA"), Arinc665Exception);
  BOOST_CHECK_THROW( ManufacturerCode( "AAAA"), Arinc665Exception);
}

BOOST_AUTO_TEST_CASE( set)
{
  ManufacturerCode manufacturerCode( "AAA");
  BOOST_CHECK( manufacturerCode.get() == "AAA");

  BOOST_CHECK_THROW( manufacturerCode.set( ""), Arinc665Exception);
  BOOST_CHECK( manufacturerCode.get() == "AAA");

  BOOST_CHECK_THROW( manufacturerCode.set( "B"), Arinc665Exception);
  BOOST_CHECK( manufacturerCode.get() == "AAA");

  BOOST_CHECK_THROW( manufacturerCode.set( "BB"), Arinc665Exception);
  BOOST_CHECK( manufacturerCode.get() == "AAA");

  BOOST_CHECK_NO_THROW( manufacturerCode.set( "BBB"));
  BOOST_CHECK( manufacturerCode.get() == "BBB");

  BOOST_CHECK_THROW( manufacturerCode.set( "CCCC"), Arinc665Exception);
  BOOST_CHECK( manufacturerCode.get() == "BBB");
}

BOOST_AUTO_TEST_SUITE_END()

}

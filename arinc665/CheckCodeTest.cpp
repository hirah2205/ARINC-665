/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::CheckCode.
 **/

#include <arinc665/CheckCode.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( CheckCodeTest)

BOOST_AUTO_TEST_CASE( constructor)
{
  BOOST_CHECK( CheckCode( "01").get() == 0x01);
  BOOST_CHECK( CheckCode( "FF").get() == 0xFF);
  BOOST_CHECK( CheckCode( 0x01).get() == 0x01);
  BOOST_CHECK( CheckCode( 0xFF).get() == 0xFF);
}

BOOST_AUTO_TEST_CASE( set)
{
  CheckCode checkCode( 0x00);
  BOOST_CHECK( checkCode.get() == 0x00);

  BOOST_CHECK_NO_THROW( checkCode.set( 0x01));
  BOOST_CHECK( checkCode.get() == 0x01);

  BOOST_CHECK_NO_THROW( checkCode.set( "FF"));
  BOOST_CHECK( checkCode.get() == 0xFF);

  BOOST_CHECK_THROW( checkCode.set( ""), Arinc665Exception);
  BOOST_CHECK_THROW( checkCode.set( "1"), Arinc665Exception);
  BOOST_CHECK_THROW( checkCode.set( "123"), Arinc665Exception);

  BOOST_CHECK_THROW( checkCode.set( "ZZ"), std::invalid_argument);
  BOOST_CHECK( checkCode.get() == 0xFF);
}

BOOST_AUTO_TEST_CASE( getStr)
{
  CheckCode checkCode( 0x00);

  BOOST_CHECK( checkCode.get() == 0x00);
  BOOST_CHECK( checkCode.getStr() == "00");

  BOOST_CHECK_NO_THROW( checkCode.set( 0xFF));
  BOOST_CHECK( checkCode.get() == 0xFF);
  BOOST_CHECK( checkCode.getStr() == "FF");
}

BOOST_AUTO_TEST_CASE( compare)
{
  BOOST_CHECK( CheckCode(0x00) == CheckCode(0x00));
  BOOST_CHECK( (CheckCode(0x00) != CheckCode(0x00)) == false);

  BOOST_CHECK( CheckCode(0xFF) == CheckCode(0xFF));
  BOOST_CHECK( (CheckCode(0xFF) != CheckCode(0xFF)) == false);

  BOOST_CHECK( (CheckCode(0xFF) == CheckCode(0x00)) == false);
  BOOST_CHECK( CheckCode(0xFF) != CheckCode(0x00));
}

BOOST_AUTO_TEST_SUITE_END()

}

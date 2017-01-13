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
 * @brief Definition of unit tests for the class Arinc665::CheckCode.
 **/

#include <arinc665/CheckCode.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( CheckCodeTest)

BOOST_AUTO_TEST_CASE( testCheckCode1)
{
  CheckCode checkCode1( "01");
  CheckCode checkCode2( "20");
  CheckCode checkCode3( 0x01);
  CheckCode checkCode4( 0x20);

  BOOST_CHECK( checkCode1.get() == 0x01);
  BOOST_CHECK( checkCode1.getStr() == "01");
  BOOST_CHECK( checkCode2.get() == 0x20);
  BOOST_CHECK( checkCode2.getStr() == "20");
  BOOST_CHECK( checkCode3.get() == 0x01);
  BOOST_CHECK( checkCode3.getStr() == "01");
  BOOST_CHECK( checkCode4.get() == 0x20);
  BOOST_CHECK( checkCode4.getStr() == "20");

  BOOST_CHECK( checkCode1 == checkCode1);
  BOOST_CHECK( checkCode1 != checkCode2);
  BOOST_CHECK( checkCode1 == checkCode3);
  BOOST_CHECK( checkCode1 != checkCode4);

  BOOST_CHECK( checkCode2 != checkCode1);
  BOOST_CHECK( checkCode2 == checkCode2);
  BOOST_CHECK( checkCode2 != checkCode3);
  BOOST_CHECK( checkCode2 == checkCode4);

  BOOST_CHECK( checkCode3 == checkCode1);
  BOOST_CHECK( checkCode3 != checkCode2);
  BOOST_CHECK( checkCode3 == checkCode3);
  BOOST_CHECK( checkCode3 != checkCode4);

  BOOST_CHECK( checkCode4 != checkCode1);
  BOOST_CHECK( checkCode4 == checkCode2);
  BOOST_CHECK( checkCode4 != checkCode3);
  BOOST_CHECK( checkCode4 == checkCode4);
}

BOOST_AUTO_TEST_SUITE_END()

}

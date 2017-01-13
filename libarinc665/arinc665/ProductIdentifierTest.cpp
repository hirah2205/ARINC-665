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
 *   Arinc665::ProductIndentifier.
 **/

#include <arinc665/ProductIdentifier.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( ProductIdentifierTest)

BOOST_AUTO_TEST_CASE( constructor)
{
  ProductIdentifier productIdentifier( "12345678");

  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( ProductIdentifier( ""), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "1"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "12"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "123"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "1234"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "12345"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "123456"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "1234567"), Arinc665Exception);
  BOOST_CHECK_THROW( ProductIdentifier( "123456789"), Arinc665Exception);
}

BOOST_AUTO_TEST_CASE( set)
{
  ProductIdentifier productIdentifier( "12345678");
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( ""), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "1"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "12"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "123"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "12345"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "123456"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_THROW( productIdentifier.set( "1234567"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "12345678");

  BOOST_CHECK_NO_THROW( productIdentifier.set( "1234567A"));
  BOOST_CHECK( productIdentifier.get() == "1234567A");

  BOOST_CHECK_THROW( productIdentifier.set( "123456789"), Arinc665Exception);
  BOOST_CHECK( productIdentifier.get() == "1234567A");
}

BOOST_AUTO_TEST_SUITE_END()

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::PartNumber.
 **/

#include <boost/test/unit_test.hpp>

#include <arinc665/partnumber/PartNumber.hpp>

#include <helper/Dump.hpp>
#include <helper/Logger.hpp>

#include <iostream>

using namespace Arinc665;

static void testPartNumber1( void)
{
  //EBE3B549720000D
  ManufacturerCode mc( "EBE");
  ProductIdentifier pi( "54972000");
  PartNumber pn( mc, pi);

  BOOST_CHECK( pn.getManufacturerCode().get()  == "EBE");
  BOOST_CHECK( pn.getProductIdentifier().get() == "54972000");

  BOOST_CHECK( pn.getPartNumber() ==
    pn.getManufacturerCode().get() +
    pn.getCheckCode().getStr() +
    pn.getProductIdentifier().get());
}

static void testPartNumber2( void)
{
  PartNumber pn( "EBE4F54972000");

  BOOST_CHECK( pn.getManufacturerCode().get() == "EBE");
  BOOST_CHECK( pn.getProductIdentifier().get() == "54972000");

  BOOST_CHECK( pn.getPartNumber() == "EBE4F54972000");
}

void testPartNumber( void)
{
  boost::unit_test::framework::master_test_suite().add( BOOST_TEST_CASE( &testPartNumber1));
  boost::unit_test::framework::master_test_suite().add( BOOST_TEST_CASE( &testPartNumber2));
}

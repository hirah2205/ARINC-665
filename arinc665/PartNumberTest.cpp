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

#include <arinc665/PartNumber.hpp>

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( PartNumberTest)

//! part number test
BOOST_AUTO_TEST_CASE( testPartNumber1)
{
  ManufacturerCode mc( "EBE"sv);
  ProductIdentifier pi( "54972000"sv);
  PartNumber pn( mc, pi);

  BOOST_CHECK( pn.manufacturerCode().get()  == "EBE");
  BOOST_CHECK( pn.productIdentifier().get() == "54972000");

  BOOST_CHECK( pn.partNumber() ==
    std::string{ pn.manufacturerCode().get()} +
    std::string{ pn.checkCode().getStr()} +
    std::string{ pn.productIdentifier().get()});
}

//! part number test
BOOST_AUTO_TEST_CASE( testPartNumber2)
{
  PartNumber pn{ "EBE4F54972000"};

  BOOST_CHECK( pn.manufacturerCode().get() == "EBE");
  BOOST_CHECK( pn.productIdentifier().get() == "54972000");

  BOOST_CHECK( pn.partNumber() == "EBE4F54972000");
}

BOOST_AUTO_TEST_SUITE_END()
}

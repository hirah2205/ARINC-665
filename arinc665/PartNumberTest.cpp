/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of unit tests for the class Arinc665::PartNumber.
 **/

#include <arinc665/PartNumber.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

namespace Arinc665 {

BOOST_AUTO_TEST_SUITE( Arinc665Test)
BOOST_AUTO_TEST_SUITE( PartNumberTest)

//! part number test
BOOST_AUTO_TEST_CASE( testPartNumber1)
{
  PartNumber pn{ "EBE", "54972000"};

  BOOST_CHECK( pn.manufacturerCode()  == "EBE");
  BOOST_CHECK( pn.productIdentifier() == "54972000");

  BOOST_CHECK( pn.partNumber() ==
    std::string{ pn.manufacturerCode()} +
    std::string{ pn.checkCode()} +
    std::string{ pn.productIdentifier()});

  BOOST_CHECK_THROW( (PartNumber{ "", "54972000"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "A", "54972000"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "AA", "54972000"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "AAAA", "54972000"}), Arinc665Exception);


  BOOST_CHECK_THROW( (PartNumber{ "EBE", ""}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "1"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "12"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "123"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "1234"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "12345"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "123456"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "1234567"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "EBE", "123456789"}), Arinc665Exception);

}

//! part number test
BOOST_AUTO_TEST_CASE( testPartNumber2)
{
  PartNumber pn{ "EBE4F54972000"};

  BOOST_CHECK( pn.manufacturerCode() == "EBE");
  BOOST_CHECK( pn.productIdentifier() == "54972000");

  BOOST_CHECK( pn.partNumber() == "EBE4F54972000");

  BOOST_CHECK_THROW( (PartNumber{ ""}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "01"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "012"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "01234"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "012345"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "01234567"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "012345678"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456789"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456789A"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456789AB"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456789ABC"}), Arinc665Exception);
//  BOOST_CHECK_THROW( (PartNumber{ "0123456789ABCD"}), Arinc665Exception);
  BOOST_CHECK_THROW( (PartNumber{ "0123456789ABCDE"}), Arinc665Exception);
}

BOOST_AUTO_TEST_CASE( manufacturerCodeTest)
{
  PartNumber pn{ "AAA", "12345678"};
  BOOST_CHECK( pn.manufacturerCode() == "AAA");

  BOOST_CHECK_THROW( pn.manufacturerCode( ""sv), Arinc665Exception);
  BOOST_CHECK( pn.manufacturerCode() == "AAA");

  BOOST_CHECK_THROW( pn.manufacturerCode( "B"sv), Arinc665Exception);
  BOOST_CHECK( pn.manufacturerCode() == "AAA");

  BOOST_CHECK_THROW( pn.manufacturerCode( "BB"sv), Arinc665Exception);
  BOOST_CHECK( pn.manufacturerCode() == "AAA");

  BOOST_CHECK_NO_THROW( pn.manufacturerCode( "BBB"sv));
  BOOST_CHECK( pn.manufacturerCode() == "BBB");

  BOOST_CHECK_THROW( pn.manufacturerCode( "CCCC"sv), Arinc665Exception);
  BOOST_CHECK( pn.manufacturerCode() == "BBB");
}

BOOST_AUTO_TEST_CASE( productIdentifierTest)
{
  PartNumber pn{ "ABC", "12345678"};

  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( ""sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "1"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "12"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "123"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "12345"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "123456"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_THROW( pn.productIdentifier( "1234567"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "12345678");

  BOOST_CHECK_NO_THROW( pn.productIdentifier( "1234567A"sv));
  BOOST_CHECK( pn.productIdentifier() == "1234567A");

  BOOST_CHECK_THROW( pn.productIdentifier( "123456789"sv), Arinc665Exception);
  BOOST_CHECK( pn.productIdentifier() == "1234567A");
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}

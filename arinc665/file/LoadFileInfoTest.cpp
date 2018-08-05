/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Unit test of class Arinc665::File::LoadFileInfo.
 **/

#include <arinc665/file/LoadFileInfo.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665::File {

BOOST_AUTO_TEST_SUITE( LoadFileInfoTest)

//! Constructor test
BOOST_AUTO_TEST_CASE( constructor1)
{
  std::string name( "NAME");
  std::string partNumber( "PART_NUMBER");

  LoadFileInfo loadFileInfo( name, partNumber, 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( name == "NAME");
  BOOST_CHECK( partNumber == "PART_NUMBER");

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! Constructor test
BOOST_AUTO_TEST_CASE( constructor2)
{
  std::string name( "NAME");
  std::string partNumber( "PART_NUMBER");

  LoadFileInfo loadFileInfo(
    std::move( name),
    std::move( partNumber),
    0xDEADBEEFUL,
    0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! get/set name test
BOOST_AUTO_TEST_CASE( GetSetName)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());

  BOOST_CHECK_NO_THROW( loadFileInfo.filename( "NAME2"));

  BOOST_CHECK( loadFileInfo.filename() == "NAME2");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! get/set partnumber test
BOOST_AUTO_TEST_CASE( GetSetPartNumber)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());

  BOOST_CHECK_NO_THROW( loadFileInfo.partNumber( "PART_NUMBER2"));

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER2");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! get/set length test
BOOST_AUTO_TEST_CASE( GetSetLength)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());

  BOOST_CHECK_NO_THROW( loadFileInfo.length( 0xAA55AA55UL));

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xAA55AA55UL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! get/set crc test
BOOST_AUTO_TEST_CASE( GetSetCrc)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());

  BOOST_CHECK_NO_THROW( loadFileInfo.crc( 0xAA55U));

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xAA55U);
  BOOST_CHECK( !loadFileInfo.checkValue());
}

//! get/set check value test
BOOST_AUTO_TEST_CASE( GetSetCheckValue)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( !loadFileInfo.checkValue());

  BOOST_CHECK_NO_THROW(
    loadFileInfo.checkValue(
      std::make_tuple(
        CheckValueType::Crc8,
        std::vector< uint8_t>{ 0x12, 0x34})));

  BOOST_CHECK( loadFileInfo.filename() == "NAME");
  BOOST_CHECK( loadFileInfo.partNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.length() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.crc() == 0xBABEU);
  BOOST_CHECK( loadFileInfo.checkValue());
  BOOST_CHECK( std::get< 0>( *loadFileInfo.checkValue()) == CheckValueType::Crc8);
  BOOST_CHECK( std::get< 1>( *loadFileInfo.checkValue()) == std::vector< uint8_t>({ 0x12, 0x34}));
}

BOOST_AUTO_TEST_SUITE_END()

}

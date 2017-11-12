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

namespace Arinc665 {
namespace File {

BOOST_AUTO_TEST_SUITE( LoadFileInfoTest)

  BOOST_AUTO_TEST_CASE( constructor1)
  {
    std::string name( "NAME");
    std::string partNumber( "PART_NUMBER");

    LoadFileInfo loadFileInfo( name, partNumber, 0xDEADBEEFUL, 0xBABEU);

    BOOST_CHECK( name == "NAME");
    BOOST_CHECK( partNumber == "PART_NUMBER");

    BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
    BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
    BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
    BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);
  }

BOOST_AUTO_TEST_CASE( constructor2)
{
  std::string name( "NAME");
  std::string partNumber( "PART_NUMBER");

  LoadFileInfo loadFileInfo(
    std::move( name),
    std::move( partNumber),
    0xDEADBEEFUL,
    0xBABEU);

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);
}

BOOST_AUTO_TEST_CASE( GetSetName)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);

  BOOST_CHECK_NO_THROW( loadFileInfo.setFilename( "NAME2"));

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME2");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);
}

BOOST_AUTO_TEST_CASE( GetSetPartNumber)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);

  BOOST_CHECK_NO_THROW( loadFileInfo.setPartNumber( "PART_NUMBER2"));

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER2");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);
}

BOOST_AUTO_TEST_CASE( GetSetLength)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);

  BOOST_CHECK_NO_THROW( loadFileInfo.setLength( 0xAA55AA55UL));

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xAA55AA55UL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);
}

BOOST_AUTO_TEST_CASE( GetSetCrc)
{
  LoadFileInfo loadFileInfo( "NAME", "PART_NUMBER", 0xDEADBEEFUL, 0xBABEU);

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xBABEU);

  BOOST_CHECK_NO_THROW( loadFileInfo.setCrc( 0xAA55U));

  BOOST_CHECK( loadFileInfo.getFilename() == "NAME");
  BOOST_CHECK( loadFileInfo.getPartNumber() == "PART_NUMBER");
  BOOST_CHECK( loadFileInfo.getLength() == 0xDEADBEEFUL);
  BOOST_CHECK( loadFileInfo.getCrc() == 0xAA55U);
}
BOOST_AUTO_TEST_SUITE_END()

}
}

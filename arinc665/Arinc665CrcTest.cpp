/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of unit tests for the ARINC 665 CRCs.
 **/

#include <arinc665/Arinc665Crc.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/endian/conversion.hpp>

#include <vector>
#include <numeric>
#include <string>

namespace Arinc665 {

//! ARINC CRC data
static struct ArincCrcData
{
  std::vector< uint8_t> t02a;
  std::vector< uint8_t> t03a;
  std::vector< uint16_t> t04a;
  std::vector< uint8_t> t11a;
  std::string t12a;
  std::string t13a;

  ArincCrcData():
    t02a( 128, 0xFF),
    t03a( 100, 0),
    t04a( 128, boost::endian::native_to_big< uint16_t>( 0xAA55)), // must be swapped (because endianess)
    t11a( 256),
    t12a( "ACM12345678"),
    t13a( "ABCDEFGHIJKLMNO")
  {
    std::iota( t11a.begin(), t11a.end(), 0);
  }
} const arincCrcData;

BOOST_AUTO_TEST_SUITE( Arinc665CrcTest)

//! CRC16 test
BOOST_AUTO_TEST_CASE( crc16)
{
  Arinc665Crc16 arincCrc16;

  arincCrc16.reset();
  BOOST_CHECK( 0xFFFF == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t02a.begin()),
    arincCrcData.t02a.size());
  BOOST_CHECK( 0x1DA3 == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t03a.begin()),
    arincCrcData.t03a.size());
  BOOST_CHECK( 0x4634 == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t04a.begin()),
    arincCrcData.t04a.size()*2);
  BOOST_CHECK( 0x1D7E == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t11a.begin()),
    arincCrcData.t11a.size());
  BOOST_CHECK( 0x3FBD == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t12a.begin()),
    arincCrcData.t12a.size());
//  BOOST_CHECK( 0x3FBD == arincCrc16.checksum());

  arincCrc16.reset();
  arincCrc16.process_bytes(
    &(*arincCrcData.t13a.begin()),
    arincCrcData.t13a.size());
//  BOOST_CHECK( 0x3FBD == arincCrc16.checksum());
}

//! CRC32 test
BOOST_AUTO_TEST_CASE( crc32)
{
  Arinc665Crc32 arincCrc32;

  arincCrc32.reset();
  BOOST_CHECK( 0x00000000UL == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t02a.begin()),
    arincCrcData.t02a.size());
  BOOST_CHECK( 0x322AB4A6 == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t03a.begin()),
    arincCrcData.t03a.size());
  BOOST_CHECK( 0x53631199 == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t04a.begin()),
    arincCrcData.t04a.size()*2);
  BOOST_CHECK( 0xC2F270BC == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t11a.begin()),
    arincCrcData.t11a.size());
  BOOST_CHECK( 0xB6B5EE95UL == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t12a.begin()),
    arincCrcData.t12a.size());
//  BOOST_CHECK( 0x3FBD == arincCrc32.checksum());

  arincCrc32.reset();
  arincCrc32.process_bytes(
    &(*arincCrcData.t13a.begin()),
    arincCrcData.t13a.size());
//  BOOST_CHECK( 0x3FBD == arincCrc32.checksum());
}

//! CRC64 test
BOOST_AUTO_TEST_CASE( crc64)
{
  Arinc665Crc64 arincCrc64;

  arincCrc64.reset();
  BOOST_CHECK( 0x0000000000000000ULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t02a.begin()),
    arincCrcData.t02a.size());
  BOOST_CHECK( 0x034528B5989BED4DULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t03a.begin()),
    arincCrcData.t03a.size());
  BOOST_CHECK( 0x5B2ACFD2703ED63DULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t04a.begin()),
    arincCrcData.t04a.size()*2);
  BOOST_CHECK( 0x428A028B474233E4ULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t11a.begin()),
    arincCrcData.t11a.size());
  BOOST_CHECK( 0x59C3325B2927A19AULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t12a.begin()),
    arincCrcData.t12a.size());
//  BOOST_CHECK( 0x59C3325B2927A19AULL == arincCrc64.checksum());

  arincCrc64.reset();
  arincCrc64.process_bytes(
    &(*arincCrcData.t13a.begin()),
    arincCrcData.t13a.size());
//  BOOST_CHECK( 0x59C3325B2927A19AULL == arincCrc64.checksum());
}

BOOST_AUTO_TEST_SUITE_END()

}


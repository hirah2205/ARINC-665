// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Unit test of Check Value Utilities.
 **/

#include <arinc_665/files/CheckValueUtils.hpp>

#include <arinc_665/Arinc665Exception.hpp>

#include <arinc_645/CheckValue.hpp>

#include <helper/Dump.hpp>
#include <helper/Endianess.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665::Files {

BOOST_AUTO_TEST_SUITE( Arinc665Test )
BOOST_AUTO_TEST_SUITE( FilesTest )
BOOST_AUTO_TEST_SUITE( CheckValueUtilsTest )

//! CheckValueUtils_encode Test
BOOST_AUTO_TEST_CASE( CheckValueUtils_encode1 )
{
  using Helper::operator""_b;

  BOOST_CHECK( CheckValueUtils_encode( Arinc645::CheckValue::NoCheckValue ) == RawData( { 0x00_b, 0x00_b } ) );
  BOOST_CHECK( ( CheckValueUtils_encode(
    Arinc645::CheckValue{
      Arinc645::CheckValueType::Crc8,
      RawData{ 0x12_b, 0x34_b } } ) == RawData{ 0x00_b, 0x06_b, 0x00_b, 0x01_b, 0x12_b, 0x34_b } ) );
}

//! CheckValueUtils_decode Test
BOOST_AUTO_TEST_CASE( CheckValueUtils_decode1 )
{
  using Helper::operator""_b;

  const auto cv1a{ CheckValueUtils_decode( RawData{ 0x00_b, 0x00_b } ) };

  BOOST_CHECK( ( cv1a == Arinc645::CheckValue{ Arinc645::CheckValue::NoCheckValue } ) );

  const auto cv1b{ CheckValueUtils_decode( RawData{ 0x00_b, 0x04_b, 0x00_b, 0x00_b } ) };

  BOOST_CHECK( ( cv1b == Arinc645::CheckValue{ Arinc645::CheckValue::NoCheckValue } ) );

  const auto cv2{ CheckValueUtils_decode( RawData{ 0x00_b, 0x06_b, 0x00_b, 0x01_b, 0x12_b, 0x34_b } ) };

  BOOST_CHECK( ( cv2 == Arinc645::CheckValue{ Arinc645::CheckValueType::Crc8, RawData{ 0x12_b, 0x34_b } } ) );

  const auto cv3{ CheckValueUtils_decode( RawData{ 0x00_b, 0x06_b, 0x00_b, 0x02_b, 0x12_b, 0x34_b } ) };

  BOOST_CHECK( ( cv3 == Arinc645::CheckValue{ Arinc645::CheckValueType::Crc16, RawData{ 0x12_b, 0x34_b } } ) );

  BOOST_CHECK_THROW( (void)CheckValueUtils_decode( RawData{} ), Arinc665Exception );

  BOOST_CHECK_THROW( (void)CheckValueUtils_decode( RawData{ 0x00_b } ), Arinc665Exception );

  BOOST_CHECK_THROW( (void)CheckValueUtils_decode( RawData{ 0x00_b, 0x05_b, 0x00_b, 0x01_b, 0x12_b } ), Arinc665Exception );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}

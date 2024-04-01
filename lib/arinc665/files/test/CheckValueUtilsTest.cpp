// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Unit test of Check Value Utilities.
 **/

#include <arinc665/files/CheckValueUtils.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <arinc_645/CheckValue.hpp>

#include <helper/Dump.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Arinc665::Files {

BOOST_AUTO_TEST_SUITE( Arinc665Test)
BOOST_AUTO_TEST_SUITE( FilesTest)
BOOST_AUTO_TEST_SUITE( CheckValueUtilsTest)

//! CheckValueUtils_encode Test
BOOST_AUTO_TEST_CASE( CheckValueUtils_encode1 )
{
  BOOST_CHECK( CheckValueUtils_encode( Arinc645::CheckValue::NoCheckValue ) == RawFile({ 0x00, 0x00} ) );
  BOOST_CHECK( CheckValueUtils_encode(
    Arinc645::CheckValue{
      Arinc645::CheckValueType::Crc8,
      RawFile( { 0x12, 0x34 } ) } ) == RawFile({ 0x00, 0x06, 0x00, 0x01, 0x12, 0x34 } ) );
}

//! CheckValueUtils_decode Test
BOOST_AUTO_TEST_CASE( CheckValueUtils_decode1 )
{
  const auto cv1a{ CheckValueUtils_decode( RawFile{ 0x00, 0x00 } ) };

  BOOST_CHECK( ( cv1a ==
    Arinc645::CheckValue{ Arinc645::CheckValue::NoCheckValue } ) );

  const auto cv1b{ CheckValueUtils_decode( RawFile{ 0x00, 0x04, 0x00, 0x00 } ) };

  BOOST_CHECK( ( cv1b ==
    Arinc645::CheckValue{ Arinc645::CheckValue::NoCheckValue } ) );

  const auto cv2{
    CheckValueUtils_decode( RawFile{ 0x00, 0x06, 0x00, 0x01, 0x12, 0x34 } ) };

  BOOST_CHECK( ( cv2 ==
   Arinc645::CheckValue{ Arinc645::CheckValueType::Crc8, RawFile{ 0x12, 0x34 } } ) );

  const auto cv3{
    CheckValueUtils_decode( RawFile{ 0x00, 0x06, 0x00, 0x02, 0x12, 0x34 } ) };

  BOOST_CHECK( ( cv3 ==
    Arinc645::CheckValue{ Arinc645::CheckValueType::Crc16, RawFile{ 0x12, 0x34 } } ) );

  BOOST_CHECK_THROW(
    (void)CheckValueUtils_decode( RawFile{} ),
    Arinc665Exception );

  BOOST_CHECK_THROW(
    (void)CheckValueUtils_decode( RawFile{ 0x00 } ),
    Arinc665Exception );

  BOOST_CHECK_THROW(
    (void)CheckValueUtils_decode( RawFile{ 0x00, 0x05, 0x00, 0x01, 0x12 } ),
    Arinc665Exception );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}

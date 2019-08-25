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

#include <arinc665/file/CheckValueUtils.hpp>

#include <helper/Dump.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Arinc665::File {

BOOST_AUTO_TEST_SUITE( CheckValueUtilsTest)


BOOST_AUTO_TEST_CASE( CheckValueUtils_encode1)
{
  BOOST_CHECK( CheckValueUtils_encode( {}) == RawFile({ 0x00, 0x00}));
  BOOST_CHECK( CheckValueUtils_encode(
    std::make_tuple(
      CheckValueType::Crc8,
      RawFile({ 0x12, 0x34}))) == RawFile({ 0x00, 0x06, 0x00, 0x01, 0x12, 0x34}));
}

BOOST_AUTO_TEST_CASE( CheckValueUtils_decode1)
{
  BOOST_CHECK( CheckValueUtils_decode( RawFile({0x00, 0x00}), 0) == std::optional< CheckValue>());
  BOOST_CHECK( CheckValueUtils_decode( RawFile({ 0x00, 0x06, 0x00, 0x01, 0x12, 0x34}),0) ==
    std::make_tuple(
      CheckValueType::Crc8,
      RawFile({ 0x12, 0x34})));
}

BOOST_AUTO_TEST_SUITE_END()

}

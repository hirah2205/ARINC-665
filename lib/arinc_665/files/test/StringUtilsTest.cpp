// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Unit Tests for Module Arinc665::Files::StringUtils.
 **/

#include <arinc_665/files/StringUtils.hpp>

#include <arinc_665/Arinc665Exception.hpp>

#include <boost/test/unit_test.hpp>

namespace Arinc665::Files {

BOOST_AUTO_TEST_SUITE( Arinc665Test )
BOOST_AUTO_TEST_SUITE( FilesTest )
BOOST_AUTO_TEST_SUITE( SringUtilsTest )

//! Decode String Test
BOOST_AUTO_TEST_CASE( decodeString )
{
  ConstRawFileSpan remaining;
  std::string out;

  std::tie( remaining, out ) = StringUtils_decodeString( ConstRawFileSpan{ { 0x00, 0x00 } } );
  BOOST_CHECK( remaining.empty() );
  BOOST_CHECK( out.empty() );

  std::tie( remaining, out ) = StringUtils_decodeString( ConstRawFileSpan{ { 0x00, 0x04, 'T', 'e', 's', 't' } } );
  BOOST_CHECK( remaining.empty() );
  BOOST_CHECK( out == "Test" );

  std::tie( remaining, out ) =
    StringUtils_decodeString( ConstRawFileSpan{ { 0x00, 0x05, 'T', 'e', 's', 't', '1', 0x00 } } );
  BOOST_CHECK( remaining.empty() );
  BOOST_CHECK( out == "Test1" );

  BOOST_CHECK_THROW(
    StringUtils_decodeString( ConstRawFileSpan{ { 0x00, 0x05, 'T', 'e', 's', 't', '1', 1U } } ),
    Arinc665Exception );
}

//! Encode String Test
BOOST_AUTO_TEST_CASE( encodeString )
{
  BOOST_CHECK( ( StringUtils_encodeString( "Test" ) == RawFile{ { 0x00, 0x04, 'T', 'e', 's', 't' } } ) );
  BOOST_CHECK( ( StringUtils_encodeString( "Test1" ) ==  RawFile{ { 0x00, 0x05, 'T', 'e', 's', 't', '1', 0x00 } } ) );
  BOOST_CHECK( ( StringUtils_encodeString( "" ) ==  RawFile{ { 0x00, 0x00 } } ) );
}

//! Decode Strings Test
BOOST_AUTO_TEST_CASE( decodeStrings )
{
  ConstRawFileSpan remaining;
  std::list< std::string > out;

  std::tie( remaining, out ) = StringUtils_decodeStrings(
    ConstRawFileSpan{
      { 0x00, 0x03,
        0x00, 0x04, 'T', 'e', 's', 't',
        0x00, 0x05, 'T', 'e', 's', 't', '1', 0x00,
        0x00, 0x00} } );

  BOOST_CHECK( remaining.empty() );
  BOOST_CHECK( out.size() == 3 );
  BOOST_CHECK( *out.begin() == "Test" );
  BOOST_CHECK( *std::next( out.begin() ) == "Test1" );
  BOOST_CHECK( std::next( out.begin(), 2 )->empty() );

  std::tie( remaining, out ) = StringUtils_decodeStrings( ConstRawFileSpan{ { 0x00, 0x00 } } );
  BOOST_CHECK( remaining.empty() );
  BOOST_CHECK( out.empty() );
}

//! Encode Strings Test
BOOST_AUTO_TEST_CASE( encodeStrings )
{
  BOOST_CHECK( ( StringUtils_encodeStrings( {} ) == RawFile{ { 0x00, 0x00 } } ) );

  BOOST_CHECK( ( StringUtils_encodeStrings( { "Test", "Test1", "" } )
    == RawFile{
      {
        0x00, 0x03,
        0x00, 0x04, 'T', 'e', 's', 't',
        0x00, 0x05, 'T', 'e', 's', 't', '1', 0x00,
        0x00, 0x00 } } ) );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

}

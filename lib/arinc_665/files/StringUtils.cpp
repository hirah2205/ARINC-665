// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of String Utility Functions.
 **/

#include "StringUtils.hpp"

#include <arinc_665/Arinc665Exception.hpp>

#include <helper/Endianess.hpp>
#include <helper/Exception.hpp>
#include <helper/SafeCast.hpp>

#include <boost/exception/all.hpp>

#include <cassert>

namespace Arinc665::Files {

std::tuple< ConstRawDataSpan, std::string > StringUtils_decodeString( ConstRawDataSpan rawData )
{
  auto remaining{ rawData };

  // string length
  uint16_t stringLength{};
  std::tie( remaining, stringLength ) = Helper::getInt< uint16_t >( remaining );

  // copy string
  std::string string{ reinterpret_cast< char const * >( remaining.data() ), stringLength };
  remaining = remaining.subspan( stringLength );

  // if string is odd skip filled 0-character
  if ( stringLength % 2 == 1 )
  {
    // check fill-character
    if ( std::byte{ 0 } != remaining.front() )
    {
      BOOST_THROW_EXCEPTION( Arinc665Exception()
        << Helper::AdditionalInfo{ "Fill character not '0'" } );
    }
    remaining = remaining.subspan( 1 );
  }

  return { remaining, string };
}

RawData StringUtils_encodeString( std::string_view string )
{
  RawData rawString;
  rawString.reserve( sizeof( uint16_t ) + string.size()  + ( string.size() % sizeof( uint16_t ) ) );

  // set string length
  rawString.resize( sizeof( uint16_t ) );
  Helper::setInt< uint16_t >( rawString, Helper::safeCast< uint16_t >( string.size() ) );

  // copy string
  auto stringSpan{ ConstRawDataSpan{ reinterpret_cast< std::byte const * >( string.data() ), string.size() } };
  rawString.insert( rawString.end(), stringSpan.begin(), stringSpan.end() );

  // fill string if it is odd
  if ( string.size() % 2 == 1 )
  {
    rawString.push_back( std::byte{ 0U } );
  }

  return rawString;
}

std::tuple< ConstRawDataSpan, std::list< std::string > > StringUtils_decodeStrings( ConstRawDataSpan rawData )
{
  // empty strings
  std::list< std::string > strings;

  auto remaining{ rawData };

  // number of strings
  uint16_t numberOfEntries{};
  std::tie( remaining, numberOfEntries ) = Helper::getInt< uint16_t >( remaining );

  for ( uint16_t index = 0U; index < numberOfEntries; ++index )
  {
    // string
    std::string string;
    std::tie( remaining, string ) = StringUtils_decodeString( remaining );
    strings.emplace_back( std::move( string ) );
  }

  return { remaining, strings };
}

RawData StringUtils_encodeStrings( const std::list< std::string > &strings )
{
  RawData rawStrings( sizeof( uint16_t ) );

  // set number of strings
  Helper::setInt< uint16_t >( rawStrings, Helper::safeCast< uint16_t >( strings.size() ) );

  for ( const auto &string : strings )
  {
    auto rawString{ StringUtils_encodeString( string ) };
    assert( rawString.size() % 2 == 0 );

    // append string
    rawStrings.insert( rawStrings.end(), rawString.begin(), rawString.end() );
  }

  return rawStrings;
}

}

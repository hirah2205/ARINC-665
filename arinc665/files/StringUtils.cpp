/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of String Utility Functions.
 **/

#include "StringUtils.hpp"

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::Files {

ConstRawFileSpan::iterator StringUtils_decodeString(
  ConstRawFileSpan::iterator it,
  std::string &str )
{
  // determine string length
  uint16_t strLength{};
  it = Helper::getInt< uint16_t>( it, strLength );

  // copy string
  str.assign( it, it + strLength );
  it += strLength;

  // if string is odd skipp filled 0-character
  if ( strLength % 2 == 1 )
  {
    ++it;
  }

  return it;
}

RawFile StringUtils_encodeString( std::string_view str )
{
  RawFile rawString( sizeof( uint16_t ) );

  // set string length
  Helper::setInt< uint16_t>(
    rawString.begin(),
    Helper::safeCast< uint16_t>( str.size() ) );

  // copy string
  rawString.insert( rawString.end(), str.begin(), str.end() );

  // fill string if it is odd
  if ( str.size() % 2 == 1 )
  {
    rawString.push_back( 0U );
  }

  return rawString;
}

ConstRawFileSpan::iterator StringUtils_decodeStrings(
  ConstRawFileSpan::iterator it,
  std::list< std::string > &strings )
{
  // number of strings
  uint16_t numberOfEntries{};
  it = Helper::getInt< uint16_t>( it, numberOfEntries);

  for ( uint16_t index = 0U; index < numberOfEntries; ++index )
  {
    // string
    std::string str{};
    it = StringUtils_decodeString( it, str);
    strings.emplace_back( std::move( str) );
  }

  return it;
}

ConstRawFileSpan::iterator StringUtils_decodeStrings(
  ConstRawFileSpan::iterator it,
  std::set< std::string, std::less<> > &strings )
{
  // number of strings
  uint16_t numberOfEntries{};
  it = Helper::getInt< uint16_t>( it, numberOfEntries );

  for ( uint16_t index = 0U; index < numberOfEntries; ++index )
  {
    // string
    std::string str{};
    it = StringUtils_decodeString( it, str);
    strings.emplace( std::move( str) );
  }

  return it;
}

RawFile StringUtils_encodeStrings( const std::list< std::string > &strings )
{
  RawFile rawStrings( sizeof( uint16_t ) );

  // set number of strings
  Helper::setInt< uint16_t >(
    rawStrings.begin(),
    static_cast< uint16_t>( strings.size()));

  for ( const auto &str : strings )
  {
    auto rawStr{ StringUtils_encodeString( str ) };
    assert( rawStr.size() % 2 == 0 );

    // append string
    rawStrings.insert( rawStrings.end(), rawStr.begin(), rawStr.end() );
  }

  return rawStrings;
}

RawFile StringUtils_encodeStrings(
  const std::set< std::string, std::less<> > &strings )
{
  RawFile rawStrings( sizeof( uint16_t ) );

  // set number of strings
  Helper::setInt< uint16_t>(
    rawStrings.begin(),
    static_cast< uint16_t>( strings.size() ) );

  for ( const auto &str : strings )
  {
    auto rawStr{ StringUtils_encodeString( str ) };
    assert( rawStr.size() % 2 == 0 );

    // append string
    rawStrings.insert( rawStrings.end(), rawStr.begin(), rawStr.end() );
  }

  return rawStrings;
}

}

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
 * @brief Definition of Check Value Utility Functions.
 **/

#include "CheckValueUtils.hpp"

#include <arinc665/Arinc665Exception.hpp>

#include <arinc645/CheckValue.hpp>
#include <arinc645/CheckValueTypeDescription.hpp>

#include <helper/Endianness.hpp>
#include <helper/Exception.hpp>
#include <helper/SafeCast.hpp>

#include <boost/exception/all.hpp>

namespace Arinc665::Files {

size_t CheckValueUtils_size( const Arinc645::CheckValueType type )
{
  return
    ( Arinc645::CheckValueType::NotUsed == type ) ?
    sizeof( uint16_t ) :
    2U * sizeof( uint16_t ) + Arinc645::CheckValue::Sizes.at( type );
}

RawFile CheckValueUtils_encode(
  const Arinc645::CheckValue &checkValue )
{
  if ( Arinc645::CheckValueType::NotUsed == checkValue.type() )
  {
    return { 0, 0 };
  }

  // Length + Type
  RawFile rawCheckValue( 2 *sizeof( uint16_t ) );

  // Check Value Type Field
  Helper::setInt< uint16_t>(
    rawCheckValue.begin() + sizeof( uint16_t ),
    static_cast< uint16_t>( checkValue.type() ) );

  // Check Value Data
  const auto &checkValueData{ checkValue.value() };

  rawCheckValue.insert(
    rawCheckValue.end(),
    checkValueData.begin(),
    checkValueData.end() );

  // Check Value Length
  Helper::setInt< uint16_t>(
    rawCheckValue.begin(),
    Helper::safeCast< uint16_t>( rawCheckValue.size() ) );

  return rawCheckValue;
}

Arinc645::CheckValue CheckValueUtils_decode( ConstRawFileSpan rawFile )
{
  if ( rawFile.size() < sizeof( uint16_t ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Invalid check value" } );
  }

  auto it{ rawFile.begin() };

  uint16_t checkValueLength{};
  it = Helper::getInt< uint16_t>( it, checkValueLength );

  if ( 0U == checkValueLength )
  {
    // empty check value
    return Arinc645::CheckValue::NoCheckValue;
  }

  if ( checkValueLength <= ( 2U * sizeof( uint16_t ) ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Invalid length field of check value" } );
  }

  uint16_t rawCheckValueType{};
  it = Helper::getInt< uint16_t >( it, rawCheckValueType );

  const auto checkValueType{
    Arinc645::CheckValueTypeDescription::instance().enumeration(
      rawCheckValueType ) };

  if ( !checkValueType )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Invalid check value type" } );
  }

  if ( Arinc645::CheckValue::Sizes.find( *checkValueType )->second
    != checkValueLength - ( 2U * sizeof( uint16_t ) ) )
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Invalid check value length" } );
  }

  return { *checkValueType,
    std::vector< uint8_t >{
      it,
      it + checkValueLength - ( 2U * sizeof( uint16_t ) ) } };
}

}

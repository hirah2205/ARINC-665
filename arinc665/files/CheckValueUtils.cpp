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

#include <helper/Endianess.hpp>
#include <helper/SafeCast.hpp>

namespace Arinc665::Files {

RawFile CheckValueUtils_encode(
  const std::optional< Arinc645::CheckValue> &checkValue )
{
  RawFile rawCheckValue( sizeof( uint16_t));

  if ( !checkValue)
  {
    Helper::setInt< uint16_t>( rawCheckValue.begin(), 0U);
    return rawCheckValue;
  }

  // Add Check Value Type
  rawCheckValue.resize( rawCheckValue.size() + sizeof( uint16_t));

  Helper::setInt< uint16_t>(
    rawCheckValue.begin() + sizeof( uint16_t),
    static_cast< uint16_t>( std::get< 0>( *checkValue)));

  // Check Value Data
  const auto &checkValueData{ std::get< 1>( *checkValue)};

  rawCheckValue.insert(
    rawCheckValue.end(),
    checkValueData.begin(),
    checkValueData.end());

  // Check Value Length
  Helper::setInt< uint16_t>(
    rawCheckValue.begin(),
    Helper::safeCast< uint16_t>( rawCheckValue.size()));

  return rawCheckValue;
}

std::optional< Arinc645::CheckValue > CheckValueUtils_decode(
  const ConstRawFileSpan &rawFile,
  const ptrdiff_t offset )
{
  auto it{ rawFile.begin() + offset };

  uint16_t checkValueLength{};
  it = Helper::getInt< uint16_t>( it, checkValueLength);

  if (0U == checkValueLength)
  {
    // empty check value
    return {};
  }

  if ( checkValueLength <= ( 2U * sizeof( uint16_t)))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo{ "Invalid length field of check value" } );
  }

  uint16_t checkValueType{};
  it = Helper::getInt< uint16_t>( it, checkValueType);

  return { std::make_tuple(
    static_cast< Arinc645::CheckValueType >( checkValueType ),
    std::vector< uint8_t >{
      it,
      it + checkValueLength - ( 2U * sizeof( uint16_t ) ) } ) };
}

}

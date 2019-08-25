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

namespace Arinc665::File {

RawFile CheckValueUtils_encode( const std::optional< CheckValue> &checkValue)
{
  RawFile rawCheckValue( sizeof( uint16_t));

  if (!checkValue)
  {
    setInt< uint16_t>( rawCheckValue.begin(), 0U);
    return rawCheckValue;
  }

  // Add Check Value Type
  rawCheckValue.resize( rawCheckValue.size() + sizeof( uint16_t));

  setInt< uint16_t>(
    rawCheckValue.begin() + sizeof( uint16_t),
    static_cast< uint16_t>( std::get< 0>( *checkValue)));

  // Check Value Data
  const auto &checkValueData{ std::get< 1>( *checkValue)};

  rawCheckValue.insert(
    rawCheckValue.end(),
    checkValueData.begin(),
    checkValueData.end());

  // Check Value Length
  setInt< uint16_t>(
    rawCheckValue.begin(),
    safeCast< uint16_t>( rawCheckValue.size()));

  return rawCheckValue;
}

std::optional< CheckValue> CheckValueUtils_decode(
  const RawFile &rawFile,
  std::size_t offset)
{
  auto it{ rawFile.begin() + offset};

  uint16_t checkValueLength;
  it = getInt< uint16_t>( it, checkValueLength);

  if (0U == checkValueLength)
  {
    // empty check value
    return {};
  }

  if ( checkValueLength <= ( 2* sizeof( uint16_t)))
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "Invalid length field of check value"));
  }

  uint16_t checkValueType;
  it = getInt< uint16_t>( it, checkValueType);

  return { std::make_tuple(
    static_cast< CheckValueType >( checkValueType),
    std::vector< uint8_t>{
      it,
      it + checkValueLength - ( 2 * sizeof( uint16_t))})};
}

}

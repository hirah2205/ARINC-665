/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::PartNumber.
 **/

#include "PartNumber.hpp"

#include <boost/format.hpp>

namespace Arinc665 {

PartNumber::PartNumber(
  std::string_view manufacturerCode,
  std::string_view productIdentifier) :
  manufacturerCodeValue{ manufacturerCode},
  productIdentifierValue{ productIdentifier}
{
  checkManufacturerCode( manufacturerCode);
  checkProductIdentifier( productIdentifier);
}

PartNumber::PartNumber( std::string_view partNumber)
try :
  manufacturerCodeValue{ partNumber.substr( 0, ManufacturerCodeLength)},
  productIdentifierValue{
    partNumber.substr(
      ManufacturerCodeLength + CheckCodeLength,
      ProductIdentifierLength)}
{
  // check size of part number
  if ( partNumber.size() != Length)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "Invalid size of part number string"));
  }

  // decode check code
  auto checkCodeDecoded{
    partNumber.substr( ManufacturerCodeLength, CheckCodeLength)};

  // compare check code with calculated one
  if ( checkCodeDecoded != checkCode())
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "calculated and given check code differs"));
  }
}
catch( std::out_of_range &e)
{
  BOOST_THROW_EXCEPTION( Arinc665Exception()
    << Helper::AdditionalInfo( e.what()));
}

std::string_view PartNumber::manufacturerCode() const
{
  return manufacturerCodeValue;
}

void PartNumber::manufacturerCode( std::string_view manufacturerCode)
{
  checkManufacturerCode( manufacturerCode);

  manufacturerCodeValue = manufacturerCode;
}

std::string_view PartNumber::productIdentifier() const
{
  return productIdentifierValue;
}

void PartNumber::productIdentifier( std::string_view productIdentifier)
{
  checkProductIdentifier( productIdentifier);

  productIdentifierValue = productIdentifier;
}

std::string PartNumber::checkCode() const
{
  uint8_t checkCode{ 0U};

  for ( const auto &character : manufacturerCodeValue)
  {
    checkCode ^= static_cast< uint8_t >( character);
  }

  for ( const auto &character : productIdentifierValue)
  {
    checkCode ^= static_cast< uint8_t >( character);
  }

  return (boost::format( "%02X") % (unsigned int) checkCode).str();
}

std::string PartNumber::partNumber() const
{
  return
    manufacturerCodeValue
    + checkCode()
    + productIdentifierValue;
}

void PartNumber::checkManufacturerCode( std::string_view manufacturerCode) const
{
  // check string length
  if (manufacturerCode.size()!= ManufacturerCodeLength)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "length of manufacturer code string invalid"));
  }
}

void PartNumber::checkProductIdentifier( std::string_view productIdentifier) const
{
  // check length of string
  if ( productIdentifier.size() != ProductIdentifierLength)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << Helper::AdditionalInfo( "length of product identifier string invalid"));
  }
}

void PartNumber::checkCheckCode( std::string_view checkCode) const
{
  // check length of string
  if ( checkCode.size() != CheckCodeLength)
  {
    BOOST_THROW_EXCEPTION(Arinc665Exception()
      << Helper::AdditionalInfo{ "length of check code string invalid" } );
  }

  // decode string to integer
  unsigned long parsedCheckCode{
    std::stoul( std::string{ checkCode}, 0, 16)}; //! @todo check implementation of explicit cast

  assert( parsedCheckCode <= 255U);
}

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::PartNumber.
 **/

#include "PartNumber.hpp"

#include <boost/format.hpp>

namespace Arinc665 {

PartNumber::PartNumber(
  const ManufacturerCode &manufacturerCode,
  const ProductIdentifier& productIdentifier) :
  manufacturerCodeValue( manufacturerCode),
  productIdentifierValue( productIdentifier)
{
}

PartNumber::PartNumber( std::string_view partNumber) :
  manufacturerCodeValue( partNumber.substr( 0, ManufacturerCode::Length)),
  productIdentifierValue(
    partNumber.substr(
      ManufacturerCode::Length + CheckCode::Length,
      ProductIdentifier::Length))
{
  // check size of part number
  if ( partNumber.size() != Length)
  {
    /** @throw Arinc665Exception If string size is invalid **/
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "Invalid size of part number string"));
  }

  // decode check code
  CheckCode checkCodeDecoded(
    partNumber.substr( ManufacturerCode::Length, CheckCode::Length));

  // compare check code with calculated one
  if ( checkCodeDecoded != checkCode())
  {
    //! @throw Arinc665Exception, when calculated check code differs from current
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "calculated and given check code differs"));
  }
}

ManufacturerCode PartNumber::manufacturerCode() const
{
  return manufacturerCodeValue;
}

ManufacturerCode& PartNumber::manufacturerCode()
{
  return manufacturerCodeValue;
}

void PartNumber::manufacturerCode( const ManufacturerCode& manufacturerCode)
{
  manufacturerCodeValue = manufacturerCode;
}

ProductIdentifier PartNumber::productIdentifier() const
{
  return productIdentifierValue;
}

ProductIdentifier& PartNumber::productIdentifier()
{
  return productIdentifierValue;
}

void PartNumber::productIdentifier(
  const ProductIdentifier& productIdentifier)
{
  productIdentifierValue = productIdentifier;
}

CheckCode PartNumber::checkCode() const
{
  return CheckCode( manufacturerCodeValue, productIdentifierValue);
}

std::string PartNumber::partNumber() const
{
  return
    std::string{ manufacturerCodeValue.get()}
    + std::string{ checkCode().getStr()}
    + std::string{ productIdentifierValue.get()};
}

}

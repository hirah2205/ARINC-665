/*
 * $Date$
 * $Revision$
 */
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
  manufacturerCode( manufacturerCode),
  productIdentifier( productIdentifier)
{
}

PartNumber::PartNumber( const string& partNumber) :
  manufacturerCode( partNumber.substr( 0, ManufacturerCode::Length)),
  productIdentifier(
    partNumber.substr(
      ManufacturerCode::Length + CheckCode::Length,
      ProductIdentifier::Length))
{
  // check size of part number
  if ( partNumber.size() != Length)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "Invalid size of part number string"));
  }

  // decode check code
  CheckCode checkCodeDecoded(
    partNumber.substr( ManufacturerCode::Length, CheckCode::Length));

  // compare check code with calculated one
  if ( checkCodeDecoded != getCheckCode())
  {
    //! @throw Arinc665Exception, when calculated check code differs from current
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "calculated and given check code differs"));
  }
}

ManufacturerCode PartNumber::getManufacturerCode() const
{
  return manufacturerCode;
}

ManufacturerCode& PartNumber::getManufacturerCode()
{
  return manufacturerCode;
}

void PartNumber::setManufacturerCode( const ManufacturerCode& manufacturerCode)
{
  this->manufacturerCode = manufacturerCode;
}

ProductIdentifier PartNumber::getProductIdentifier() const
{
  return productIdentifier;
}

ProductIdentifier& PartNumber::getProductIdentifier()
{
  return productIdentifier;
}

void PartNumber::setProductIdentifier(
  const ProductIdentifier& productIdentifier)
{
  this->productIdentifier = productIdentifier;
}

CheckCode PartNumber::getCheckCode() const
{
  return CheckCode( manufacturerCode, productIdentifier);
}

PartNumber::string PartNumber::getPartNumber() const
{
  return manufacturerCode.get() + getCheckCode().getStr()
    + productIdentifier.get();
}

}


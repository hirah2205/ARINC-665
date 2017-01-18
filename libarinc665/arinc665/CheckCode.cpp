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
 * @brief Definition of class Arinc665::CheckCode.
 **/

#include "CheckCode.hpp"

#include <arinc665/Arinc665Exception.hpp>
#include <arinc665/ManufacturerCode.hpp>
#include <arinc665/ProductIdentifier.hpp>

#include <boost/format.hpp>

namespace Arinc665 {

CheckCode::CheckCode( const uint8_t checkCode) :
  checkCode( checkCode)
{
}

CheckCode::CheckCode( const string &checkCode)
{
  set( checkCode);
}

CheckCode::CheckCode(
  const ManufacturerCode &manufacturerCode,
  const ProductIdentifier &productIdentifier)
{
  calculate( manufacturerCode, productIdentifier);
}

uint8_t CheckCode::get() const noexcept
{
  return checkCode;
}

std::string CheckCode::getStr() const
{
  return (boost::format( "%02X") % (unsigned int) checkCode).str();
}

void CheckCode::set( const uint8_t checkCode)
{
  this->checkCode = checkCode;
}

void CheckCode::set( const string &checkCode)
{
  // check length of string
  if ( checkCode.size() != Length)
  {
    //! @throw Arinc665Exception if length of checkCode is invalid.
    BOOST_THROW_EXCEPTION(
      Arinc665Exception()
        << AdditionalInfo( "length of check code string invalid"));
  }

  // decode string to integer
  //! @throw std::invalid_argument When invalid string is supplied
  unsigned long parsedCheckCode = std::stoul( checkCode, 0, 16);

  this->checkCode = parsedCheckCode;
}

void CheckCode::calculate(
  const ManufacturerCode &manufacturerCode,
  const ProductIdentifier &productIdentifier)
{
  checkCode = 0;

  string calcString = manufacturerCode.get() + productIdentifier.get();

  for ( const auto &character : calcString)
  {
    checkCode ^= character;
  }
}

bool CheckCode::operator ==( const CheckCode &rhs) const
{
  return (rhs.checkCode == checkCode);
}

bool CheckCode::operator !=( const CheckCode &rhs) const
{
  return (rhs.checkCode != checkCode);
}

}

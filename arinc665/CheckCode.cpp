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

CheckCode::CheckCode( std::string_view checkCode)
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

void CheckCode::set( std::string_view checkCode)
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
  unsigned long parsedCheckCode{ std::stoul( std::string{ checkCode}, 0, 16)}; //! @todo check implementation of explicit cast

  assert( parsedCheckCode <= 255U);

  this->checkCode = static_cast< uint8_t>( parsedCheckCode);
}

void CheckCode::calculate(
  const ManufacturerCode &manufacturerCode,
  const ProductIdentifier &productIdentifier)
{
  checkCode = 0;

  for ( const auto &character : manufacturerCode.get())
  {
    checkCode ^= character;
  }

  for ( const auto &character : productIdentifier.get())
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

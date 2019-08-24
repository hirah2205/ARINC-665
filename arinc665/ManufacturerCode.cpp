/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::ManufacturerCode;
 **/

#include "ManufacturerCode.hpp"

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665 {

ManufacturerCode::ManufacturerCode( std::string_view manufacturerCode)
{
  // call set routine to perform automatic length check
  set( manufacturerCode);
}

ManufacturerCode::ManufacturerCode( std::string &&manufacturerCode)
{
  // call set routine to perform automatic length check
  set( std::move( manufacturerCode));
}

std::string_view ManufacturerCode::get() const
{
  return manufacturerCode;
}

void ManufacturerCode::set( std::string_view manufacturerCode)
{
  // check string length
  if (manufacturerCode.size()!= Length)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "length of manufacturer code string invalid"));
  }

  this->manufacturerCode = manufacturerCode;
}

void ManufacturerCode::set( std::string &&manufacturerCode)
{
  // check string length
  if (manufacturerCode.size()!= Length)
  {
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "length of manufacturer code string invalid"));
  }

  this->manufacturerCode = std::move( manufacturerCode);
}

}

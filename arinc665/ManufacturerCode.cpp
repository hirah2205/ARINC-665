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
 * @brief Definition of class Arinc665::ManufacturerCode;
 **/

#include "ManufacturerCode.hpp"
#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665 {

ManufacturerCode::ManufacturerCode( const string& manufacturerCode)
{
  // call set routine to perform automatic length check
  //! @throw Arinc665Exception @sa ManufacturerCode::set
  set( manufacturerCode);
}

std::string ManufacturerCode::get() const
{
  return manufacturerCode;
}

void ManufacturerCode::set( const string& manufacturerCode)
{
  // check string length
  if (manufacturerCode.size()!= Length)
  {
    //! @throw Arinc665Exception When the given string is not an valid
    //!   manufacturer code.
    BOOST_THROW_EXCEPTION( Arinc665Exception() <<
      AdditionalInfo( "length of manufacturer code string invalid"));
  }

  this->manufacturerCode = manufacturerCode;
}

}

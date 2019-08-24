/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::ProductIdentifier.
 **/

#include "ProductIdentifier.hpp"

#include <arinc665/Arinc665Exception.hpp>

namespace Arinc665 {

ProductIdentifier::ProductIdentifier( std::string_view productIdentifier)
{
  // call set routine to perform automatic length check
  set( productIdentifier);
}

ProductIdentifier::ProductIdentifier( std::string &&productIdentifier)
{
  // call set routine to perform automatic length check
  set( std::move( productIdentifier));
}

std::string_view ProductIdentifier::get() const
{
  return productIdentifier;
}

void ProductIdentifier::set( std::string_view productIdentifier)
{
  check( productIdentifier);

  this->productIdentifier = productIdentifier;
}

void ProductIdentifier::set( std::string &&productIdentifier)
{
  check( productIdentifier);

  this->productIdentifier = std::move( productIdentifier);
}

void ProductIdentifier::check( std::string_view productIdentifier) const
{
  // check length of string
  if ( productIdentifier.size() != Length)
  {
    //! @throw Arinc665Exception if length is invalid.
    BOOST_THROW_EXCEPTION( Arinc665Exception()
      << AdditionalInfo( "length of product identifier string invalid"));
  }
}

}

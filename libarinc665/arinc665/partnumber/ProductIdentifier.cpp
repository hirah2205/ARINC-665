/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::ProductIdentifier.
 **/

#include "ProductIdentifier.hpp"
#include <arinc665/Arinc665Exception.hpp>

using namespace Arinc665;

ProductIdentifier::ProductIdentifier( const string &productIdentifier)
{
	//! call set routine to perform automatic length check
	//! @throw Arinc665Exception (@sa ProductIdentifier::set())
	set( productIdentifier);
}

string ProductIdentifier::get( void) const
{
	return productIdentifier;
}

void ProductIdentifier::set( const string& productIdentifier)
{
	// check length of string
	if (productIdentifier.size() != LENGTH)
	{
		//! @throw Arinc665Exception if length is invalid.
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "length of product identifier string invalid"));
	}

	this->productIdentifier = productIdentifier;
}

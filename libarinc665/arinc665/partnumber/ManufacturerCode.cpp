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
 * @brief Definition of class Arinc665::ManufacturerCode;
 **/

#include "ManufacturerCode.hpp"
#include <arinc665/Arinc665Exception.hpp>

using namespace Arinc665;

ManufacturerCode::ManufacturerCode( const string& manufacturerCode)
{
	// call set routine to perform automatic length check
	set( manufacturerCode);
}

std::string ManufacturerCode::get( void) const
{
	return manufacturerCode;
}

void ManufacturerCode::set( const string& manufacturerCode)
{
	// check string length
	if (manufacturerCode.size()!= LENGTH)
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "length of manufacturer code string invalid"));

	this->manufacturerCode = manufacturerCode;
}

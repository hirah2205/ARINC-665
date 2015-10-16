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
 * @brief Definition of class PartNumber
 **/

#include "PartNumber.hpp"
#include <boost/format.hpp>

using namespace Arinc665;

PartNumber::PartNumber(
	const ManufacturerCode &manufacturerCode,
	const ProductIdentifier& productIdentifier):
	manufacturerCode( manufacturerCode),
	productIdentifier( productIdentifier)
{
}

PartNumber::PartNumber( const string& partNumber):
	manufacturerCode( partNumber.substr(0, ManufacturerCode::LENGTH)),
	productIdentifier( partNumber.substr(
		ManufacturerCode::LENGTH + CheckCode::LENGTH,
		ProductIdentifier::LENGTH))
{
	//! check size of part number
	if (partNumber.size() != LENGTH)
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "Invalid size of part number string"));

	//! decode check code
	CheckCode checkCodeDecoded( partNumber.substr(
		ManufacturerCode::LENGTH,
		CheckCode::LENGTH));

	//! compare check code with calculated one
	if (checkCodeDecoded != getCheckCode())
		BOOST_THROW_EXCEPTION( Arinc665Exception() <<
			AdditionalInfo( "calculated and given check code differs"));
}

ManufacturerCode PartNumber::getManufacturerCode( void) const
{
	return manufacturerCode;
}

ManufacturerCode& PartNumber::getManufacturerCode( void)
{
	return manufacturerCode;
}

void PartNumber::setManufacturerCode( const ManufacturerCode& manufacturerCode)
{
	this->manufacturerCode = manufacturerCode;
}

ProductIdentifier PartNumber::getProductIdentifier( void) const
{
	return productIdentifier;
}

ProductIdentifier& PartNumber::getProductIdentifier( void)
{
	return productIdentifier;
}

void PartNumber::setProductIdentifier(
	const ProductIdentifier& productIdentifier)
{
	this->productIdentifier = productIdentifier;
}

CheckCode PartNumber::getCheckCode( void) const
{
	return CheckCode( manufacturerCode, productIdentifier);
}

std::string PartNumber::getPartNumber( void) const
{
	return manufacturerCode.get() +
		getCheckCode().getStr() + productIdentifier.get();
}

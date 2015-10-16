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
 * @brief Definition of class PartNumberdEntity
 **/

#include "PartNumberdEntity.hpp"

using namespace Arinc665::Media;

std::string PartNumberdEntity::getPartNumber( void) const
{
	return partNumber;
}

void PartNumberdEntity::setPartNumber( const string &partNumber)
{
	this->partNumber = partNumber;
}

PartNumberdEntity::PartNumberdEntity( const string &partNumber):
	partNumber( partNumber)
{
}

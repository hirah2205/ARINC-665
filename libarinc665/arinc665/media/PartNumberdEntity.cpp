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
 * @brief Definition of class Arinc665::Media::PartNumberdEntity.
 **/

#include "PartNumberdEntity.hpp"

namespace Arinc665 {
namespace Media {

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

}
}

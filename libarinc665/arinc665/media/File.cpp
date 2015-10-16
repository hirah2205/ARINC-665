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
 * @brief @todo Add brief description.
 *
 * TODO Add detailed description.
 **/

#include "File.hpp"

using namespace Arinc665::Media;

File::File(
	const string &name,
	const uint16_t crc,
	const uint32_t fileLength,
	const string &partNumber):
	PartNumberdEntity( partNumber),
	name( name),
	crc( crc),
	fileLength( fileLength)
{
}

const string& File::getName( void) const
{
	return name;
}

uint16_t File::getCrc( void) const
{
	return crc;
}

void File::setCrc( const uint16_t crc)
{
	this->crc = crc;
}

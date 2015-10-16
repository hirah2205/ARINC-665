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

#include "Batch.hpp"

using namespace Arinc665::Media;

Batch::Batch( const string &partNumber, const string &comment):
	PartNumberdEntity( partNumber),
	comment( comment)
{
}

string Batch::getComment( void) const
{
	return comment;
}

void Batch::setComment( const string &comment)
{
	this->comment = comment;
}

const Batch::BatchInfoList& Batch::getBatchInfos( void)
{
	return batchInfos;
}

BatchInfo& Batch::addBatchInfo( const string &targetHardwareId)
{
	BatchInfo batchInfo( targetHardwareId);
	batchInfos.push_back( batchInfo);
	return batchInfos.back();
}

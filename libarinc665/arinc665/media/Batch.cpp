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
 * @brief Definition of class Arinc665::Media::Batch.
 **/

#include "Batch.hpp"

namespace Arinc665 {
namespace Media {

Batch::Batch(
  const string &name,
  const string &partNumber,
  const string &comment) :
  BaseFile( name, partNumber),
	comment( comment)
{
}

Batch::FileType Batch::getFileType( void) const
{
  return FileType::BatchFile;
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

}
}

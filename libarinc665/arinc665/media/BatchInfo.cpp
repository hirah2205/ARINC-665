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

#include "BatchInfo.hpp"

using namespace Arinc665::Media;

BatchInfo::BatchInfo( const std::string &targetHardwareId):
	targetHardwareId( targetHardwareId)
{
}

std::string BatchInfo::getTargetHardwareId( void) const
{
	return targetHardwareId;
}

void BatchInfo::setTargetHardwareId( const std::string &targetHardwareId)
{
	this->targetHardwareId = targetHardwareId;
}

BatchInfo::LoadList& BatchInfo::getLoadList( void)
{
	return loadList;
}

void BatchInfo::addLoad( const WeakLoadPtr load)
{
	loadList.push_back( load);
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Definition of class LoadInfo
 **/

#include "LoadInfo.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

namespace Arinc665 {
namespace File {

std::list< LoadInfo> LoadInfo::getLoadList( std::vector< uint8_t>::const_iterator &it)
{
	std::vector< uint8_t>::const_iterator workIt = it;

	std::list< LoadInfo> loadList;

	// number of loads
	uint16_t numberOfLoads;
	workIt = getInt< uint16_t>( workIt, numberOfLoads);

	for ( unsigned int loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex )
	{
		loadList.push_back( LoadInfo( workIt));
	}

	it = workIt;

	return loadList;
}

LoadInfo::LoadInfo( void)
{
}

LoadInfo::LoadInfo( std::vector< uint8_t>::const_iterator &it)
{
	std::vector< uint8_t>::const_iterator workIt = it;

	// next load pointer
	uint16_t loadPointer;
	workIt = getInt< uint16_t>( workIt, loadPointer);

	// part number
	workIt = getString( workIt, partNumber);

	// header filename
	workIt = getString( workIt, headerFilename);

	// member sequence number
	workIt = getInt< uint16_t>( workIt, memberSequenceNumber);

	workIt = getStringList( workIt, targetHardwareIds);

	// set it to begin of next load
	it += loadPointer * 2;
}

std::string LoadInfo::getPartNumber( void) const
{
	return partNumber;
}

void LoadInfo::setPartNumber( const std::string &partNumber)
{
	this->partNumber = partNumber;
}

std::string LoadInfo::getHeaderFilename( void) const
{
	return headerFilename;
}

void LoadInfo::setHeaderFilename( const std::string &headerFilename)
{
	this->headerFilename = headerFilename;
}

uint16_t LoadInfo::getMemberSequenceNumber( void) const
{
	return memberSequenceNumber;
}

void LoadInfo::setMemberSequenceNumber( const uint16_t memberSequenceNumber)
{
	this->memberSequenceNumber = memberSequenceNumber;
}

const std::list<std::string>& LoadInfo::getTargetHardwareIdList( void) const
{
	return targetHardwareIds;
}

std::list<std::string>& LoadInfo::getTargetHardwareIdList( void)
{
	return targetHardwareIds;
}

}
}

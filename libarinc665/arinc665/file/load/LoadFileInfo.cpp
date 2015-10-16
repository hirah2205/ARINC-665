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
 * @brief Definition of class LoadFileInfo
 **/

#include "LoadFileInfo.hpp"
#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

using namespace Arinc665::File;

LoadFileInfo::LoadFileInfoList LoadFileInfo::getFileList(
	std::vector< uint8_t>::const_iterator &it)
{
	std::vector< uint8_t>::const_iterator workIt = it;

	std::list< LoadFileInfo> files;

	// number of data files
	uint16_t numberOfFiles;
	workIt = getInt< uint16_t>( workIt, numberOfFiles);

	for ( unsigned int fileIndex = 0; fileIndex < numberOfFiles; ++fileIndex)
	{
		files.push_back( LoadFileInfo( workIt));
	}

	it = workIt;

	return files;
}

LoadFileInfo::LoadFileInfo( void):
	length( 0),
	crc( 0)
{
}

LoadFileInfo::LoadFileInfo( std::vector< uint8_t>::const_iterator &it)
{
	std::vector< uint8_t>::const_iterator workIt = it;

	// next file pointer
	uint16_t filePointer;
	workIt = getInt< uint16_t>( workIt, filePointer);

	// filename
	workIt = getString( workIt, name);

	// part number
	workIt = getString( workIt, partNumber);

	// file length
	 workIt = getInt< uint32_t>( workIt, length);

	// CRC
	workIt = getInt< uint16_t>( workIt, crc);

	// set it to begin of next file
	it += filePointer * 2;
}

std::string LoadFileInfo::getName( void) const
{
	return name;
}

void LoadFileInfo::setName( const std::string &name)
{
	this->name = name;
}

std::string LoadFileInfo::getPartNumber( void) const
{
	return partNumber;
}

void LoadFileInfo::setPartNumber( const std::string &partNumber)
{
	this->partNumber = partNumber;
}

uint32_t LoadFileInfo::getLength( void) const
{
	return length;
}

void LoadFileInfo::setLength( uint32_t length)
{
	this->length = length;
}

uint16_t LoadFileInfo::getCrc( void) const
{
	return crc;
}

void LoadFileInfo::setCrc( const uint16_t crc)
{
	this->crc = crc;
}

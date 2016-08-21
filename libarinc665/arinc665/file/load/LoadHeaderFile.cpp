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
 * @brief Declaration of class LoadHeaderFile
 **/

#include "LoadHeaderFile.hpp"

#include <arinc665/file/StringHelper.hpp>

#include <helper/Logger.hpp>

using namespace Arinc665::File;

LoadHeaderFile::LoadHeaderFile( void):
	loadCrc( 0)
{
}

LoadHeaderFile::LoadHeaderFile( const RawFile &file):
	Arinc665File( file, Arinc665FileFormatVersion::LOAD_FILE_VERSION_2, 6)
{
	// set processing start to position after spare
	RawFile::const_iterator it = file.begin() + BaseHeaderOffset;

	uint32_t loadPartNumberPtr;
	it = getInt< uint32_t>( it, loadPartNumberPtr);

	uint32_t targetHardwareIdListPtr;
	it = getInt< uint32_t>( it, targetHardwareIdListPtr);

	uint32_t dataFileListPtr;
	it = getInt< uint32_t>( it, dataFileListPtr);

	uint32_t supportFileListPtr;
	it = getInt< uint32_t>( it, supportFileListPtr);

	uint32_t userDefinedDataPtr;
	it = getInt< uint32_t>( it, userDefinedDataPtr);


	//! load part number
	it = file.begin() + loadPartNumberPtr * 2;
	it = getString( it, partNumber);

	//! target hardware id list
	it = file.begin() + targetHardwareIdListPtr * 2;
	it = getStringList( it, targetHardwareIdList);

	// data file list
	it = file.begin() + dataFileListPtr * 2;
	dataFileList = LoadFileInfo::getFileList( it);

	// support file list
	if (0 != supportFileListPtr)
	{
		it = file.begin() + supportFileListPtr * 2;
		supportFileList = LoadFileInfo::getFileList( it);
	}

	// user defined data
	if (0 != userDefinedDataPtr)
	{
		it = file.begin() + userDefinedDataPtr * 2;
		userDefinedData.assign( it, file.end() - 6);
	}

	// file crc decoded and checked within base class

	// load crc
	getInt< uint32_t>( it, loadCrc);
}

Arinc665::Arinc665Version LoadHeaderFile::getArincVersion( void) const
{
	return Arinc665Version::ARINC_665_2;
}

LoadHeaderFile::string LoadHeaderFile::getPartNumber( void) const
{
	return partNumber;
}

void LoadHeaderFile::setPartNumber( const string &partNumber)
{
	this->partNumber = partNumber;
}

const std::list< std::string>& LoadHeaderFile::getTargetHardwareIdList( void) const
{
	return targetHardwareIdList;
}

std::list< std::string>& LoadHeaderFile::getTargetHardwareIdList( void)
{
	return targetHardwareIdList;
}

const LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getDataFileList( void) const
{
	return dataFileList;
}

LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getDataFileList( void)
{
	return dataFileList;
}

const LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getSupportFileList( void) const
{
	return supportFileList;
}

LoadHeaderFile::LoadFileInfoList& LoadHeaderFile::getSupportFileList( void)
{
	return supportFileList;
}

const LoadHeaderFile::UserDefinedData& LoadHeaderFile::getUserDefinedData( void) const
{
	return userDefinedData;
}

void LoadHeaderFile::setUserDefinedData(
	const UserDefinedData &userDefinedData)
{
	this->userDefinedData = userDefinedData;
}

uint32_t LoadHeaderFile::getLoadCrc( void) const
{
	return loadCrc;
}

void LoadHeaderFile::setLoadCrc( const uint32_t loadCrc)
{
	this->loadCrc = loadCrc;
}

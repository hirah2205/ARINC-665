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
 * @brief Definition of class Arinc665::Media::Medium.
 **/

#include "Medium.hpp"

#include <arinc665/media/Directory.hpp>

using namespace Arinc665::Media;

Medium::Medium( const unsigned int index):
	index( index),
	rootDirectory( std::make_shared< Directory>( ""))
{
}

unsigned int Medium::getIndex( void) const
{
	return index;
}

ConstDirectoryPtr Medium::getRootDirectory( void) const
{
	return rootDirectory;
}

DirectoryPtr Medium::getRootDirectory( void)
{
	return rootDirectory;
}

size_t Medium::getNumberOfFiles( void) const
{
	return rootDirectory->getNumberOfFiles( true);
}

ConstFileMap Medium::getFiles( void) const
{
	return ConstDirectoryPtr( rootDirectory)->getFiles( true);
}

FileMap Medium::getFiles( void)
{
	return rootDirectory->getFiles( true);
}

ConstFilePtr Medium::getFile( const string &filename) const
{
	return rootDirectory->getFile( filename, true);
}

FilePtr Medium::getFile( const string &filename)
{
	return rootDirectory->getFile( filename, true);
}

FilePtr Medium::addFile(
	const string &filename,
	const string &path,
	const uint16_t crc,
	const uint32_t fileLength,
	const string &partNumber)
{
	Directory &directory = rootDirectory->getDirectory( path);

	return directory.addFile( filename, crc, fileLength, partNumber);
}

size_t Medium::getNumberOfLoads( void) const
{
	return rootDirectory->getNumberOfLoads( true);
}

ConstLoadMap Medium::getLoads( void) const
{
	return ConstDirectoryPtr( rootDirectory)->getLoads( true);
}

LoadPtr Medium::addLoad(
	const string &filename,
	const string &path,
	const string &partNumber)
{
	Directory &directory = rootDirectory->getDirectory( path);

	return directory.addLoad( filename, partNumber);
}

size_t Medium::getNumberOfBatches( void) const
{
	return rootDirectory->getNumberOfBatches( true);
}

ConstBatchMap Medium::getBatches( void) const
{
	return ConstDirectoryPtr( rootDirectory)->getBatches( true);
}

BatchPtr Medium::addBatch(
	const string &filename,
	const string &path,
	const string &partNumber)
{
	Directory &directory = rootDirectory->getDirectory( path);

	return directory.addBatch( filename, partNumber);
}

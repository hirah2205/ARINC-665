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
 * @brief Definition of class MediaSet.
 **/

#include "MediaSet.hpp"

#include <arinc665/media/Medium.hpp>
#include <arinc665/Arinc665Exception.hpp>

#include <boost/foreach.hpp>

using namespace Arinc665::Media;

MediaSet::MediaSet( const string &partNumber, const unsigned int numberOfMedia):
	PartNumberdEntity( partNumber)
{
	for (unsigned int i = 0; i < numberOfMedia; ++i)
	{
		addMedium();
	}
}

unsigned int MediaSet::getNumberOfMedia( void) const
{
	return media.size();
}


const MediaMap& MediaSet::getMedia( void) const
{
	return media;
}

ConstMediumPtr MediaSet::getMedium( const unsigned int index) const
{
	return media.at( index);
}

MediumPtr MediaSet::getMedium( const unsigned int index)
{
	return media.at( index);
}

unsigned int MediaSet::addMedium( void)
{
	const unsigned int newIndex = media.size() + 1;

	media.insert( std::make_pair( newIndex, std::make_shared< Medium>( newIndex)));

	return newIndex;
}

ConstFileMap MediaSet::getFiles( void) const
{
	ConstFileMap files;

	//! Iterate over all medias and add their files to a complete list.
	for (const auto &medium: media)
	{
		ConstFileMap mediaFiles = ConstMediumPtr( medium.second)->getFiles();
		files.insert( mediaFiles.begin(), mediaFiles.end());
	}

	return files;
}

FileMap MediaSet::getFiles( void)
{
	FileMap files;

	//! Iterate over all medias and add their files to a complete list.
	for (auto &medium: media)
	{
		FileMap mediaFiles = medium.second->getFiles();
		files.insert( mediaFiles.begin(), mediaFiles.end());
	}

	return files;
}

ConstFilePtr MediaSet::getFile( const string &filename) const
{
	for ( auto const &medium : media)
	{
		ConstFilePtr file( medium.second->getFile( filename));

		if (file)
		{
			return file;
		}
	}

	return ConstFilePtr();
}

FilePtr MediaSet::getFile( const string &filename)
{
	for ( auto &medium : media)
	{
		FilePtr file( medium.second->getFile( filename));

		if (file)
		{
			return file;
		}
	}

	return FilePtr();
}

FilePtr MediaSet::addFile(
	MediumPtr medium,
	const string &filename,
	const string &path,
	const uint16_t crc,
	const uint32_t fileLength,
	const string &partNumber)
{
	return medium->addFile( filename, path, crc, fileLength, partNumber);
}

FilePtr MediaSet::addFile(
	const unsigned int mediumIndex,
	const string &filename,
	const string &path,
	const uint16_t crc,
	const uint32_t fileLength,
	const string &partNumber)
{
	return getMedium( mediumIndex)->addFile(
		filename, path, crc, fileLength, partNumber);
}

ConstLoadMap MediaSet::getLoads( void) const
{
	ConstLoadMap loads;

	for (auto &medium : media)
	{
		ConstLoadMap mediaLoads = medium.second->getLoads();
		loads.insert( mediaLoads.begin(), mediaLoads.end());
	}

	return loads;
}

LoadPtr MediaSet::addLoad(
	MediumPtr medium,
	const string &filename,
	const string &path,
	const string &partNumber)
{
	return medium->addLoad( partNumber, filename, path);
}

LoadPtr MediaSet::addLoad(
	const unsigned int mediumIndex,
	const string &filename,
	const string &path,
	const string &partNumber)
{
	return getMedium( mediumIndex)->addLoad( filename, path, partNumber);
}

ConstBatchMap MediaSet::getBatches( void) const
{
	ConstBatchMap batches;

	for (auto &medium : media)
	{
		ConstBatchMap mediaBatches = medium.second->getBatches();
		batches.insert( mediaBatches.begin(), mediaBatches.end());
	}

	return batches;
}

BatchPtr MediaSet::addBatch(
	MediumPtr medium,
	const string &filename,
	const string &path,
	const string &partNumber)
{
	return medium->addBatch( partNumber, filename, path);
}

BatchPtr MediaSet::addBatch(
	const unsigned int mediumIndex,
	const string &filename,
	const string &path,
	const string &partNumber)
{
	return getMedium( mediumIndex)->addBatch( partNumber, filename, path);
}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#include "Directory.hpp"

#include <arinc665/Arinc665Exception.hpp>

using namespace Arinc665::Media;

Directory::Directory( const string &name):
	name( name)
{
}

const string& Directory::getName( void) const
{
	return name;
}

Directory& Directory::getDirectory( const string &path)
{
	//! Check if supplied parameter is correct
	if (path.empty() || path[0] != '\\')
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "invalid path: '" + path + "'"));
	}

	//! if the path only contains the backslash -> we request ourself
	if (path == "\\")
	{
		return *this;
	}

	//! get next path separator
	string::size_type separatorPos = path.find( '\\', 1);

	//! find sub-directory
	DirectoryPtr subDirectory( getSubDirectory( string( path, 1, separatorPos)));

	if (!subDirectory)
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "Path not found"));
	}

	string tail( path, separatorPos);

	if (tail.empty())
	{
		return *subDirectory;
	}

	return subDirectory->getDirectory( tail);
}

ConstDirectoryMap Directory::getSubDirectories( void) const
{
	return ConstDirectoryMap( subDirectories.begin(), subDirectories.end());
}

DirectoryMap Directory::getSubDirectories( void)
{
	return subDirectories;
}

ConstDirectoryPtr Directory::getSubDirectory( const string &name) const
{
	DirectoryMap::const_iterator subDirectory = subDirectories.find( name);

	if (subDirectory == subDirectories.end())
	{
		return ConstDirectoryPtr();
	}

	return subDirectory->second;
}

DirectoryPtr Directory::getSubDirectory( const string &name)
{
	DirectoryMap::iterator subDirectory = subDirectories.find( name);

	if (subDirectory == subDirectories.end())
	{
		return DirectoryPtr();
	}

	return subDirectory->second;
}

string Directory::getSubDirectoryName( ConstDirectoryPtr subDirectory) const
{
	for ( auto const & subDir : subDirectories)
	{
		if (subDir.second == subDirectory)
		{
			return subDir.first;
		}
	}

	//! @todo throw exception
	return string();
}

DirectoryPtr Directory::addSubDirectory( const string &name)
{
	if (getSubDirectory( name))
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "sub-directory already exists"));
	}

	//! create new sub-directory
	DirectoryPtr subDirectory( std::make_shared< Directory>( name));

	//! insert into map
	subDirectories.insert( std::make_pair( name, subDirectory));

	//! return new sub-directory
	return subDirectory;
}

void Directory::removeSubDirectory( const string &name)
{
	DirectoryMap::iterator subDirectory = subDirectories.find( name);

	if (subDirectory == subDirectories.end())
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "sub-directory does not exists"));
	}

	subDirectories.erase( subDirectory);
}

void Directory::removeSubDirectory( DirectoryPtr subDirectory)
{
	for ( auto &subDir : subDirectories)
	{
		if (subDir.second == subDirectory)
		{
			subDirectories.erase( subDir.first);
			return;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "sub-directory does not exists"));
}


size_t Directory::getNumberOfFiles( bool recursive) const
{
	size_t fileSize( files.size());

	//! descent to sub directories if requested
	if (recursive)
	{
		for (const auto &subDirectory : subDirectories)
		{
			fileSize += subDirectory.second->getNumberOfFiles( true);
		}
	}

	return fileSize;
}

ConstFileMap Directory::getFiles( bool recursive) const
{
	if (!recursive)
	{
		return ConstFileMap( files.begin(), files.end());
	}

	ConstFileMap allfiles( files.begin(), files.end());

	for (const auto &subDirectory : subDirectories)
	{
		ConstFileMap subFiles(
			ConstDirectoryPtr( subDirectory.second)->getFiles( true));

		allfiles.insert( subFiles.begin(), subFiles.end());
	}

	return allfiles;
}

FileMap Directory::getFiles( bool recursive)
{
	if (!recursive)
	{
		return files;
	}

	FileMap allFiles( files);

	for ( auto &subDirectory : subDirectories)
	{
		FileMap subFiles( subDirectory.second->getFiles( true));

		allFiles.insert( subFiles.begin(), subFiles.end());
	}

	return allFiles;
}

ConstFilePtr Directory::getFile( const string &filename, bool recursive) const
{
	FileMap::const_iterator fileIt = files.find( filename);

	//! if file has been found return immediately
	if (fileIt!=files.end())
	{
		return fileIt->second;
	}

	if (recursive)
	{
		for (const auto & subDirectory : subDirectories)
		{
			ConstFilePtr file = subDirectory.second->getFile( filename, true);

			//! if file has been found return immediately
			if (file)
			{
				return file;
			}
		}
	}

	return ConstFilePtr();
}

FilePtr Directory::getFile( const string &filename, bool recursive)
{
	FileMap::const_iterator fileIt = files.find( filename);

	//! if file has been found return immediately
	if (fileIt!=files.end())
	{
		return fileIt->second;
	}

	if (recursive)
	{
		for (const auto & subDirectory : subDirectories)
		{
			FilePtr file = subDirectory.second->getFile( filename, true);

			//! if file has been found return immediately
			if (file)
			{
				return file;
			}
		}
	}

	return FilePtr();
}

string Directory::getFilename( ConstFilePtr file) const
{
	for ( const auto & fileIt : files)
	{
		if (fileIt.second == file)
		{
			return fileIt.first;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "File not found"));
}

FilePtr Directory::addFile(
	const string &filename,
	const uint16_t crc,
	const uint32_t fileLength,
	const string &partNumber)
{
	if (getFile( filename))
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "File already exists"));
	}

	//! create file
	FilePtr file( std::make_shared< File>( filename, crc, fileLength, partNumber));

	//! insert into map
	files.insert( std::make_pair( filename, file));

	//! return new file
	return file;
}

void Directory::removeFile( const string &filename)
{
	FileMap::iterator file = files.find( filename);

	if (file == files.end())
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "File not found"));
	}

	files.erase( file);
}

void Directory::removeFile( ConstFilePtr file)
{
	for ( auto &fileIt : files)
	{
		if (fileIt.second == file)
		{
			files.erase( fileIt.first);
			return;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "File not found"));
}

size_t Directory::getNumberOfLoads( bool recursive) const
{
	size_t numberOfLoads( loads.size());

	//! descent to sub directories if requested
	if (recursive)
	{
		for (const auto &subDirectory : subDirectories)
		{
			numberOfLoads += subDirectory.second->getNumberOfLoads( true);
		}
	}

	return numberOfLoads;
}

ConstLoadMap Directory::getLoads( bool recursive) const
{
	if (!recursive)
	{
		return ConstLoadMap( loads.begin(), loads.end());
	}

	ConstLoadMap allLoads( loads.begin(), loads.end());

	for (const auto &subDirectory : subDirectories)
	{
		ConstLoadMap subLoads(
			ConstDirectoryPtr( subDirectory.second)->getLoads( true));

		allLoads.insert( subLoads.begin(), subLoads.end());
	}

	return allLoads;
}

LoadMap Directory::getLoads( bool recursive)
{
	if (!recursive)
	{
		return loads;
	}

	LoadMap allLoads( loads);

	for ( auto &subDirectory : subDirectories)
	{
		LoadMap subLoads( subDirectory.second->getLoads( true));

		allLoads.insert( subLoads.begin(), subLoads.end());
	}

	return allLoads;
}

ConstLoadPtr Directory::getLoad( const string &filename) const
{
	LoadMap::const_iterator loadIt = loads.find( filename);

	if (loadIt != loads.end())
	{
		return loadIt->second;
	}

	return ConstLoadPtr();
}

LoadPtr Directory::getLoad( const string &filename)
{
	LoadMap::iterator loadIt = loads.find( filename);

	if (loadIt != loads.end())
	{
		return loadIt->second;
	}

	return LoadPtr();
}

string Directory::getLoadName( ConstLoadPtr load)
{
	for ( const auto & loadIt : loads)
	{
		if (loadIt.second == load)
		{
			return loadIt.first;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "Load does not exists"));
}

LoadPtr Directory::addLoad( const string &filename, const string &partNumber)
{
	if (getLoad( filename))
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "Load already exists"));
	}

	//! create file
	LoadPtr load( std::make_shared< Load>( partNumber));

	//! insert into map
	loads.insert( std::make_pair( filename, load));

	//! return new file
	return load;
}

void Directory::removeLoad( const string &filename)
{
	LoadMap::iterator load = loads.find( filename);

	if (load == loads.end())
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "Load does not exists"));
	}

	loads.erase( load);
}

void Directory::removeLoad( ConstLoadPtr load)
{
	for ( auto & loadIt : loads)
	{
		if (loadIt.second == load)
		{
			loads.erase( loadIt.first);
			return;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "Load does not exists"));
}

size_t Directory::getNumberOfBatches( bool recursive) const
{
	size_t numberOfBatches( batches.size());

	//! descent to sub directories if requested
	if (recursive)
	{
		for (const auto &subDirectory : subDirectories)
		{
			numberOfBatches += subDirectory.second->getNumberOfBatches( true);
		}
	}

	return numberOfBatches;
}

ConstBatchMap Directory::getBatches( bool recursive) const
{
	if (!recursive)
	{
		return ConstBatchMap( batches.begin(), batches.end());
	}

	ConstBatchMap allBatches( batches.begin(), batches.end());

	for (const auto &subDirectory : subDirectories)
	{
		ConstBatchMap subBatches(
			ConstDirectoryPtr( subDirectory.second)->getBatches( true));

		allBatches.insert( subBatches.begin(), subBatches.end());
	}

	return allBatches;
}

BatchMap Directory::getBatches( bool recursive)
{
	if (!recursive)
	{
		return batches;
	}

	BatchMap allBatches( batches);

	for (auto &subDirectory : subDirectories)
	{
		BatchMap subBatches( subDirectory.second->getBatches( true));

		allBatches.insert( subBatches.begin(), subBatches.end());
	}

	return allBatches;
}

ConstBatchPtr Directory::getBatch( const string &filename) const
{
	BatchMap::const_iterator batchIt = batches.find( filename);

	if (batchIt != batches.end())
	{
		return batchIt->second;
	}

	return ConstBatchPtr();
}

BatchPtr Directory::getBatch( const string &filename)
{
	BatchMap::iterator batchIt = batches.find( filename);

	if (batchIt != batches.end())
	{
		return batchIt->second;
	}

	return BatchPtr();
}

string Directory::getBatchName( ConstBatchPtr batch)
{
	for ( const auto & batchIt : batches)
	{
		if (batchIt.second == batch)
		{
			return batchIt.first;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "batch does not exists"));
}

BatchPtr Directory::addBatch( const string &filename, const string &partNumber)
{
	if (getBatch( filename))
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "batch does not exists"));
	}

	//! create file
	BatchPtr batch( std::make_shared< Batch>( partNumber));

	//! insert into map
	batches.insert( std::make_pair( filename, batch));

	//! return new file
	return batch;
}

void Directory::removeBatch( const string &filename)
{
	BatchMap::iterator batch = batches.find( filename);

	if (batch == batches.end())
	{
		BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
			AdditionalInfo( "batch does not exists"));
	}

	batches.erase( batch);
}

void Directory::removeBatch( ConstBatchPtr batch)
{
	for ( auto & batchIt : batches)
	{
		if (batchIt.second == batch)
		{
			batches.erase( batchIt.first);
			return;
		}
	}

	BOOST_THROW_EXCEPTION( Arinc665::Arinc665Exception() <<
		AdditionalInfo( "batch does not exists"));
}

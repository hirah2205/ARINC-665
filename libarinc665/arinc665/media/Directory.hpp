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
 * @brief Declaration of class Arinc665::Directory.
 **/

#ifndef ARINC665_MEDIA_DIRECTORY_HPP
#define ARINC665_MEDIA_DIRECTORY_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/File.hpp>
#include <arinc665/media/Load.hpp>
#include <arinc665/media/Batch.hpp>

#include <string>

namespace Arinc665
{
	namespace Media
	{
		using std::string;
		/**
		 *
		 **/
		class Directory
		{
			public:
				Directory( const string &name);

				const string& getName( void) const;

				Directory& getDirectory( const string &path);

				ConstDirectoryMap getSubDirectories( void) const;

				DirectoryMap getSubDirectories( void);

				ConstDirectoryPtr getSubDirectory( const string &name) const;

				DirectoryPtr getSubDirectory( const string &name);

				string getSubDirectoryName( ConstDirectoryPtr subDirectory) const;

				DirectoryPtr addSubDirectory( const string &name);

				void removeSubDirectory( const string &name);

				void removeSubDirectory( DirectoryPtr subDirectory);

				/**
				 * @brief Returns the number of files within this directory and
				 *   sub-directories.
				 *
				 * @return The number of files within this directory.
				 **/
				size_t getNumberOfFiles( bool recursive = false) const;

				ConstFileMap getFiles( bool recursive = false) const;

				FileMap getFiles( bool recursive = false);

				ConstFilePtr getFile(
					const string &filename,
					bool recursive = false) const;

				FilePtr getFile( const string &filename, bool recursive = false);

				string getFilename( ConstFilePtr file) const;

				/**
				 * @brief Adds a file into this directory.
				 *
				 * @param filename
				 * @param crc
				 * @param fileLength
				 * @param partNumber
				 *
				 * @return
				 **/
				FilePtr addFile(
					const string &filename,
					const uint16_t crc,
					const uint32_t fileLength = 0,
					const string &partNumber = string());

				void removeFile( const string &filename);

				void removeFile( ConstFilePtr file);


				size_t getNumberOfLoads( bool recursive = false) const;

				ConstLoadMap getLoads(  bool recursive = false) const;

				LoadMap getLoads( bool recursive = false);

				ConstLoadPtr getLoad( const string &filename) const;

				LoadPtr getLoad( const string &filename);

				string getLoadName( ConstLoadPtr load);

				LoadPtr addLoad(
					const string &filename,
					const string &partNumber);

				void removeLoad( const string &filename);

				void removeLoad( ConstLoadPtr load);


				size_t getNumberOfBatches( bool recursive = false) const;

				ConstBatchMap getBatches( bool recursive = false) const;

				BatchMap getBatches( bool recursive = false);

				ConstBatchPtr getBatch( const string &filename) const;

				BatchPtr getBatch( const string &filename);

				string getBatchName( ConstBatchPtr batch);

				BatchPtr addBatch(
					const string &filename,
					const string &partNumber);

				void removeBatch( const string &filename);

				void removeBatch( ConstBatchPtr batch);

			private:
				const string name;
				DirectoryMap subDirectories;
				FileMap files;
				LoadMap loads;
				BatchMap batches;
		};
	}
}

#endif

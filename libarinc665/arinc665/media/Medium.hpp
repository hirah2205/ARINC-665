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
 * @brief Declaration of class Arinc665::Media::Medium.
 **/

#ifndef ARINC665_MEDIA_MEDIUM_HPP
#define ARINC665_MEDIA_MEDIUM_HPP

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
		 * @brief Represents a media within an ARINC 665 media set
		 **/
		class Medium
		{
			public:
				Medium( const unsigned int index);

				/**
				 * @brief Returns the index of the medium within the media set.
				 *
				 * @return The index of the media within the media set.
				 **/
				unsigned int getIndex( void) const;

				/**
				 * @brief Returns the root directory of the medium.
				 *
				 * @return The root directory of the medium.
				 **/
				ConstDirectoryPtr getRootDirectory( void) const;

				/**
				 * @brief Returns the root directory of the medium.
				 *
				 * @return The root directory of the medium.
				 **/
				DirectoryPtr getRootDirectory( void);

				/**
				 * @brief Returns the number of files on this medium.
				 *
				 * @return The number of files on this medium.
				 **/
				size_t getNumberOfFiles( void) const;

				ConstFileMap getFiles( void) const;

				FileMap getFiles( void);

				ConstFilePtr getFile( const string &filename) const;

				FilePtr getFile( const string &filename);

				FilePtr addFile(
					const string &filename,
					const string &path,
					const uint16_t crc,
					const uint32_t fileLength = 0,
					const string &partNumber = string());

				size_t getNumberOfLoads( void) const;

				ConstLoadMap getLoads( void) const;

				LoadPtr addLoad(
					const string &filename,
					const string &path,
					const string &partNumber);

				size_t getNumberOfBatches( void) const;

				ConstBatchMap getBatches( void) const;

				BatchPtr addBatch(
					const string &filename,
					const string &path,
					const string &partNumber);

			private:
				unsigned int index;     // must be non-const to allow operator =
				DirectoryPtr rootDirectory;
		};
	}
}

#endif

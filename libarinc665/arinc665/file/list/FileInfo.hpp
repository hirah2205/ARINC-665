/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class FileInfo
 **/

#ifndef ARINC665_FILEINFO_HPP
#define ARINC665_FILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665
{
	namespace File
	{
		using std::string;

		/**
		 * @brief The file information, which is part of the FileListFile.
		 **/
		class FileInfo
		{
			public:
				static std::list< FileInfo> getFileList( std::vector< uint8_t>::const_iterator &it);


				FileInfo( void);

				FileInfo( std::vector< uint8_t>::const_iterator &it);


				string getFilename( void) const;

				void setFilename( const string &filename);


				string getPathName( void) const;

				void setPathName( const string &pathName);


				uint16_t getMemberSequenceNumber( void) const;

				void setMemberSequenceNumber( const uint16_t memberSequenceNumber);


				uint16_t getCrc( void) const;

				void setCrc( const uint16_t crc);

			private:
				string filename;
				string pathName;
				uint16_t memberSequenceNumber;
				uint16_t crc;
		};
	}
}

#endif

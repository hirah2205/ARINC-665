/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class BatchInfo
 **/

#ifndef ARINC665_FILE_BATCHINFO_HPP
#define ARINC665_FILE_BATCHINFO_HPP

#include <arinc665/file/File.hpp>

namespace Arinc665
{
	namespace File
	{
		using std::string;

		class BatchInfo
		{
			public:
				string getPartNumber( void) const;

				void setPartNumber( const string &partNumber);


				string getFilename( void) const;

				void setFilename( const string &filename);


				uint16_t getMemberSequenceNumber( void) const;

				void setMemberSequenceNumber( const uint16_t memberSequenceNumber);

			private:
				string partNumber;
				string filename;
				uint16_t memberSequenceNumber;
		};
	}
}

#endif

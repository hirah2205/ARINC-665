/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class BatchLoadInfo
 **/

#ifndef ARINC665_FILE_BATCHLOADINFO_HPP
#define ARINC665_FILE_BATCHLOADINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>

namespace Arinc665
{
	namespace File
	{
		using std::string;

		/**
		 * @brief The load info the batch load entry.
		 **/
		class BatchLoadInfo
		{
			public:
				string getHeaderFilename( void) const;

				void setHeaderFilename( const string &headerFilename);


				string getPartNumber( void) const;

				void setPartNumber( const string &partNumber);

			private:
				string headerFilename;
				string partNumber;
		};
	}
}

#endif

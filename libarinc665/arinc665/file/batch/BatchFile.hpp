/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class BatchFile
 **/

#ifndef ARINC665_FILE_BATCH_HPP
#define ARINC665_FILE_BATCH_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/batch/TargetHardwareLoad.hpp>

#include <string>
#include <list>

namespace Arinc665
{
	namespace File
	{
		using std::string;

		/**
		 * @brief A batch file represents a list of loads of target hardwares.
		 *
		 *
		 **/
		class BatchFile : public Arinc665File
		{
			public:
				typedef std::list< TargetHardwareLoad> ListType;

				BatchFile( void);

				BatchFile( const RawFile &file);

				virtual Arinc665Version getArincVersion( void) const override;

				string getPartNumber( void) const;

				void setPartNumber( const string &partNumber);


				string getComment( void) const;

				void setComment( const string &comment);


				const ListType& getTargetHardwareLoadList( void) const;

				ListType& getTargetHardwareLoadList( void);

			private:
				string partNumber;
				string comment;
				ListType targetHardwareLoadList;
		};
	}
}

#endif

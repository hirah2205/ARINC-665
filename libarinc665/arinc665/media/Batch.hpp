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
 * @brief Declaration of class Batch
 **/

#ifndef ARINC665_MEDIA_BATCH_HPP
#define ARINC665_MEDIA_BATCH_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/PartNumberdEntity.hpp>
#include <arinc665/media/BatchInfo.hpp>

#include <string>
#include <list>

namespace Arinc665
{
	namespace Media
	{
		using std::string;

		/**
		 * @brief
		 **/
		class Batch : public PartNumberdEntity
		{
			public:
				typedef std::list< BatchInfo> BatchInfoList;

				Batch( const string &partNumber, const string &comment = string());

				/**
				 * @brief Get the comment, which describes the batch.
				 *
				 * @return The comment, which describes the batch.
				 **/
				string getComment( void) const;

				/**
				 * @brief Set the comment, which describes the batch.
				 *
				 * @param[in] comment
				 *   The comment, which describes the batch.
				 **/
				void setComment( const string &comment);


				const BatchInfoList& getBatchInfos( void);

				BatchInfo& addBatchInfo( const string &targetHardwareId);

			private:
				string comment;
				BatchInfoList batchInfos;
		};
	}
}

#endif

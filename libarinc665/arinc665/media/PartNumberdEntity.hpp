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
 * @brief Declaration of class PartNumberdEntity.
 **/

#ifndef ARINC665_MEDIA_PARTNUMBERDENTITY_HPP
#define ARINC665_MEDIA_PARTNUMBERDENTITY_HPP

#include <arinc665/media/Media.hpp>
#include <string>

namespace Arinc665
{
	namespace Media
	{
		using std::string;

		/**
		 * @brief Base class for classes, which have a part number
		 **/
		class PartNumberdEntity
		{
			public:
				/**
				 * @brief Return the part number of the entity.
				 *
				 * @return The part number of the entity
				 **/
				string getPartNumber( void) const;

				/**
				 * @brief Set the part number of the entity.
				 *
				 * @param[in] partNumber
				 *   The part number
				 **/
				void setPartNumber( const string &partNumber);

			protected:
				/**
				 * @brief Creates the part numbered entity.
				 *
				 * @param[in] partNumber
				 *   The part number
				 **/
				PartNumberdEntity( const string &partNumber);

			private:
				//! The part number
				string partNumber;
		};
	}
}

#endif

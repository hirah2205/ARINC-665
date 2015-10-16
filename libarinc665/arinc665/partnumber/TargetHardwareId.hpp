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
 * @brief Declaration of class TargetHardwareId.
 **/

#ifndef ARINC665_TARGETHARDWAREWID_HPP
#define ARINC665_TARGETHARDWAREWID_HPP

#include <arinc665/Arinc665.hpp>
#include <string>

namespace Arinc665
{
	using std::string;

	/**
	 * @brief Represents a ARINC 665 target hardware ID (THW ID).
	 **/
	class TargetHardwareId
	{
		public:
			/**
			 * @brief Constructs a target hardware ID with the given string.
			 *
			 * @param[in] targetHardwareId
			 *   The target hardware ID as string.
			 **/
			TargetHardwareId( const string &targetHardwareId);

			/**
			 * @brief Returns the target hardware ID as string.
			 *
			 * @return The target hardware ID as string.
			 **/
			const string& get( void) const;

			/**
			 * @brief Sets the target hardware ID to the given string.
			 *
			 * @param[in] targetHardwareId
			 *   The target hardware ID as string.
			 **/
			void set( const string &targetHardwareId);

		private:
			//! The target hardware ID
			string targetHardwareId;
	};
}

#endif

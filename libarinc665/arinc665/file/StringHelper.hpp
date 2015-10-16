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
 * @brief Declaration of string helper functions
 *
 * The string helper functions are used to decode strings, which are encoded
 * within the ARINC 665 protocol files.
 **/

#ifndef ARINC665_FILE_STRINGHELPER_HPP
#define ARINC665_FILE_STRINGHELPER_HPP

#include <arinc665/file/File.hpp>

#include <helper/Endianess.hpp>

#include <string>
#include <vector>
#include <list>
#include <cstdint>

namespace Arinc665
{
	namespace File
	{
		/**
		 * @brief
		 *
		 * @param[in] it
		 * @param[out] str
		 *
		 * @return
		 **/
		std::vector< uint8_t>::const_iterator getString(
			std::vector< uint8_t>::const_iterator it,
			std::string &str);

		/**
		 * @brief
		 *
		 * @param[in,out] it
		 * @param[in] str
		 **/
		std::vector< uint8_t>::iterator setString(
			std::vector< uint8_t>::iterator it,
			const std::string &str);

		/**
		 * @brief
		 *
		 * @param[in,out] it
		 * @param strList
		 *
		 * @return
		 **/
		std::vector< uint8_t>::const_iterator getStringList(
			std::vector< uint8_t>::const_iterator it,
			std::list< std::string> &strList);

		/**
		 * @brief
		 *
		 * @param[in,out] it
		 * @param[in] strList
		 **/
		std::vector< uint8_t>::iterator setStringList(
			std::vector< uint8_t>::iterator it,
			const std::list< std::string> &strList);
	}
}

#endif

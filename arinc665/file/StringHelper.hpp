/*
 * $Date$
 * $Revision$
 */
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
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

#include <string>
#include <vector>
#include <list>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Decodes the ARINC 665 string from the stream.
 *
 * @param[in] it
 * @param[out] str
 *
 * @return New iterator position.
 **/
RawFile::const_iterator getString(
  RawFile::const_iterator it,
  std::string &str);

/**
 * @brief Encodes the ARINC 665 string to the stream.
 *
 * @param[in] it
 * @param[in] str
 *
 * @return New iterator position.
 **/
RawFile::iterator setString(
  RawFile::iterator it,
  const std::string &str);

std::size_t getRawStringLength( const std::string &str);

RawFile getRawString( const std::string &str);

/**
 * @brief Decodes the ARINC 665 string list from the stream.
 *
 * @param[in] it
 * @param[out] strList
 *
 *
 * @return New iterator position.
 **/
RawFile::const_iterator getStringList(
  RawFile::const_iterator it,
  std::list< std::string> &strList);

/**
 * @brief Encodes the ARINC 665 string list to the stream.
 *
 * @param[in] it
 * @param[in] strList
 *
 * @return New iterator position.
 **/
RawFile::iterator setStringList(
  RawFile::iterator it,
  const std::list< std::string> &strList);

}
}

#endif

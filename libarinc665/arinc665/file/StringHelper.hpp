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
 * @brief
 *
 * @param[in] it
 * @param[out] str
 *
 * @return
 **/
RawFile::const_iterator getString(
  RawFile::const_iterator it,
  std::string &str);

/**
 * @brief
 *
 * @param[in,out] it
 * @param[in] str
 **/
RawFile::iterator setString(
  RawFile::iterator it,
  const std::string &str);

/**
 * @brief
 *
 * @param[in,out] it
 * @param strList
 *
 * @return
 **/
RawFile::const_iterator getStringList(
  RawFile::const_iterator it,
  std::list< std::string> &strList);

/**
 * @brief
 *
 * @param[in,out] it
 * @param[in] strList
 **/
RawFile::iterator setStringList(
  RawFile::iterator it,
  const std::list< std::string> &strList);

}
}

#endif

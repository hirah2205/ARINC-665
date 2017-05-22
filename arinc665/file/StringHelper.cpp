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
 * @brief Definition of string helper functions
 **/

#include "StringHelper.hpp"

#include <helper/Logger.hpp>
#include <helper/Endianess.hpp>


namespace Arinc665 {
namespace File {

RawFile::const_iterator getString( RawFile::const_iterator it, std::string &str)
{
  // determine string length
  uint16_t strLength;
  it = getInt< uint16_t>( it, strLength);

  // copy string
  str.assign( it, it + strLength);
  it += strLength;

  if ( strLength % 2 == 1)
  {
    ++it;
  }

  return it;
}

RawFile::iterator setString( RawFile::iterator it, const std::string &str)
{
  // set string length
  it = setInt< uint16_t>( it, static_cast< uint16_t>( str.size()));

  // copy string
  it = std::copy( str.begin(), str.end(), it);

  // fill string if it is odd
  if ( str.size() % 2 == 1)
  {
    *it = 0;
    ++it;
  }

  return it;
}

std::size_t getRawStringLength( const std::string &str)
{
  return sizeof( uint16_t) + 2 * ((str.size() + 1) / 2);
}

RawFile getRawString( const std::string &str)
{
  RawFile rawString( getRawStringLength( str));

  setString( rawString.begin(), str);

  return rawString;
}

RawFile::const_iterator getStringList(
  RawFile::const_iterator it,
  std::list< std::string> &strList)
{
  // number of strings
  uint16_t numberOfEntries;
  it = getInt< uint16_t>( it, numberOfEntries);

  for ( unsigned int index = 0; index < numberOfEntries; ++index)
  {
    // string
    std::string str;
    it = getString( it, str);
    strList.push_back( str);
  }

  return it;
}

RawFile::iterator setStringList(
  RawFile::iterator it,
  const std::list< std::string> &strList)
{
  // set number of strings
  it = setInt< uint16_t>( it, static_cast< uint16_t>( strList.size()));

  for ( std::list< std::string>::const_iterator strIt = strList.begin();
    strIt != strList.end(); ++strIt)
  {
    // string
    it = setString( it, *strIt);
  }

  return it;
}

}
}

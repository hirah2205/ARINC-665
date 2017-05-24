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
 * @brief Definition of class Arinc665::File::BatchLoadInfo.
 **/

#include "BatchLoadInfo.hpp"

namespace Arinc665 {
namespace File {

BatchLoadInfo::BatchLoadInfo(
  const string &headerFilename,
  const string &partNumber):
  headerFilename( headerFilename),
  partNumber( partNumber)
{
}

BatchLoadInfo:: BatchLoadInfo(
  string &&headerFilename,
  string &&partNumber):
  headerFilename( headerFilename),
  partNumber( partNumber)
{
}

BatchLoadInfo::string BatchLoadInfo::getHeaderFilename() const
{
  return headerFilename;
}

void BatchLoadInfo::setHeaderFilename( const string &headerFilename)
{
  this->headerFilename= headerFilename;
}

BatchLoadInfo::string BatchLoadInfo::getPartNumber() const
{
  return partNumber;
}

void BatchLoadInfo::setPartNumber( const string &partNumber)
{
  this->partNumber= partNumber;
}

}
}

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
  headerFilenameValue( headerFilename),
  partNumberValue( partNumber)
{
}

BatchLoadInfo:: BatchLoadInfo(
  string &&headerFilename,
  string &&partNumber):
  headerFilenameValue( headerFilename),
  partNumberValue( partNumber)
{
}

BatchLoadInfo::string BatchLoadInfo::headerFilename() const
{
  return headerFilenameValue;
}

void BatchLoadInfo::headerFilename( const string &headerFilename)
{
  headerFilenameValue = headerFilename;
}

BatchLoadInfo::string BatchLoadInfo::partNumber() const
{
  return partNumberValue;
}

void BatchLoadInfo::partNumber( const string &partNumber)
{
  partNumberValue = partNumber;
}

}
}

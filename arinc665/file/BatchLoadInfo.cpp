/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::BatchLoadInfo.
 **/

#include "BatchLoadInfo.hpp"

namespace Arinc665::File {

BatchLoadInfo::BatchLoadInfo(
  const std::string &headerFilename,
  const std::string &partNumber):
  headerFilenameV( headerFilename),
  partNumberV( partNumber)
{
}

BatchLoadInfo:: BatchLoadInfo(
  std::string &&headerFilename,
  std::string &&partNumber):
  headerFilenameV( std::move( headerFilename)),
  partNumberV( std::move( partNumber))
{
}

std::string BatchLoadInfo::headerFilename() const
{
  return headerFilenameV;
}

void BatchLoadInfo::headerFilename( const std::string &headerFilename)
{
  headerFilenameV = headerFilename;
}

std::string BatchLoadInfo::partNumber() const
{
  return partNumberV;
}

void BatchLoadInfo::partNumber( const std::string &partNumber)
{
  partNumberV = partNumber;
}

}

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
  std::string_view headerFilename,
  std::string_view partNumber):
  headerFilenameV{ headerFilename},
  partNumberV{ partNumber}
{
}

BatchLoadInfo:: BatchLoadInfo(
  std::string &&headerFilename,
  std::string &&partNumber):
  headerFilenameV{ std::move( headerFilename)},
  partNumberV{ std::move( partNumber)}
{
}

std::string_view BatchLoadInfo::headerFilename() const
{
  return headerFilenameV;
}

void BatchLoadInfo::headerFilename( std::string_view headerFilename)
{
  headerFilenameV = headerFilename;
}

void BatchLoadInfo::headerFilename( std::string &&headerFilename)
{
  headerFilenameV = std::move( headerFilename);
}

std::string_view BatchLoadInfo::partNumber() const
{
  return partNumberV;
}

void BatchLoadInfo::partNumber( std::string_view partNumber)
{
  partNumberV = partNumber;
}

void BatchLoadInfo::partNumber( std::string &&partNumber)
{
  partNumberV = std::move( partNumber);
}

}

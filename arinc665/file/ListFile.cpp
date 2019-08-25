/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::File::ListFile.
 **/

#include "ListFile.hpp"

namespace Arinc665::File {

ListFile::ListFile(
  const FileType fileType,
  const SupportedArinc665Version version,
  const std::size_t checksumPosition):
  Arinc665File( fileType, version, checksumPosition)
{
}
ListFile::
ListFile(
  FileType fileType,
  const RawFile &rawFile,
  std::size_t checksumPosition):
  Arinc665File( fileType, rawFile, checksumPosition)
{
}

}

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
  const SupportedArinc665Version version,
  const std::size_t checksumPosition) noexcept:
  Arinc665File{ version, checksumPosition}
{
}
ListFile::ListFile(
  const ConstRawFileSpan &rawFile,
  FileType expectedFileType,
  std::size_t checksumPosition):
  Arinc665File{ rawFile, expectedFileType, checksumPosition}
{
}

}

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
 * @brief Definition of class Arinc665::Utils::Arinc665Utils.
 **/

#include "Arinc665Utils.hpp"

#include <arinc665/utils/implementation/MediaSetImporterImpl.hpp>
#include <arinc665/utils/implementation/MediaSetExporterImpl.hpp>

namespace Arinc665 {
namespace Utils {

Arinc665Utils::Arinc665Importer Arinc665Utils::createArinc665Importer(
  GetMediumPathHandler getMediumPathHandler)
{
  return std::bind(
    &MediaSetImporterImpl::operator(),
    std::make_shared< MediaSetImporterImpl>( getMediumPathHandler),
    std::placeholders::_1);
}

Arinc665Utils::Arinc665Exporter Arinc665Utils::createArinc665Exporter(
  Media::ConstMediaSetPtr mediaSet,
  GetMediumPathHandler getMediumPathHandler,
  CopyFileHandler copyFileHandler,
  bool createBatchFiles,
  bool createLoadHeaderFiles)
{
  return std::bind(
    &MediaSetExporterImpl::operator(),
    std::make_shared< MediaSetExporterImpl>(
      mediaSet,
      getMediumPathHandler,
      copyFileHandler,
      createBatchFiles,
      createLoadHeaderFiles));
}

}
}

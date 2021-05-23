/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Utils::Arinc665Utils.
 **/

#include "Arinc665Utils.hpp"

#include <arinc665/utils/implementation/MediaSetImporterImpl.hpp>
#include <arinc665/utils/implementation/MediaSetExporterImpl.hpp>
#include <arinc665/utils/implementation/MediaSetValidatorImpl.hpp>

namespace Arinc665::Utils {

Arinc665Utils::Arinc665Importer Arinc665Utils::arinc665Importer(
  ReadFileHandler readFileHandler,
  const bool checkFileIntegrity )
{
  return std::bind(
    &MediaSetImporterImpl::operator(),
    std::make_shared< MediaSetImporterImpl>(
      std::move( readFileHandler ),
      checkFileIntegrity ) );
}

Arinc665Utils::Arinc665Validator Arinc665Utils::arinc665Validator(
  ReadFileHandler readFileHandler,
  ValidatorInformationHandler informationHandler )
{
  return std::bind(
    &MediaSetValidatorImpl::operator(),
    std::make_shared< MediaSetValidatorImpl>(
      std::move( readFileHandler ),
      std::move( informationHandler ) ) );
}

Arinc665Utils::Arinc665Exporter Arinc665Utils::arinc665Exporter(
  Media::ConstMediaSetPtr mediaSet,
  CreateMediumHandler createMediumHandler,
  CreateDirectoryHandler createDirectoryHandler,
  CheckFileExistenceHandler checkFileExistenceHandler,
  CreateFileHandler createFileHandler,
  WriteFileHandler writeFileHandler,
  ReadFileHandler readFileHandler,
  const SupportedArinc665Version arinc665Version,
  const FileCreationPolicy createBatchFiles,
  const FileCreationPolicy createLoadHeaderFiles )
{
  return std::bind(
    &MediaSetExporterImpl::operator(),
    std::make_shared< MediaSetExporterImpl>(
      mediaSet,
      createMediumHandler,
      createDirectoryHandler,
      checkFileExistenceHandler,
      createFileHandler,
      writeFileHandler,
      readFileHandler,
      arinc665Version,
      createBatchFiles,
      createLoadHeaderFiles ) );
}

}

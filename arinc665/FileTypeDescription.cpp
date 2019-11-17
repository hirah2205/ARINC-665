/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::FileTypeDescription.
 **/

#include "FileTypeDescription.hpp"

namespace Arinc665 {

using namespace std::literals;

FileTypeDescription::FileTypeDescription():
  Description{
    { "BatchFile"sv,        FileType::BatchFile},
    { "LoadUploadHeader"sv, FileType::LoadUploadHeader},
    { "LoadList"sv,         FileType::LoadList},
    { "BatchList"sv,        FileType::BatchList},
    { "FileList"sv,         FileType::FileList},
  }
{
}

std::ostream& operator<<( std::ostream &stream, const FileType fileType)
{
  return (stream << FileTypeDescription::instance().name( fileType));
}

}

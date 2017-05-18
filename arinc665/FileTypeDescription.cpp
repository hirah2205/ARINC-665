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
 * @brief Definition of class Arinc665::FileTypeDescription.
 **/

#include "FileTypeDescription.hpp"

namespace Arinc665 {

FileTypeDescription::FileTypeDescription():
  Description{
    { "BatchFile",        FileType::BatchFile},
    { "LoadUploadHeader", FileType::LoadUploadHeader},
    { "LoadList",         FileType::LoadList},
    { "BatchList",        FileType::BatchList},
    { "FileList",         FileType::FileList},

    { "Invalid",          FileType::Invalid}
  }
{
}

std::ostream& operator<<( std::ostream &stream, const FileType fileType)
{
  return (stream << FileTypeDescription::getInstance().getDescription(
    fileType).name);
}

}

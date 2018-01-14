/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::Media::File.
 **/

#ifndef ARINC665_MEDIA_FILE_HPP
#define ARINC665_MEDIA_FILE_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/BaseFile.hpp>

#include <string>
#include <cstdint>

namespace Arinc665 {
namespace Media {

/**
 * @brief Data file within media set.
 **/
class File: public BaseFile
{
  public:
    /**
     * @brief Creates a file, which is part of an media within a media set
     *
     * The list of files does not handle the protocol files like:
     * @li List of files file
     * @li List of loads file
     * @li List of batches file
     * @li Load upload header file
     * @li Batch file
     *
     * @param[in] parent
     *   The parent of this file.
     * @param[in] name
     *   Name of the file.
     **/
    File( ContainerEntityPtr parent, const string &name);

    //! @copydoc File()
    File( ContainerEntityPtr parent, string &&name);

    /**
     * @copydoc BaseFile::fileType
     * @retval FileType::RegularFile
     *   Always
     **/
    FileType fileType() const final;
};

}
}

#endif

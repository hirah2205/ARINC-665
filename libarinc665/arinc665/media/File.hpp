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
 * @brief
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
     * @param[in] name
     *   Name of the file.
     * @param[in] crc
     *   The CRC-16 of the file
     * @param[in] fileLength
     *   The file length - only used for load member files.
     * @param[in] partNumber
     *   The part number. (Defaults to empty string - is only used for load
     *   member files.)
     **/
    File(
      const string &name,
      const uint16_t crc,
      const uint32_t fileLength = 0,
      const string &partNumber = string());

    virtual FileType getFileType( void) const override;

    /**
     * @brief Returns the CRC of the file.
     *
     * @return The CRC of the file.
     **/
    uint16_t getCrc( void) const;

    /**
     * @brief Sets the CRC of the file.
     *
     * @param[in] crc
     *   The CRC of the file.
     **/
    void setCrc( const uint16_t crc);

    /**
     * @brief Returns the length of the file.
     *
     * @return The length of the file.
     **/
    uint32_t getFileLength( void) const;

    /**
     * @brief Sets the length of the file.
     *
     * @param[in] fileLength
     *   The file length.
     **/
    void setFileLength( const uint32_t fileLength);

  private:
    //! The CRC
    uint16_t crc;
    //! The file length
    uint32_t fileLength;
};

}
}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::LoadFileInfo.
 **/

#ifndef ARINC665_FILE_LOAD_LOADFILEINFO_HPP
#define ARINC665_FILE_LOAD_LOADFILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief the File information within the file list of the load upload header.
 *
 * @sa LoadHeaderFile
 **/
class LoadFileInfo
{
  public:
    /**
     * @brief Initialise the load file information with the given data.
     *
     * @param[in] filename
     *   filename
     * @param partNumber
     *   Part number
     * @param length
     *   File size
     * @param crc
     *   File CRC
     **/
    LoadFileInfo(
      const std::string &filename,
      const std::string &partNumber,
      uint32_t length,
      uint16_t crc);

    //! @copydoc LoadFileInfo::LoadFileInfo(const std::string&,const std::string&,uint32_t,uint16_t)
    LoadFileInfo(
      std::string &&filename,
      std::string &&partNumber,
      uint32_t length,
      uint16_t crc);

    /**
     * @return The filename.
     *
     * @return The filename.
     **/
    std::string filename() const;

    /**
     * @brief Updates the filename.
     *
     * @param[in] filename
     *   The filename.
     **/
    void filename( const std::string &filename);

    //! @copydoc filename(const std::string&)
    void filename( std::string &&filename);

    /**
     * @return Returns the part number.
     *
     * @return The part number.
     **/
    std::string partNumber() const;

    /**
     * @brief Updates the file part number.
     *
     * @param[in] partNumber
     *   The file part number.
     **/
    void partNumber( const std::string &partNumber);

    //! @copydoc partNumber(const std::string&)
    void partNumber( std::string &&partNumber);

    /**
     * @return Returns the file size.
     *
     * @return The file size.
     **/
    uint32_t length() const;

    /**
     * @brief Updates the file size.
     *
     * @param[in] length
     *   The file size.
     **/
    void length( uint32_t length);

    /**
     * @return Returns the file CRC.
     *
     * @return The file CRC.
     **/
    uint16_t crc() const;

    /**
     * @brief Updates the file CRC.
     *
     * @param[in] crc
     *   The file CRC.
     **/
    void crc( uint16_t crc);

  private:
    //! Filename
    std::string filenameValue;
    //! File part number
    std::string partNumberValue;
    //! File length
    uint32_t lengthValue;
    //! File CRC
    uint16_t crcValue;
};

}
}

#endif

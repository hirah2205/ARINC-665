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
#include <optional>
#include <cstdint>

namespace Arinc665::File {

/**
 * @brief the File information within the file list of the load upload header.
 *
 * The file size is always interpreted in number of bytes (octets).
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
     * @param[in] partNumber
     *   Part number
     * @param[in] length
     *   File size
     * @param[in] crc
     *   File CRC
     * @param[in] checkValue
     *   Check Value.
     **/
    LoadFileInfo(
      const std::string &filename,
      const std::string &partNumber,
      uint64_t length,
      uint16_t crc,
      const std::optional< CheckValue> &checkValue = {});

    //! @copydoc LoadFileInfo::LoadFileInfo(const std::string&,const std::string&,uint32_t,uint16_t,const std::optional<CheckValue>&)
    LoadFileInfo(
      std::string &&filename,
      std::string &&partNumber,
      uint64_t length,
      uint16_t crc,
      std::optional< CheckValue> &&checkValue = {});

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
    uint64_t length() const;

    /**
     * @brief Updates the file size.
     *
     * @param[in] length
     *   The file size.
     **/
    void length( uint64_t length);

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

    /**
     * @brief Returns the Check Value.
     *
     * @return The Check Value.
     **/
    const std::optional< CheckValue>& checkValue() const;

    /**
     * @brief Updates the Check Value
     *
     * @param[in] value
     *   Check Value.
     **/
    void checkValue( const std::optional< CheckValue> &checkValue);

    //! @copydoc checkValue(const std::optional<CheckValue>&)
    void checkValue( std::optional< CheckValue> &&checkValue);

  private:
    //! Filename
    std::string filenameValue;
    //! File part number
    std::string partNumberValue;
    //! File length (Always in bytes)
    uint64_t lengthValue;
    //! File CRC
    uint16_t crcValue;
    //! Check Value (since ARINC 665-3)
    std::optional< CheckValue> checkValueValue;
};

}

#endif

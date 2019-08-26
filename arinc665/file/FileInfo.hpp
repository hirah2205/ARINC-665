/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::FileInfo.
 **/

#ifndef ARINC665_FILEINFO_HPP
#define ARINC665_FILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <cstdint>

namespace Arinc665::File {

/**
 * @brief File Information stored within the file list of the ARINC 665 Files
 *   List File.
 *
 * @sa FileListFile
 **/
class FileInfo
{
  public:
    /**
     * @brief Initialises the file information
     *
     * @param[in] filename
     *   File filename
     * @param[in] pathName
     *   File path name
     * @param[in] memberSequenceNumber
     *   Member sequence number.
     * @param[in] crc
     *   File CRC
     * @param[in] checkValue
     *   Check Value.
     **/
    FileInfo(
      std::string_view filename,
      std::string_view pathName,
      uint16_t memberSequenceNumber,
      uint16_t crc,
      const std::optional< CheckValue> &checkValue = {});

    //! @copydoc FileInfo::FileInfo(std::string_view,std::string_view,uint16_t,uint16_t,const std::optional<CheckValue>&)
    FileInfo(
      std::string &&filename,
      std::string &&pathName,
      uint16_t memberSequenceNumber,
      uint16_t crc,
      std::optional< CheckValue> &&checkValue = {});

    /**
     * @brief Returns the filename.
     *
     * @return The filename
     **/
    [[nodiscard]] std::string_view filename() const;

    /**
     * @brief Updates the filename.
     *
     * @param[in] filename
     *   The filename.
     **/
    void filename( std::string_view filename);

    //! @copydoc filename(std::string_view)
    void filename( std::string &&filename);

    /**
     * @brief Returns the path name.
     *
     * @return The path name
     **/
    [[nodiscard]] std::string_view pathName() const;

    /**
     * @brief Updates the path name.
     *
     * @param[in] pathName
     *   The path name.
     **/
    void pathName( std::string_view pathName);

    //! @copydoc pathName(std::string_view)
    void pathName( std::string &&pathName);

    /**
     * @brief Returns the path (path name / filename)
     *
     * @return path (incl. filename)
     **/
    [[nodiscard]] std::filesystem::path path() const;

    /**
     * @brief Returns the member sequence number.
     *
     * @return The member sequence number
     **/
    [[nodiscard]] uint16_t memberSequenceNumber() const;

    /**
     * @brief Updates the  member sequence number.
     *
     * @param[in] memberSequenceNumber
     *   The  member sequence number.
     **/
    void memberSequenceNumber( uint16_t memberSequenceNumber);

    /**
     * @brief Returns the file CRC.
     *
     * @return The file CRC.
     **/
    [[nodiscard]] uint16_t crc() const;

    /**
     * @brief Updates the file CRC.
     *
     * @param[in] crc
     *   File CRC.
     **/
    void crc( uint16_t crc);

    /**
     * @brief Returns the Check Value.
     *
     * @return The Check Value.
     **/
    [[nodiscard]] const std::optional< CheckValue>& checkValue() const;

    /**
     * @brief Updates the Check Value
     *
     * @param[in] checkValue
     *   Check Value.
     **/
    void checkValue( const std::optional< CheckValue> &checkValue);

    //! @copydoc checkValue(const std::optional<CheckValue>&)
    void checkValue( std::optional< CheckValue> &&checkValue);

    /**
     * @brief Compares the given file info against [this].
     *
     * @param[in] other
     *  Other file info.
     *
     * @return if [this] is equal to [other].
     **/
    bool operator==( const FileInfo &other) const;

    /**
     * @brief Compares the given file info against [this].
     *
     * @param[in] other
     *  Other file info.
     *
     * @return if [this] is un-equal to [other].
     **/
    bool operator!=( const FileInfo &other) const;

  private:
    //! file name
    std::string filenameValue;
    //! path name
    std::string pathNameValue;
    //! member sequence number
    uint16_t memberSequenceNumberValue;
    //! CRC
    uint16_t crcValue;
    //! Check Value (since ARINC 665-3)
    std::optional< CheckValue> checkValueValue;
};

}

#endif

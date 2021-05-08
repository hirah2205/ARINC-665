/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::LoadInfo.
 **/

#ifndef ARINC665_LOADINFO_HPP
#define ARINC665_LOADINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Load Information.
 *
 * Load Information stored within the load list of the ARINC 665 Loads List
 * File.
 *
 * @sa LoadListFile
 **/
class LoadInfo
{
  public:
    //! Target Hardware IDs.
    using ThwIds = std::list< std::string>;

    /**
     * @brief Initialises the load information
     *
     * @param[in] partNumber
     *   Load part number.
     * @param[in] headerFilename
     *   Load header filename
     * @param[in] memberSequenceNumber
     *   Member sequence number.
     * @param[in] targetHardwareIds
     *   Target Hardware IDs
     **/
    LoadInfo(
      std::string_view partNumber,
      std::string_view headerFilename,
      uint8_t memberSequenceNumber,
      const ThwIds &targetHardwareIds );

    //! @copydoc LoadInfo::LoadInfo(std::string_view,std::string_view,uint8_t,const ThwIds&)
    LoadInfo(
      std::string &&partNumber,
      std::string &&headerFilename,
      uint8_t memberSequenceNumber,
      ThwIds &&targetHardwareIds );

    /**
     * @brief Returns the load part number.
     *
     * @return The load part number
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the load part number.
     *
     * @param[in] partNumber
     *   load part number.
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber );

    /**
     * @brief Returns the load header filename.
     *
     * @return The load header filename.
     **/
    [[nodiscard]] std::string_view headerFilename() const;

    /**
     * @brief Updates the load header filename.
     *
     * @param[in] headerFilename
     *   The load header filename.
     **/
    void headerFilename( std::string_view headerFilename );

    //! @copydoc headerFilename(std::string_view)
    void headerFilename( std::string &&headerFilename );

    /**
     * @brief Returns the member sequence number.
     *
     * @return The member sequence number
     **/
    [[nodiscard]] uint8_t memberSequenceNumber() const;

    /**
     * @brief Updates the  member sequence number.
     *
     * @param[in] memberSequenceNumber
     *   The  member sequence number.
     **/
    void memberSequenceNumber( uint8_t memberSequenceNumber );

    /**
     * @brief Returns the target hardware IDs.
     *
     * @return The target hardware IDs.
     **/
    [[nodiscard]] const ThwIds& targetHardwareIds() const;

    //! @copydoc targetHardwareIds() const
    [[nodiscard]] ThwIds& targetHardwareIds();

    /**
     * @brief Adds a target hardware ID.
     *
     * @param[in] targetHardwareId
     *   target hardware ID.
     **/
    void targetHardwareId( std::string_view targetHardwareId );

    //! @copydoc targetHardwareId(std::string_view)
    void targetHardwareId( std::string &&targetHardwareId );

    /**
     * @brief Compares the other load information against this for equality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if [this] and [other] are equal.
     **/
    bool operator ==( const LoadInfo &other ) const;

    /**
     * @brief Compares the other load information against this for inequality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if [this] and [other] are unequal.
     **/
    bool operator !=( const LoadInfo &other ) const;

    /**
     * @brief Compares the other file information against this for equality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if [this] and [other] are equal.
     **/
    bool operator ==( const FileInfo &other ) const;

    /**
     * @brief Compares the other file information against this for inequality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if [this] and [other] are unequal.
     **/
    bool operator !=( const FileInfo &other ) const;

  private:
    //! Load part number
    std::string partNumberValue;
    //! Header filename
    std::string headerFilenameValue;
    //! Member sequence number
    uint8_t memberSequenceNumberValue;
    //! Target Hardware IDs (list)
    ThwIds targetHardwareIdsValue;
};

/**
 * @brief Compares FileInfo against LoadInfo
 *
 * @param[in] fileInfo
 *   The file information.
 * @param[in] loadInfo
 *   The load information.
 *
 * @return If file information is equal to the load information.
 **/
bool operator ==( const FileInfo &fileInfo, const LoadInfo &loadInfo );

/**
 * @brief Compares FileInfo against LoadInfo
 *
 * @param[in] fileInfo
 *   The file information.
 * @param[in] loadInfo
 *   The load information.
 *
 * @return If file information is not equal to the load information.
 **/
bool operator !=( const FileInfo &fileInfo, const LoadInfo &loadInfo );

}

#endif

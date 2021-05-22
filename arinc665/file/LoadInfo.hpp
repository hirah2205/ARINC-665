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
     *   Load Part Number.
     * @param[in] headerFilename
     *   Load Header Filename
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
     * @brief Returns the Load Part Number.
     *
     * @return Load Part Number
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Load Part Number.
     *
     * @param[in] partNumber
     *   Load Part Number.
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber );

    /**
     * @brief Returns the Load Header Filename.
     *
     * @return Load Header Filename.
     **/
    [[nodiscard]] std::string_view headerFilename() const;

    /**
     * @brief Updates the Load Header Filename.
     *
     * @param[in] headerFilename
     *   Load Header Filename.
     **/
    void headerFilename( std::string_view headerFilename );

    //! @copydoc headerFilename(std::string_view)
    void headerFilename( std::string &&headerFilename );

    /**
     * @brief Returns the Member Sequence Number.
     *
     * @return Member Sequence Number
     **/
    [[nodiscard]] uint8_t memberSequenceNumber() const;

    /**
     * @brief Updates the Member Sequence Number.
     *
     * @param[in] memberSequenceNumber
     *   Member Sequence Number.
     **/
    void memberSequenceNumber( uint8_t memberSequenceNumber );

    /**
     * @brief Returns the Target Hardware IDs.
     *
     * @return Target Hardware IDs.
     **/
    [[nodiscard]] const ThwIds& targetHardwareIds() const;

    //! @copydoc targetHardwareIds() const
    [[nodiscard]] ThwIds& targetHardwareIds();

    /**
     * @brief Adds a Target Hardware ID.
     *
     * @param[in] targetHardwareId
     *   target hardware ID.
     **/
    void targetHardwareId( std::string_view targetHardwareId );

    //! @copydoc targetHardwareId(std::string_view)
    void targetHardwareId( std::string &&targetHardwareId );

    /**
     * @brief Compares the other load information against @p this for equality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if @p this and @p other are equal.
     **/
    bool operator ==( const LoadInfo &other ) const;

    /**
     * @brief Compares the other load information against @p this for inequality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if @p this and @p other are unequal.
     **/
    bool operator !=( const LoadInfo &other ) const;

    /**
     * @brief Compares the other file information against @p this for equality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if @p this and @p other are equal.
     **/
    bool operator ==( const FileInfo &other ) const;

    /**
     * @brief Compares the other file information against @p this for inequality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if @p this and @p other are unequal.
     **/
    bool operator !=( const FileInfo &other ) const;

  private:
    //! Load Part Number
    std::string partNumberValue;
    //! Header Filename
    std::string headerFilenameValue;
    //! Member Sequence Number
    uint8_t memberSequenceNumberValue;
    //! Target Hardware IDs (list)
    ThwIds targetHardwareIdsValue;
};

/**
 * @brief Compares @p FileInfo against @p LoadInfo
 *
 * @param[in] fileInfo
 *   File Information.
 * @param[in] loadInfo
 *   Load Information.
 *
 * @return If file information is equal to the load information.
 **/
bool operator ==( const FileInfo &fileInfo, const LoadInfo &loadInfo );

/**
 * @brief Compares @p FileInfo against @p LoadInfo
 *
 * @param[in] fileInfo
 *   File Information.
 * @param[in] loadInfo
 *   Load Information.
 *
 * @return If file information is not equal to the load information.
 **/
bool operator !=( const FileInfo &fileInfo, const LoadInfo &loadInfo );

}

#endif

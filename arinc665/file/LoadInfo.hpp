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
 * @brief Declaration of class Arinc665::File::LoadInfo.
 **/

#ifndef ARINC665_LOADINFO_HPP
#define ARINC665_LOADINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <list>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Load information, which is part of a load list file.
 *
 * @sa LoadListFile
 **/
class LoadInfo
{
  public:
    //! String type
    using string = std::string;
    //! Target Hardware IDs type.
    using ThwIds = std::list< string>;

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
      const string &partNumber,
      const string &headerFilename,
      uint8_t memberSequenceNumber,
      const ThwIds &targetHardwareIds);

    //! @copydoc LoadInfo::LoadInfo(const string&,const string&,uint8_t,const ThwIds&)
    LoadInfo(
      string &&partNumber,
      string &&headerFilename,
      uint8_t memberSequenceNumber,
      ThwIds &&targetHardwareIds);

    /**
     * @brief Returns the load part number.
     *
     * @return The load part number
     **/
    string getPartNumber() const;

    void setPartNumber( const string &partNumber);

    /**
     * @brief Returns the load header filename.
     *
     * @return The load header filename.
     **/
    string getHeaderFilename() const;

    void setHeaderFilename( const string &headerFilename);

    uint8_t getMemberSequenceNumber() const;

    void setMemberSequenceNumber( uint8_t memberSequenceNumber);

    const ThwIds& getTargetHardwareIds() const;

    ThwIds& getTargetHardwareIds();

    void addTargetHardwareId( const string &targetHardwareId);

    void addTargetHardwareId( string &&targetHardwareId);

    /**
     * @brief Compares the other load information against this for equality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if [this] and [other] are equal.
     **/
    bool operator ==( const LoadInfo &other) const;

    /**
     * @brief Compares the other load information against this for inequality.
     *
     * @param[in] other
     *   Other load information
     *
     * @return if [this] and [other] are unequal.
     **/
    bool operator !=( const LoadInfo &other) const;

    /**
     * @brief Compares the other file information against this for equality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if [this] and [other] are equal.
     **/
    bool operator ==( const FileInfo &other) const;

    /**
     * @brief Compares the other file information against this for inequality.
     *
     * @param[in] other
     *   Other file information
     *
     * @return if [this] and [other] are unequal.
     **/
    bool operator !=( const FileInfo &other) const;

  private:
    //! Load part number
    string partNumber;
    //! Header filename
    string headerFilename;
    //! Member sequence number
    uint8_t memberSequenceNumber;
    //! Target Hardware IDs (list)
    ThwIds targetHardwareIds;
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
bool operator ==( const FileInfo &fileInfo, const LoadInfo &loadInfo);

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
bool operator !=( const FileInfo &fileInfo, const LoadInfo &loadInfo);

}
}

#endif

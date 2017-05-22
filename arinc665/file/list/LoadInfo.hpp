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
 * @brief load information, which is part of a load list file.
 **/
class LoadInfo
{
  public:
    using string = std::string;
    using ThwIds = std::list< string>;

    static LoadInfos getLoadInfos( RawFile::const_iterator &it);

    LoadInfo();

    LoadInfo( RawFile::const_iterator &it);

    string getPartNumber() const;

    void setPartNumber( const string &partNumber);

    string getHeaderFilename() const;

    void setHeaderFilename( const string &headerFilename);

    uint8_t getMemberSequenceNumber() const;

    void setMemberSequenceNumber( uint8_t memberSequenceNumber);

    const ThwIds& getTargetHardwareIdList() const;

    ThwIds& getTargetHardwareIdList();

    bool operator ==( const LoadInfo &other) const;

    bool operator !=( const LoadInfo &other) const;

    bool operator ==( const FileInfo &other) const;

    bool operator !=( const FileInfo &other) const;

  private:
    string partNumber;
    string headerFilename;
    uint8_t memberSequenceNumber;
    ThwIds targetHardwareIds;
};

/**
 * @brief Compares FileInfo against LoadInfo
 *
 * @param fileInfo
 * @param loadInfo
 *
 * @return
 **/
bool operator ==( const FileInfo &fileInfo, const LoadInfo &loadInfo);

/**
 * @brief Compares FileInfo against LoadInfo
 *
 * @param fileInfo
 * @param loadInfo
 *
 * @return
 **/
bool operator !=( const FileInfo &fileInfo, const LoadInfo &loadInfo);

}
}

#endif

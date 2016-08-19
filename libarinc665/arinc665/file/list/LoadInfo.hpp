/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @brief Declaration of class LoadInfo
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

    static std::list< LoadInfo> getLoadList(
      std::vector< uint8_t>::const_iterator &it);

    LoadInfo( void);

    LoadInfo( std::vector< uint8_t>::const_iterator &it);

    string getPartNumber( void) const;

    void setPartNumber( const string &partNumber);

    string getHeaderFilename( void) const;

    void setHeaderFilename( const string &headerFilename);

    uint16_t getMemberSequenceNumber( void) const;

    void setMemberSequenceNumber( const uint16_t memberSequenceNumber);

    const std::list< string>& getTargetHardwareIdList( void) const;

    std::list< string>& getTargetHardwareIdList( void);

  private:
    string partNumber;
    string headerFilename;
    uint16_t memberSequenceNumber;
    std::list< string> targetHardwareIds;
};
}
}

#endif

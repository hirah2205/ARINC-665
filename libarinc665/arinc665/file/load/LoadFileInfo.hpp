/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * $Date$
 * $Revision$
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class LoadFileInfo
 **/

#ifndef ARINC665_FILE_LOAD_LOADFILEINFO_HPP
#define ARINC665_FILE_LOAD_LOADFILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <vector>
#include <list>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief the File information within the file list of the load upload header.
 **/
class LoadFileInfo
{
  public:
    using string = std::string;

    //
    using LoadFileInfoList = std::list< LoadFileInfo>;

    static LoadFileInfoList getFileList(
      RawFile::const_iterator &it);

    LoadFileInfo( void);

    LoadFileInfo(
      const string &name,
      const string partNumber,
      const uint32_t length,
      const uint16_t crc);

    LoadFileInfo( RawFile::const_iterator &it);

    string getName( void) const;
    void setName( const string &name);

    string getPartNumber( void) const;
    void setPartNumber( const string &partNumber);

    uint32_t getLength( void) const;
    void setLength( uint32_t length);

    uint16_t getCrc( void) const;
    void setCrc( const uint16_t crc);

  private:
    //! Filename
    string name;
    //! File part number
    string partNumber;
    //! File length
    uint32_t length;
    //! File CRC
    uint16_t crc;
};

}
}

#endif

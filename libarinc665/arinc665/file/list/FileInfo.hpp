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
 * @brief Declaration of class Arinc665::File::FileInfo.
 **/

#ifndef ARINC665_FILEINFO_HPP
#define ARINC665_FILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief The file information, which is part of the FileListFile.
 **/
class FileInfo
{
  public:
    using string = std::string;
    using FileInfoList = std::vector< FileInfo>;
    using path = boost::filesystem::path;

    static FileInfoList getFileList(
      RawFile::const_iterator &it);

    FileInfo( void);

    FileInfo( RawFile::const_iterator &it);

    string getFilename( void) const;

    void setFilename( const string &filename);

    string getPathName( void) const;

    void setPathName( const string &pathName);

    path getPath( void) const;

    uint16_t getMemberSequenceNumber( void) const;

    void setMemberSequenceNumber( const uint16_t memberSequenceNumber);

    uint16_t getCrc( void) const;

    void setCrc( const uint16_t crc);

    bool operator ==( const FileInfo &other) const;

    bool operator !=( const FileInfo &other) const;

  private:
    string filename;
    string pathName;
    uint16_t memberSequenceNumber;
    uint16_t crc;
};
}
}

#endif

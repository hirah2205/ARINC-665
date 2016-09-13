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
 * @brief Declaration of class Arinc665::File::BatchInfo.
 **/

#ifndef ARINC665_FILE_BATCHINFO_HPP
#define ARINC665_FILE_BATCHINFO_HPP

#include <arinc665/file/File.hpp>

#include <vector>

namespace Arinc665 {
namespace File {

class BatchInfo
{
  public:
    using string = std::string;
    using BatchInfoList = std::vector< BatchInfo>;

    static BatchInfoList getBatchInfos(
      RawFile::const_iterator &it);

    BatchInfo( void);

    BatchInfo( RawFile::const_iterator &it);

    string getPartNumber( void) const;

    void setPartNumber( const string &partNumber);

    string getFilename( void) const;

    void setFilename( const string &filename);

    uint16_t getMemberSequenceNumber( void) const;

    void setMemberSequenceNumber( const uint16_t memberSequenceNumber);

    bool operator ==( const BatchInfo &other) const;

    bool operator !=( const BatchInfo &other) const;

    bool operator ==( const FileInfo &other) const;

    bool operator !=( const FileInfo &other) const;

  private:
    string partNumber;
    string filename;
    uint16_t memberSequenceNumber;
};

bool operator ==( const FileInfo &fileInfo, const BatchInfo &batchInfo);

bool operator !=( const FileInfo &fileInfo, const BatchInfo &batchInfo);

}
}

#endif

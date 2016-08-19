/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author
 *
 * @brief Declaration of class FileListFile
 **/

#ifndef ARINC665_FILELISTFILE_HPP
#define ARINC665_FILELISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/FileInfo.hpp>

#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Represents the parsed file list, which is contained on each media
 *   of the media set.
 **/
class FileListFile: public ListFile
{
  public:
    using ListType = std::list< FileInfo>;

    FileListFile( void);

    FileListFile( const RawFile &file);

    virtual Arinc665Version getArincVersion( void) const override;

    unsigned int getNumberOfFiles( void) const;

    const ListType& getFiles( void) const;

    ListType& getFiles( void);

    const std::vector< uint8_t>& getUserDefinedData( void) const;

    void setUserDefinedData( const std::vector< uint8_t> &userDefinedData);

  private:
    ListType fileList;
    std::vector< uint8_t> userDefinedData;
};
}
}

#endif

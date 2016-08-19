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
 * @brief Declaration of class FileFactory
 **/

#ifndef ARINC665_FILE_FILEFACTORY_HPP
#define ARINC665_FILE_FILEFACTORY_HPP

#include <arinc665/file/File.hpp>

#include <arinc665/file/list/FileListFile.hpp>
#include <arinc665/file/list/LoadListFile.hpp>
#include <arinc665/file/list/BatchListFile.hpp>
#include <arinc665/file/load/LoadHeaderFile.hpp>
#include <arinc665/file/batch/BatchFile.hpp>

#include <boost/filesystem.hpp>

namespace Arinc665 {
namespace File {

/**
 * @brief Factory of ARINC 665 special files.
 **/
class FileFactory
{
  public:
    /**
     * @brief
     * @param file
     * @return
     **/
    static FileClassType getArincFileType( const RawFile &file);

    static LoadFileFormatVersion getLoadFileFormatVersion( const RawFile &file);

    static BatchFileFormatVersion getBatchFileFormatVersion(
      const RawFile &file);

    static MediaFileFormatVersion getMediaFileFormatVersion(
      const RawFile &file);

    static FileType getFileType( const boost::filesystem::path &filename);

    static FileListFile getFileListFile( const RawFile &file);

    static LoadListFile getLoadListFile( const RawFile &file);

    static BatchListFile getBatchListFile( const RawFile &file);

    static LoadHeaderFile getLoadHeaderFile( const RawFile &file);

    static BatchFile getBatchFile( const RawFile &file);

  private:
    FileFactory( void);
};
}
}

#endif

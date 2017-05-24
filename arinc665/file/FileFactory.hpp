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
 * @brief Declaration of class Arinc665::File::FileFactory.
 **/

#ifndef ARINC665_FILE_FILEFACTORY_HPP
#define ARINC665_FILE_FILEFACTORY_HPP

#include <arinc665/file/File.hpp>

#include <boost/filesystem.hpp>

namespace Arinc665 {
namespace File {

/**
 * @brief Factory of ARINC 665 special files.
 **/
class FileFactory
{
  public:
    using path = boost::filesystem::path;

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

    static FileType getFileType( const path &filename);

  private:
    FileFactory() = delete;
};
}
}

#endif

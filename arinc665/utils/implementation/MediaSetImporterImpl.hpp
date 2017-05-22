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
 * @brief Declaration of class Arinc665::Utils::MediaSetImporterImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETIMPORTERIMPL_HPP
#define ARINC665_UTILS_MEDIASETIMPORTERIMPL_HPP

#include <arinc665/utils/Arinc665Utils.hpp>

#include <arinc665/file/list/FileListFile.hpp>
#include <arinc665/file/list/LoadListFile.hpp>
#include <arinc665/file/list/BatchListFile.hpp>
#include <arinc665/file/load/LoadHeaderFile.hpp>
#include <arinc665/file/batch/BatchFile.hpp>


#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <boost/optional.hpp>

#include <map>

namespace Arinc665 {
namespace Utils {

/**
 * @brief Implementation of a ARINC 665 media set importer.
 */
class MediaSetImporterImpl
{
  public:
    using path = Arinc665Utils::path;

    using string = std::string;

    /**
     * @brief Initialises the ARINC 665 media set importer
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     **/
    MediaSetImporterImpl(
      Arinc665Utils::ReadFileHandler readFileHandler);

    /**
     * @brief Entry-point of the ARINC 665 media set importer.
     *
     * @return
     **/
    Media::MediaSetPtr operator()( const string &mediaSetName);

  private:
    using LoadHeaderFiles = std::map< std::string, File::LoadHeaderFile>;
    using BatchFiles = std::map< std::string, File::BatchFile>;
    using ContainerEntityPtr = std::shared_ptr< Media::ContainerEntity>;

    /**
     * @brief Loads the information of the given medium
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void addMedium( uint8_t mediumIndex);

    void loadFileListFile( uint8_t mediumIndex);

    void loadLoadListFile( uint8_t mediumIndex);

    void loadBatchListFile( uint8_t mediumIndex);

    void loadLoadHeaderFiles( uint8_t mediumIndex);

    void loadBatchFiles( uint8_t mediumIndex);

    void addFiles();

    void addLoads( File::FileListFile::FileInfoMap &loadHeaders);

    void addBatches( File::FileListFile::FileInfoMap &batches);

    ContainerEntityPtr checkCreateDirectory(
      uint8_t mediumIndex,
      const path &filePath);

    Arinc665Utils::ReadFileHandler readFileHandler;

    //! The Media Set
    Media::MediaSetPtr mediaSet;

    boost::optional < File::FileListFile> fileListFile;
    boost::optional < File::LoadListFile> loadListFile;
    boost::optional < File::BatchListFile> batchListFile;
    LoadHeaderFiles loadHeaderFiles;
    BatchFiles batchFiles;

    File::FileListFile::FileInfoMap fileInfos;
    File::LoadListFile::LoadInfoMap loadInfos;
    File::BatchListFile::BatchInfoMap batchInfos;
};

}
}

#endif

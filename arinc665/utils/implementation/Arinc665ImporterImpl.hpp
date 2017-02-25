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
 * @brief Declaration of class Arinc665::Utils::Arinc665ImporterImpl.
 **/

#ifndef ARINC665_UTILS_ARINC665IMPORTERIMPL_HPP
#define ARINC665_UTILS_ARINC665IMPORTERIMPL_HPP

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

class Arinc665ImporterImpl
{
  public:
    using MediaSetPtr = Arinc665Utils::MediaSetPtr;
    using path = Arinc665Utils::path;

    Arinc665ImporterImpl( Arinc665Utils::GetMediumPathHandler getMediumPathHandler);

    MediaSetPtr operator ()();

  private:
    using RawFile = Arinc665::File::RawFile;
    using FileListFile = Arinc665::File::FileListFile;
    using LoadListFile = Arinc665::File::LoadListFile;
    using BatchListFile = Arinc665::File::BatchListFile;
    using LoadHeaderFile = Arinc665::File::LoadHeaderFile;
    using BatchFile = Arinc665::File::BatchFile;
    using LoadHeaderFiles = std::map< std::string, LoadHeaderFile>;
    using BatchFiles = std::map< std::string, BatchFile>;
    using ContainerEntityPtr = std::shared_ptr< Media::ContainerEntity>;

    void addMedium( unsigned int mediaIndex, const path &mediumPath);

    void loadFileListFile( unsigned int mediaIndex, const path &mediumPath);

    void loadLoadListFile( unsigned int mediaIndex, const path &mediumPath);

    void loadBatchListFile( unsigned int mediaIndex, const path &mediumPath);

    void loadLoadHeaderFiles( unsigned int mediaIndex, const path &mediumPath);

    void loadBatchFiles( unsigned int mediaIndex, const path &mediumPath);

    void addFiles();

    void addLoads( FileListFile::FileInfoMap &loadHeaders);

    void addBatches( FileListFile::FileInfoMap &batches);

    ContainerEntityPtr checkCreateDirectory( unsigned int mediaIndex, const path &filePath);

    /**
     * @brief loads the file.
     *
     * @param filePath
     *
     * @return
     **/
    RawFile loadFile( const path &filePath);

    Arinc665Utils::GetMediumPathHandler getMediumPathHandler;

    //! The Media Set
    MediaSetPtr mediaSet;

    boost::optional < FileListFile> fileListFile;
    boost::optional < LoadListFile> loadListFile;
    boost::optional < BatchListFile> batchListFile;
    LoadHeaderFiles loadHeaderFiles;
    BatchFiles batchFiles;

    FileListFile::FileInfoMap fileInfos;
    LoadListFile::LoadInfoMap loadInfos;
    BatchListFile::BatchInfoMap batchInfos;
};

}
}

#endif

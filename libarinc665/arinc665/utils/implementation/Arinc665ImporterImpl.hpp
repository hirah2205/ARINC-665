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

#include <arinc665/utils/Arinc665Importer.hpp>


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

class Arinc665ImporterImpl : public Arinc665Importer
{
  public:
    using RawFile = Arinc665::File::RawFile;
    using FileListFile = Arinc665::File::FileListFile;
    using LoadListFile = Arinc665::File::LoadListFile;
    using BatchListFile = Arinc665::File::BatchListFile;
    using LoadHeaderFile = Arinc665::File::LoadHeaderFile;
    using BatchFile = Arinc665::File::BatchFile;
    using ContainerEntityPtr = std::shared_ptr< Media::ContainerEntity>;

    void import( GetMediumHandler getMediumHandler);

    MediaSetPtr getMediaSet( void);

  protected:

    void addMedium( const unsigned int mediaIndex, const path &mediumPath);

    void loadFileListFile( const unsigned int mediaIndex, const path &mediumPath);

    void loadLoadListFile( const unsigned int mediaIndex, const path &mediumPath);

    void loadBatchListFile( const unsigned int mediaIndex, const path &mediumPath);

    void loadLoadHeaderFiles( const unsigned int mediaIndex, const path &mediumPath);

    void loadBatchFiles( const unsigned int mediaIndex, const path &mediumPath);

    void addFiles( void);

    ContainerEntityPtr checkCreateDirectory( const unsigned int mediaIndex, const path &mediumPath);

    /**
     * @brief loads the file.
     *
     * @param filePath
     *
     * @return
     **/
    RawFile loadFile( const path &filePath);

  private:
    //! The media set.
    MediaSetPtr mediaSet;
    boost::optional < FileListFile> fileListFile;
    boost::optional < LoadListFile> loadListFile;
    boost::optional < BatchListFile> batchListFile;
    std::map< std::string, LoadHeaderFile> loadHeaderFiles;
    std::map< std::string, BatchFile> batchFiles;

    FileListFile::FileMap files;
    LoadListFile::LoadMap loads;
    BatchListFile::BatchMap batches;
};

}
}

#endif

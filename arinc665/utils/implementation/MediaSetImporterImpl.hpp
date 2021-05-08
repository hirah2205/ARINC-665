/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Utils::MediaSetImporterImpl.
 **/

#ifndef ARINC665_UTILS_MEDIASETIMPORTERIMPL_HPP
#define ARINC665_UTILS_MEDIASETIMPORTERIMPL_HPP

#include <arinc665/utils/Arinc665Utils.hpp>

#include <arinc665/file/FileListFile.hpp>
#include <arinc665/file/LoadListFile.hpp>
#include <arinc665/file/BatchListFile.hpp>
#include <arinc665/file/LoadHeaderFile.hpp>
#include <arinc665/file/BatchFile.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <optional>
#include <map>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a ARINC 665 media set importer.
 */
class MediaSetImporterImpl
{
  public:
    /**
     * @brief Initialises the ARINC 665 media set importer
     *
     * @param[in] readFileHandler
     *   Handler which is called to obtain the requested file from the medium.
     **/
    explicit MediaSetImporterImpl(
      Arinc665Utils::ReadFileHandler readFileHandler );

    /**
     * @brief Entry-point of the ARINC 665 media set importer.
     *
     * @return The media set instance.
     **/
    Media::MediaSetPtr operator()();

  private:
    //! Load header files type (mapping filename -> load header file)
    using LoadHeaderFiles = std::map< std::string, File::LoadHeaderFile >;
    //! Batch files type (mapping filename -> batch file)
    using BatchFiles = std::map< std::string, File::BatchFile >;
    //! Container Entity type
    using ContainerEntityPtr = std::shared_ptr< Media::ContainerEntity >;

    /**
     * @brief Loads the information of the given medium
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void addMedium( uint8_t mediumIndex );

    /**
     * @brief Loads the file list file of the given medium.
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void loadFileListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the load list file of the given medium.
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void loadLoadListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the batch list file of the given medium.
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void loadBatchListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the load header files of the given medium.
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void loadLoadHeaderFiles( uint8_t mediumIndex );

    /**
     * @brief Loads the batch files of the given medium.
     *
     * @param[in] mediumIndex
     *   The medium index.
     **/
    void loadBatchFiles( uint8_t mediumIndex );

    /**
     * @brief Adds all files to the media set representation
     *
     * Iterates over the file information and creates the directories and files.
     *
     * @sa addLoads
     * @sa addBatches
     * @sa checkCreateDirectory
     **/
    void addFiles();

    /**
     * @brief Adds the loads to the media set.
     *
     * @param[in] loadHeaders
     *   The load header files
     **/
    void addLoads( File::FileListFile::FileInfoMap &loadHeaders );

    /**
     * @brief Adds the batches to the media set.
     *
     * @param[in] batches
     *   The batch files
     **/
    void addBatches( File::FileListFile::FileInfoMap &batches );

    /**
     * @brief Creates the logical directory entry if not already created and
     *   return its representation.
     *
     * @param[in] mediumIndex
     *   Medium index.
     * @param[in] directoryPath
     *   The directory path.
     *
     * @return The directory entry.
     **/
    ContainerEntityPtr checkCreateDirectory(
      uint8_t mediumIndex,
      const std::filesystem::path &directoryPath );

    //! Read File Handler
    Arinc665Utils::ReadFileHandler readFileHandler;

    //! Media Set
    Media::MediaSetPtr mediaSet;

    //! File List File
    std::optional< File::FileListFile > fileListFile;
    //! Load List File
    std::optional< File::LoadListFile > loadListFile;
    //! Batch List File
    std::optional< File::BatchListFile > batchListFile;
    //! Load Header Files
    LoadHeaderFiles loadHeaderFiles;
    //! Batch Files
    BatchFiles batchFiles;

    //! File Information from List of Files
    File::FileListFile::FileInfoMap fileInfos;
    //! Load Information from List of Loads
    File::LoadListFile::LoadsInfoMap loadInfos;
    //! Batch Information from List of Batches
    File::BatchListFile::BatchInfoMap batchInfos;
};

}

#endif

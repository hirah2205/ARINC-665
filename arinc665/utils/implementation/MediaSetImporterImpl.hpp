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

#include <arinc665/utils/MediaSetImporter.hpp>

#include <arinc665/files/FileListFile.hpp>
#include <arinc665/files/LoadListFile.hpp>
#include <arinc665/files/BatchListFile.hpp>
#include <arinc665/files/LoadHeaderFile.hpp>
#include <arinc665/files/BatchFile.hpp>

#include <arinc665/media/Media.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <optional>
#include <map>
#include <set>

namespace Arinc665::Utils {

/**
 * @brief Implementation of a ARINC 665 Media Set Importer.
 */
class MediaSetImporterImpl final : public MediaSetImporter
{
  public:
    /**
     * @brief Initialises the ARINC 665 Media Set Importer
     **/
    explicit MediaSetImporterImpl();

    //! @copydoc MediaSetImporter::readFileHandler()
    MediaSetImporter& readFileHandler( ReadFileHandler readFileHandler ) final;

    //! @copydoc MediaSetImporter::checkFileIntegrity()
    MediaSetImporter& checkFileIntegrity( bool checkFileIntegrity ) final;

    /**
     * @brief Entry-point of the ARINC 665 Media Set Importer.
     *
     * @return Created Media Set Instance.
     **/
    Media::MediaSetPtr operator()();

  private:
    //! Container Entity Type
    using ContainerEntityPtr = std::shared_ptr< Media::ContainerEntity >;

    /**
     * @brief Loads the Information of the Given Medium.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     *
     * @return If last medium has been read
     **/
    bool loadMedium( uint8_t mediumIndex );

    /**
     * @brief Loads the file list file of the given medium.
     *
     * Also checks the files on the current medium.
     * Checks File CRCs.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     *
     * @throw Arinc665Exception
     *   FILES.LUM are inconsistent between media.
     * @throw Arinc665Exception
     *   File CRCs does not match
     **/
    void loadFileListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the load list file of the given medium.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     *
     * @throw Arinc665Exception
     *   Load header file is not found on the media set.
     * @throw Arinc665Exception
     *   LOADS.LUM files are inconsistent between media.
     **/
    void loadLoadListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the batch list file of the given medium.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     *
     * @throw Arinc665Exception
     *   When BATCHES.LUM files are inconsistent between media.
     **/
    void loadBatchListFile( uint8_t mediumIndex );

    /**
     * @brief Loads the load header files of the given medium.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     *
     * @throw Arinc665Exception
     *   When is inconsistent.
     **/
    void loadLoadHeaderFiles( uint8_t mediumIndex );

    /**
     * @brief Loads the batch files of the given medium.
     *
     * @param[in] mediumIndex
     *   Medium Index.
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
     * @throw Arinc665Exception When files are not existent
     **/
    void addLoads();

    /**
     * @brief Adds the batches to the media set.
     *
     * @throw Arinc665Exception When loads are not existent
     **/
    void addBatches();

    /**
     * @brief Creates the logical directory entry if not already created and
     *   return its representation.
     *
     * @param[in] mediumIndex
     *   Medium Index.
     * @param[in] directoryPath
     *   Directory Path.
     *
     * @return Directory entry.
     *
     * @throw Arinc665Exception When sub-directory cannot be created
     **/
    ContainerEntityPtr checkCreateDirectory(
      uint8_t mediumIndex,
      const std::filesystem::path &directoryPath );

    //! Read File Handler
    ReadFileHandler readFileHandlerV;
    //! Check File Integrity
    bool checkFileIntegrityV;

    //! Media Set
    Media::MediaSetPtr mediaSet;

    //! File List File
    std::optional< Files::FileListFile > fileListFile;
    //! Load List File
    std::optional< Files::LoadListFile > loadListFile;
    //! Batch List File
    std::optional< Files::BatchListFile > batchListFile;

    //! Load Header Files (file name -> load header file)
    std::map< std::string, Files::LoadHeaderFile, std::less<> > loadHeaderFiles;
    //! Batch Files (file name -> batch file)
    std::map< std::string, Files::BatchFile, std::less<> > batchFiles;

    //! File Information from List of Files ( file name -> file info )
    std::map< std::string, Files::FileInfo, std::less<> > fileInfos;
    //! File Sizes ( file name -> file size )
    std::map< std::string, std::size_t, std::less<> > fileSizes;
    //! Loads (file names)
    std::set< std::string, std::less<> > loads;
    //! Batches (file names)
    std::set< std::string, std::less<> > batches;
};

}

#endif

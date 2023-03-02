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

#include <map>

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
    explicit MediaSetImporterImpl() = default;

    //! @copydoc MediaSetImporter::fileSizeHandler()
    MediaSetImporter& fileSizeHandler(
      FileSizeHandler fileSizeHandler ) override;

    //! @copydoc MediaSetImporter::readFileHandler()
    MediaSetImporter& readFileHandler(
      ReadFileHandler readFileHandler ) override;

    //! @copydoc MediaSetImporter::checkFileIntegrity()
    MediaSetImporter& checkFileIntegrity(
      bool checkFileIntegrity ) noexcept override;

    /**
     * @brief Entry-point of the ARINC 665 Media Set Importer.
     *
     * @return Created Media Set Instance.
     **/
    [[nodiscard]] Result operator()() override;

  private:
    //! Files Information (From File List File) (filename -> file information)
    using FilesInformation =
      std::multimap< std::string, Files::FileInfo, std::less<> >;
    //! Loads Information from List of Loads (filenames -> Load Information)
    using LoadsInformation =
      std::map< std::string, Files::LoadInfo, std::less<> >;
    //! Batches Information from List of Batches (filename -> Batch Information)
    using BatchesInformation =
      std::map< std::string, Files::BatchInfo, std::less<> >;

    /**
     * @brief Loads the first Medium of the Media Set.
     *
     * - Loads the List of Files File and checks it for integrity.
     * - Loads the List of Loads File and checks it for integrity.
     * - If present, loads the List of Batches and checks it for integrity.
     *
     * @throw Arinc665Exception
     *   When Medium Integrity is not given.
     **/
    void loadFirstMedium();

    /**
     * @brief Loads and Checks all further Media.
     *
     * for each further Media:
     * - Loads the List of Files File and checks it for integrity.
     * - Loads the List of Loads File and checks it for integrity.
     * - If present, loads the List of Batches and checks it for integrity.
     **/
    void loadFurtherMedia() const;

    /**
     * @brief Adds all files to the media set representation
     *
     * Iterates over the file information and creates the directories and files.
     *
     * @sa regularFile
     * @sa loadFile
     * @sa batchFile
     * @sa addLoad
     * @sa addBatch
     * @sa checkCreateDirectory
     **/
    void files();

    /**
     * @brief Add Regular File
     *
     * @param[in,out] parent
     *   Parent Container
     * @param[in] fileInfo
     *   File information
     **/
    void regularFile(
      Media::ContainerEntity &parent,
      const Files::FileInfo &fileInfo );

    /**
     * @brief Add Load Header File
     *
     * @param[in,out] parent
     *   Parent Container
     * @param[in] fileInfo
     *   File information
     * @param[in] loadInfo
     *   Load Information
     **/
    void loadFile(
      Media::ContainerEntity &parent,
      const Files::FileInfo &fileInfo,
      const Files::LoadInfo &loadInfo );

    /**
     * @brief Add Batch File
     *
     * @param[in,out] parent
     *   Parent Container
     * @param[in] fileInfo
     *   File information
     * @param[in] batchInfo
     *   Batch Information
     **/
    void batchFile(
      Media::ContainerEntity &parent,
      const Files::FileInfo &fileInfo,
      const Files::BatchInfo &batchInfo );

    /**
     * @brief Adds Load information to Load.
     *
     * The load information can only be updated when all files are loaded and
     * known to the media set.
     *
     * @param[in,out] load
     *   Load
     * @param[in] fileInfo
     *   File information
     * @param[in] loadInfo
     *   Load Information.
     **/
    void addLoad(
      Media::Load &load,
      const Files::FileInfo &fileInfo,
      const Files::LoadInfo &loadInfo );

    /**
     * @brief Returns a load file (data or support file) according to ARINC 665
     *   rules.
     *
     * @param[in] parent
     *   Parent Container of Load
     * @param[in] filename
     *   Filename of file to search for
     * @param[in] crc
     *   CRC-16 of File
     *
     * @return Found file
     * @throw Arinc665Exception
     *   If no file can be found.
     **/
    Media::RegularFilePtr loadFile(
      Media::ContainerEntity &parent,
      std::string_view filename,
      uint16_t crc ) const;

    /**
     * @brief Add the batch information to the Batch.
     *
     * The batch information can only be updated when all files are loaded and
     * known to the media set.
     *
     * @param[in,out] batch
     *   Batch
     * @param[in] fileInfo
     *   File information
     * @param[in] batchInfo
     *   Batch Information
     **/
    void addBatch(
      Media::Batch &batch,
      const Files::FileInfo &fileInfo,
      const Files::BatchInfo &batchInfo );

    /**
     * @brief Creates the logical directory entry if not already created and
     *   return its representation.
     *
     * @param[in] directoryPath
     *   Directory Path.
     *
     * @return Directory entry.
     *
     * @throw Arinc665Exception
     *   When subdirectory cannot be created
     **/
    [[nodiscard]] Media::ContainerEntityPtr checkCreateDirectory(
      const std::filesystem::path &directoryPath );

    /**
     * @brief Checks all Files on given Medium.
     *
     * Check for file existence and stores file size for later usage.
     * If file integrity checks is requested read each file and check file CRC
     * and Check Value.
     *
     * @param[in] mediumNumber
     *   Current Medium Number
     **/
    void checkMediumFiles( const MediumNumber &mediumNumber ) const;

    /**
     * @brief Check File Integrity
     *
     * Calculates and compares File CRC and File Check Value against stored
     * ones.
     *
     * @param[in] fileInfo
     *   File Information.
     *
     * @throw Arinc665Exception
     *   When File CRC does not match.
     * @throw Arinc665Exception
     *   When File Check Value does not match.
     **/
    void checkFileIntegrity( const Files::FileInfo &fileInfo ) const;

    /**
     * @brief Preform Checks of Load Files (data and support).
     *
     * Calculates Load CRC and Load Check Value.
     * Verifies File Length
     * Verifies File CRC and File Check Value
     *
     * @param[in,out] loadCrc
     *   Load CRC
     * @param[in,out] loadCheckValueGenerator
     *   Load Check Value Generator
     * @param[in] fileInfo
     *   File Information
     * @param[in] loadFileInfo
     *   Load File Information
     * @param[in] fileSize16Bit
     *   If Data Size is stored in multiple of 16bit.
     *   This is true for data files in ARINC 665-2 Load Header Files.
     *
     * @throw Arinc665Exception
     *   If File Integrity checks failed.
     **/
    void checkLoadFile(
      Arinc645::Arinc645Crc32 &loadCrc,
      Arinc645::CheckValueGenerator &loadCheckValueGenerator,
      const Files::FileInfo &fileInfo,
      const Files::LoadFileInfo &loadFileInfo,
      bool fileSize16Bit ) const;

    /**
     * @brief Compares the Check Values.
     *
     * @param[in] fileListCheckValue
     *   Check Value from File List
     * @param[in] loadFileCheckValue
     *   Check Value from Load File (Data or Support File)
     *
     * @return If Checksum Comparison was successful.
     * @retval true
     *   Check Value of File List and Load File Information from same type and
     *   check value are equal or no check value provided for load file.
     * @retval false
     *   Check Value could not be compared (different check value types).
     *   Check Value should be tested separately.
     *
     * @throw Arinc665Exception
     *   When Load File Check Values are inconsistent.
     **/
    [[nodiscard]] bool checkCheckValues(
      const Arinc645::CheckValue &fileListCheckValue,
      const Arinc645::CheckValue &loadFileCheckValue ) const;

    //! File Size Handler
    FileSizeHandler fileSizeHandlerV;
    //! Read File Handler
    ReadFileHandler readFileHandlerV;
    //! Check File Integrity
    bool checkFileIntegrityV{ true };

    //! Media Set
    Media::MediaSetPtr mediaSetV;
    //! Check Values
    Media::CheckValues checkValuesV;

    //! File List File (Load by loadFirstMedium(), used by loadFurtherMedia() )
    Files::FileListFile fileListFileV;
    //! Load List File (Load by loadFirstMedium(), used by loadFurtherMedia() )
    Files::LoadListFile loadListFileV;
    //! Batch List File (Load by loadFirstMedium(), used by loadFurtherMedia() )
    Files::BatchListFile batchListFileV;
    //! Batch List File present indicator
    bool batchListFilePresentV{ false };

    //! File Information from List of File
    FilesInformation filesInfosV;
    //! Load Information from List of Loads
    LoadsInformation loadsInfosV;
    //! Batch Information from List of Batches
    BatchesInformation batchesInfosV;

    //! Regular Files
    std::map< Media::RegularFilePtr, Files::FileInfo, std::less< > > regularFilesV;
    //! Loads
    std::map< Media::LoadPtr, std::pair< Files::FileInfo, Files::LoadInfo >, std::less< > > loadsV;
    //! Batches
    std::map< Media::BatchPtr, std::pair< Files::FileInfo, Files::BatchInfo >, std::less< > > batchesV;
};

}

#endif

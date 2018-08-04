/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::FileListFile.
 **/

#ifndef ARINC665_FILE_FILELISTFILE_HPP
#define ARINC665_FILE_FILELISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/FileInfo.hpp>

#include <vector>
#include <map>
#include <cstdint>

namespace Arinc665::File {

/**
 * @brief ARINC 665 File List File.
 **/
class FileListFile: public ListFile
{
  public:
    //! Maps media <sequence number, file name> to File Info
    using FileInfoMap = std::map< std::pair< uint8_t, std::string>, FileInfo>;
    //! Maps media <sequence number, file path> to File Info
    using FileInfoPathMap =
      std::map< std::pair< uint8_t, std::filesystem::path>, FileInfo>;
    //! User defined data type
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Media Set Part Number Pointer Field
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffset = 8U;

    //! Offset of the Media Set Files Pointer Field
    static constexpr std::size_t MediaSetFilesPointerFieldOffset = 12U;

    //! Offset of the User Defined Data Pointer Field
    static constexpr std::size_t UserDefinedDataPointerFieldOffset = 16U;
;
    //! Offset of the Load Check Value Pointer Field (Only ARINC 665-3/4)
    static constexpr std::size_t LoadCheckValuePointerFieldOffset = 20U;

    //! First Start of pointer data for ARINC 665-2 Load Headers.
    static constexpr std::size_t FileHeaderSizeV2 = 20U;

    //! First Start of pointer data for ARINC 665-3/4 Load Headers.
    static constexpr std::size_t FileHeaderSizeV3 = 24U;

    /**
     * @brief Creates an empty file list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    FileListFile( Arinc665Version version);

    /**
     * @brief Creates file list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] mediaSetPn
     *   The Media Set Part Number.
     * @param[in] mediaSequenceNumber
     *   The Media Sequence Number [1..255].
     * @param numberOfMediaSetMembers
     *   The Number of Media Set Members [1..255] & mediaSequenceNumber <=
     *     [numberOfMediaSetMembers]
     * @param[in] files
     *   The files informations.
     * @param[in] userDefinedData
     *   Additional User Defined Data.
     **/
    FileListFile(
      Arinc665Version version,
      const std::string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const FilesInfo &files,
      const UserDefinedData &userDefinedData);

    //! @copydoc FileListFile(Arinc665Version,const std::string&,uint8_t,uint8_t,const FilesInfo&,const UserDefinedData&)
    FileListFile(
      Arinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      FilesInfo &&files = {},
      UserDefinedData &&userDefinedData = {});

    /**
     * @brief Creates a file list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    FileListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=
    FileListFile& operator=( const RawFile &rawFile) final;

    //! @copydoc ListFile::mediaSetPn
    std::string mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( const std::string &mediaSetPn) final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( std::string &&mediaSetPn) final;

    //! @copydoc ListFile::mediaSequenceNumber
    uint8_t mediaSequenceNumber() const final;

    //! @copydoc ListFile::mediaSequenceNumber
    void mediaSequenceNumber( uint8_t mediaSequenceNumber) final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    uint8_t numberOfMediaSetMembers() const final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) final;

    /**
     * @brief Returns the number of files.
     *
     * @return The number of files.
     **/
    size_t numberOfFiles() const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    const FilesInfo& files() const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    FilesInfo& files();

    /**
     * @brief Return the list of files as map ( media index and filename as key).
     *
     * @return The list of files
     **/
    FileInfoMap filesAsMap() const;

    /**
     * @brief Return the list of files as map ( media index and complete path as key).
     *
     * @return The list of files
     **/
    FileInfoPathMap filesAsPathMap() const;

    /**
     * @brief Adds the given file information.
     *
     * @param[in] file
     *   The file information.
     **/
    void file( const FileInfo &file);

    //! @copydoc fileInfo(const FileInfo&)
    void file( FileInfo &&file);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

    /**
     * @brief Returns if the given file list file belongs to the same media set.
     *
     * @param[in] other
     *   The other file list file to compare to this.
     *
     * @return If the given file list file belongs to the same media set.
     **/
    bool belongsToSameMediaSet( const FileListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    RawFile encode() const final;

    /**
     * @brief Decodes the body of the file list file.
     *
     * @param[in] rawFile
     *   Raw file list file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the files information list.
     *
     * @return Raw representation of files information list.
     **/
    RawFile encodeFilesInfo() const;

    /**
     * @brief Decodes the files information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw files list file representation.
     * @param[in] offset
     *   Offset of the files information list.
     **/
    void decodeFilesInfo( const RawFile &rawFile, std::size_t offset);

    //! The media set part number
    std::string mediaSetPnValue;
    //! The media sequence number
    uint8_t mediaSequenceNumberValue;
    //! The number of media set members.
    uint8_t numberOfMediaSetMembersValue;
    //! Files information (list)
    FilesInfo filesValue;
    //! Use defined data.
    UserDefinedData userDefinedDataValue;
};

}

#endif

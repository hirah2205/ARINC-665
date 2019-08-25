/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::FileListFile.
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
 * @brief ARINC 665 File List File (FILES.LUM).
 *
 * @par File Format (ARINC 665-4)
 * Name of Field                                | Field Size (bits)
 * ---------------------------------------------|:----------------:
 * FILES.LUM File Length                        | 32
 * Media File Format Version                    | 16
 * Spare                                        | 16
 * Pointer to Media Set PN Length               | 32
 * Pointer to Number of Media Set Files         | 32
 * Pointer To User Defined Data                 | 32
 * Pointer to FILES.LUM File Check Value Length | 32
 * Expansion Point No. 1                        |  0
 * Media Set PN Length                          | 16
 * Media Set PN                                 | 16
 * Media Sequence Number (X)                    |  8
 * No. Of Media Set Members (Y)                 |  8
 * Number of Media Set Files                    | 16
 * # File Pointer                               | 16
 * # File Name Length                           | 16
 * # File Name                                  | 16
 * # File Pathname Length                       | 16
 * # File Pathname                              | 16
 * # File Member Sequence No.                   | 16
 * # File CRC                                   | 16
 * # File Check Value Length                    | 16
 * # File Check Value Type                      | 16
 * # File Check Value                           | 16
 * # Expansion Point No. 2                      |  0
 * Expansion Point No. 3                        |  0
 * User Defined Data                            | 16
 * FILES.LUM File Check Value Length            | 16
 * FILES.LUM File Check Value Type              | 16
 * FILES.LUM File Check Value                   | 16
 * FILES.LUM File CRC                           | 16
 **/
class FileListFile: public ListFile
{
  public:
    //! Maps media <sequence number, file name> to File Info
    using FileInfoMap = std::map< std::pair< uint8_t, std::string>, FileInfo>;
    //! Maps media <sequence number, file path> to File Info
    using FileInfoPathMap =
      std::map< std::pair< uint8_t, std::filesystem::path>, FileInfo>;
    //! User Defined Data Type
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Spare field.
    static constexpr std::size_t SpareFieldOffset = 6U;

    //! Offset of the Media Set Part Number Pointer Field
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffset = 8U;

    //! Offset of the Media Set Files Pointer Field
    static constexpr std::size_t MediaSetFilesPointerFieldOffset = 12U;

    //! Offset of the User Defined Data Pointer Field
    static constexpr std::size_t UserDefinedDataPointerFieldOffset = 16U;

    //! Offset of the File Check Value Pointer Field (Only ARINC 665-3/4)
    static constexpr std::size_t FileCheckValuePointerFieldOffset = 20U;

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
    FileListFile( SupportedArinc665Version version);

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
      SupportedArinc665Version version,
      const std::string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const FilesInfo &files,
      const UserDefinedData &userDefinedData);

    //! @copydoc FileListFile(SupportedArinc665Version,const std::string&,uint8_t,uint8_t,const FilesInfo&,const UserDefinedData&)
    FileListFile(
      SupportedArinc665Version version,
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

    //! @copydoc file(const FileInfo&)
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
     * @brief Returns the Check Value.
     *
     * @return The Check Value.
     **/
    const std::optional< CheckValue>& checkValue() const;

    /**
     * @brief Updates the Check Value
     *
     * @param[in] value
     *   Check Value.
     **/
    void checkValue( const std::optional< CheckValue> &value);

    //! @copydoc checkValue(const std::optional<CheckValue>&)
    void checkValue( std::optional< CheckValue> &&value);

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
     * @param[in] encodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is encoded.
     *
     * @return Raw representation of files information list.
     **/
    RawFile encodeFilesInfo( bool encodeV3Data) const;

    /**
     * @brief Decodes the files information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw files list file representation.
     * @param[in] offset
     *   Offset of the files information list.
     * @param[in] decodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is decoded.
     **/
    void decodeFilesInfo(
      const RawFile &rawFile,
      std::size_t offset,
      bool decodeV3Data);

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
    //! The Load Check Value (since ARINC 665-3)
    std::optional< CheckValue> checkValueValue;
};

}

#endif

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

namespace Arinc665 {
namespace File {

/**
 * @brief ARINC 665 File List File.
 **/
class FileListFile: public ListFile
{
  public:
    //! Maps media <sequence number, file name> to File Info
    using FileInfoMap = std::map< std::pair< uint8_t, string>, FileInfo>;
    //! Maps media <sequence number, file path> to File Info
    using FileInfoPathMap= std::map< std::pair< uint8_t, path>, FileInfo>;
    //! User defined data type
    using UserDefinedData = std::vector< uint8_t>;

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
     * @param mediaSetPn
     * @param mediaSequenceNumber
     * @param numberOfMediaSetMembers
     * @param filesInfo
     * @param userDefinedData
     **/
    FileListFile(
      Arinc665Version version,
      const string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const FilesInfo &filesInfo,
      const UserDefinedData &userDefinedData);

    //! @copydoc FileListFile(Arinc665Version,const string&,uint8_t,uint8_t,const FilesInfo&,const UserDefinedData&)
    FileListFile(
      Arinc665Version version,
      string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      FilesInfo &&filesInfo,
      UserDefinedData &&userDefinedData);

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
    string mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( const string &mediaSetPn) final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( string &&mediaSetPn) final;

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
    const FilesInfo& filesInfo() const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    FilesInfo& filesInfo();

    /**
     * @brief Return the list of files as map ( media index and filename as key).
     *
     * @return The list of files
     **/
    FileInfoMap filesInfoAsMap() const;

    /**
     * @brief Return the list of files as map ( media index and complete path as key).
     *
     * @return The list of files
     **/
    FileInfoPathMap filesInfoAsPathMap() const;

    /**
     * @brief Adds the given file information.
     *
     * @param[in] fileInfo
     *   The file information.
     **/
    void addFileInfo( const FileInfo &fileInfo);

    //! @copydoc addFileInfo(const FileInfo&)
    void addFileInfo( FileInfo &&fileInfo);

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
    string mediaSetPnValue;
    //! The media sequence number
    uint8_t mediaSequenceNumberValue;
    //! The number of media set members.
    uint8_t numberOfMediaSetMembersValue;
    //! Files information (list)
    FilesInfo filesInfoValue;
    //! Use defined data.
    UserDefinedData userDefinedDataValue;
};

}
}

#endif

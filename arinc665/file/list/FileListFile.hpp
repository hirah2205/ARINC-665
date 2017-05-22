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
 * @brief Declaration of class Arinc665::File::FileListFile.
 **/

#ifndef ARINC665_FILE_FILELISTFILE_HPP
#define ARINC665_FILE_FILELISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/FileInfo.hpp>

#include <boost/filesystem.hpp>

#include <vector>
#include <map>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Represents the parsed file list, which is contained on each media
 *   of the media set.
 **/
class FileListFile: public ListFile
{
  public:
    using path = boost::filesystem::path;
    //! Maps media <sequence number, file name> to File Info
    using FileInfoMap = std::map< std::pair< uint8_t, string>, FileInfo>;
    //! Maps media <sequence number, file path> to File Info
    using FileInfoPathMap= std::map< std::pair< uint8_t, path>, FileInfo>;
    using UserDefinedData = std::vector< uint8_t>;

    FileListFile( Arinc665Version version);

    FileListFile( const RawFile &file);

    //! @copydoc ListFile::operator=
    virtual FileListFile& operator=( const RawFile &file) override;

    //! @copydoc ListFile::getMediaSetPn
    virtual string getMediaSetPn() const override;

    //! @copydoc ListFile::setMediaSetPn
    virtual void setMediaSetPn( const string &mediaSetPn) override;

    //! @copydoc ListFile::getMediaSequenceNumber
    virtual uint8_t getMediaSequenceNumber() const override;

    //! @copydoc ListFile::setMediaSequenceNumber
    virtual void setMediaSequenceNumber( uint8_t mediaSequenceNumber) override;

    //! @copydoc ListFile::getNumberOfMediaSetMembers
    virtual uint8_t getNumberOfMediaSetMembers() const override;

    //! @copydoc ListFile::setNumberOfMediaSetMembers
    virtual void setNumberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) override;

    /**
     * @brief Returns the number of files.
     *
     * @return The number of files.
     **/
    size_t getNumberOfFiles() const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    const FileInfoList& getFileInfos() const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    FileInfoList& getFileInfos();

    /**
     * @brief Return the list of files as map ( media index and filename as key).
     *
     * @return The list of files
     **/
    FileInfoMap getFileInfosAsMap() const;

    /**
     * @brief Return the list of files as map ( media index and complete path as key).
     *
     * @return The list of files
     **/
    FileInfoPathMap getFileInfosAsPathMap() const;

    const UserDefinedData& getUserDefinedData() const;

    void setUserDefinedData( const UserDefinedData &userDefinedData);

    bool belongsToSameMediaSet( const FileListFile &other) const;

  private:
    //! @copydoc ListFile::encodeData
    virtual RawFile encode() const override final;

    void decodeBody( const RawFile &file);

    RawFile encodeFileInfo() const;

    FileInfoList decodeFileInfo( const RawFile &file, std::size_t offset);

    string mediaSetPn;
    uint8_t mediaSequenceNumber;
    uint8_t numberOfMediaSetMembers;
    FileInfoList fileInfos;
    UserDefinedData userDefinedData;
};

}
}

#endif

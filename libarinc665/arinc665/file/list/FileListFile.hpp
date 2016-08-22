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
 * @brief Declaration of class Arinc665::File::FileListFile
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
    using FileList = FileInfo::FileInfoList;
    using path = boost::filesystem::path;
    using FileMap = std::map< std::pair< unsigned int, string>, FileInfo>;
    using FilePathMap= std::map< std::pair< unsigned int, path>, FileInfo>;
    using UserDefinedData = std::vector< uint8_t>;

    FileListFile( void);

    FileListFile( const RawFile &file);

    //! @copydoc Arinc665File::getArincVersion
    virtual Arinc665Version getArincVersion( void) const override;

    //! @copydoc ListFile::getMediaSetPn
    virtual string getMediaSetPn( void) const override;

    //! @copydoc ListFile::setMediaSetPn
    virtual void setMediaSetPn( const string &mediaSetPn) override;

    //! @copydoc ListFile::getMediaSequenceNumber
    virtual uint8_t getMediaSequenceNumber( void) const override;

    //! @copydoc ListFile::setMediaSequenceNumber
    virtual void setMediaSequenceNumber( const uint8_t mediaSequenceNumber) override;

    //! @copydoc ListFile::getNumberOfMediaSetMembers
    virtual uint8_t getNumberOfMediaSetMembers( void) const override;

    //! @copydoc ListFile::setNumberOfMediaSetMembers
    virtual void setNumberOfMediaSetMembers( const uint8_t numberOfMediaSetMembers) override;

    /**
     * @brief Returns the number of files.
     *
     * @return The number of files.
     **/
    unsigned int getNumberOfFiles( void) const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    const FileList& getFiles( void) const;

    /**
     * @brief Return the list of files.
     *
     * @return The list of files
     **/
    FileList& getFiles( void);

    /**
     * @brief Return the list of files as map ( media index and filename as key).
     *
     * @return The list of files
     **/
    FileMap getFileMap( void) const;

    /**
     * @brief Return the list of files as map ( media index and complete path as key).
     *
     * @return The list of files
     **/
    FilePathMap getFilePathMap( void) const;

    const UserDefinedData& getUserDefinedData( void) const;

    void setUserDefinedData( const UserDefinedData &userDefinedData);

    bool belongsToSameMediaSet( const FileListFile &other) const;

  private:
    string mediaSetPn;
    uint8_t mediaSequenceNumber;
    uint8_t numberOfMediaSetMembers;
    FileList fileList;
    UserDefinedData userDefinedData;
};

}
}

#endif

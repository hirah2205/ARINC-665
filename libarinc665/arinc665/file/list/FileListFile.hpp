/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author
 *
 * @brief Declaration of class FileListFile
 **/

#ifndef ARINC665_FILELISTFILE_HPP
#define ARINC665_FILELISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/list/ListFile.hpp>
#include <arinc665/file/list/FileInfo.hpp>

#include <vector>
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
    using ListType = std::list< FileInfo>;

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

    unsigned int getNumberOfFiles( void) const;

    const ListType& getFiles( void) const;

    ListType& getFiles( void);

    const std::vector< uint8_t>& getUserDefinedData( void) const;

    void setUserDefinedData( const std::vector< uint8_t> &userDefinedData);

  private:
    string mediaSetPn;
    uint8_t mediaSequenceNumber;
    uint8_t numberOfMediaSetMembers;
    ListType fileList;
    std::vector< uint8_t> userDefinedData;
};
}
}

#endif

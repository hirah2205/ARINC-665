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
 * @brief Declaration of class Arinc665::File::FileInfo.
 **/

#ifndef ARINC665_FILEINFO_HPP
#define ARINC665_FILEINFO_HPP

#include <arinc665/file/File.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>
#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief The file information, which is part of the FileListFile.
 **/
class FileInfo
{
  public:
    //! Path type
    using path = boost::filesystem::path;
    //! String type
    using string = std::string;

    /**
     * @brief Initialises the file information
     *
     * @param[in] filename
     *   File filename
     * @param[in] pathName
     *   File path name
     * @param[in] memberSequenceNumber
     *   Member sequence number.
     * @param[in] crc
     *   File CRC
     **/
    FileInfo(
      const string &filename,
      const string &pathName,
      uint16_t memberSequenceNumber,
      uint16_t crc);

    //! @copydoc FileInfo::FileInfo(const string&,const string&,uint16_t,uint16_t)
    FileInfo(
      string &&filename,
      string &&pathName,
      uint16_t memberSequenceNumber,
      uint16_t crc);

    /**
     * @brief Returns the filename.
     *
     * @return The filename
     **/
    const string& getFilename() const;

    /**
     * @brief Updates the filename.
     *
     * @param[in] filename
     *   The filename.
     **/
    void setFilename( const string &filename);

    //! @copydoc setFilename(const string&)
    void setFilename( string &&filename);

    /**
     * @brief Returns the path name.
     *
     * @return The path name
     **/
    string getPathName() const;

    /**
     * @brief Updates the path name.
     *
     * @param[in] pathName
     *   The path name.
     **/
    void setPathName( const string &pathName);

    //! @copydoc setPathName(const string&)
    void setPathName( string &&pathName);

    /**
     * @brief Returns the path (path name / filename)
     *
     * @return path (incl. filename)
     **/
    path getPath() const;

    /**
     * @brief Returns the member sequence number.
     *
     * @return The member sequence number
     **/
    uint16_t getMemberSequenceNumber() const;

    /**
     * @brief Updates the  member sequence number.
     *
     * @param[in] memberSequenceNumber
     *   The  member sequence number.
     **/
    void setMemberSequenceNumber( uint16_t memberSequenceNumber);

    /**
     * @brief Returns the file CRC.
     *
     * @return The file CRC.
     **/
    uint16_t getCrc() const;

    /**
     * @brief Updates the file CRC.
     *
     * @param[in] crc
     *   File CRC.
     **/
    void setCrc( uint16_t crc);

    /**
     * @brief Compares the given file info against [this].
     *
     * @param[in] other
     *  Other file info.
     *
     * @return if [this] is equal to [other].
     **/
    bool operator==( const FileInfo &other) const;

    /**
     * @brief Compares the given file info against [this].
     *
     * @param[in] other
     *  Other file info.
     *
     * @return if [this] is un-equal to [other].
     **/
    bool operator!=( const FileInfo &other) const;

  private:
    //! file name
    string filename;
    //! path name
    string pathName;
    //! member sequence number
    uint16_t memberSequenceNumber;
    //! CRC
    uint16_t crc;
};

}
}

#endif

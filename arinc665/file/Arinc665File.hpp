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
 * @brief Declaration of class Arinc665::File::Arinc665File.
 **/

#ifndef ARINC665_ARINC665FILE_HPP
#define ARINC665_ARINC665FILE_HPP

#include <arinc665/file/File.hpp>

#include <boost/filesystem/path.hpp>

#include <cstdint>
#include <string>

namespace Arinc665 {
namespace File {

/**
 * @brief Base class of ARIC 665 file
 **/
class Arinc665File
{
  public:
    //! Path type
    using path = boost::filesystem::path;
    //! String type
    using string = std::string;
    //! String list type
    using StringList = std::list< string>;

    //! Base Header Offset
    //! @deprecated
    static constexpr size_t BaseHeaderOffset =
      sizeof( uint32_t) + // file length
      sizeof( uint32_t);  // ARINC Version + Spare

    //! unused at the moment
    static constexpr size_t BaseHeaderSize =
      sizeof( uint32_t) + // file length
      sizeof( uint16_t);  // ARINC Version

    //! The default checksum position
    static constexpr std::size_t DefaultChecksumPosition = 2U;

    /**
     * @brief Decodes the ARINC 665 string from the stream.
     *
     * @param[in] it
     *   current position, where the raw string start.
     * @param[out] str
     *   Will contain the decoded string.
     *
     * @return New iterator position.
     **/
    static RawFile::const_iterator decodeString(
      RawFile::const_iterator it,
      string &str);

    /**
     * @brief Encodes the ARINC 665 string to the stream.
     *
     * @param[in] str
     *  The string to encode.
     *
     * @return The encoded raw string.
     **/
    static RawFile encodeString( const string &str);

    /**
     * @brief Decodes the ARINC 665 string list from the stream.
     *
     * @param[in] it
     * @param[out] strList
     *
     *
     * @return New iterator position.
     **/
    static  RawFile::const_iterator decodeStringList(
      RawFile::const_iterator it,
      StringList &strList);

    /**
     * @brief Encodes the ARINC 665 string list to the stream.
     *
     * @param[in] strList
     *
     * @return The encoded raw string list.
     **/
    static RawFile encodeStringList( const StringList &strList);

    /**
     * @brief Encodes the given path for storage within ARINC 665 media set
     *   files.
     *
     * The main purpose is to replace all forward slashes '/' by
     * back-slashes '\'.
     *
     * @param[in] path
     *   The path, which should be converted.
     *
     * @return The converted path.
     **/
    static string encodePath( const path &path);

    /**
     * @brief Decode the file length information from the given file.
     *
     * @param[in] file
     *   The raw ARINC 665 file.
     *
     * @return The encoded file length.
     **/
    static uint32_t getFileLength( const RawFile &file);

    /**
     * @brief Decode the format version information from the given file.
     *
     * @param[in] file
     *   The raw ARINC 665 file.
     *
     * @return The encoded format version.
     **/
    static uint16_t getFormatVersion( const RawFile &file);

    /**
     * @brief Calculates the checksum over the given file.
     *
     * @param[in] file
     *   The raw ARINC 665 file.
     * @param[in] skipLastBytes
     *   Defines the number of bytes, which are skipped, e.g. contains the
     *   stored checksum.
     *
     * @return The calculated checksum.
     **/
    static uint16_t calculateChecksum(
      const RawFile &file,
      std::size_t skipLastBytes = 2U);

    /**
     * @brief Returns the ARINC 665 file class type.
     *
     * @param[in] rawFile
     *   The raw file data.
     *
     * @return The ARINC 665 file class type.
     **/
    static FileClassType getArincFileType( const RawFile &rawFile);

    /**
     * @brief Returns the load header file version for [rawFile]
     *
     * @param[in] rawFile
     *   The raw file data.
     *
     * @return The load header file version for [rawFile].
     * @retval LoadFileFormatVersion::Invalid
     *   When [rawFile] is nor a load header file
     **/
    static LoadFileFormatVersion getLoadFileFormatVersion(
      const RawFile &rawFile);

    /**
     * @brief Returns the batch file version for [rawFile]
     *
     * @param[in] rawFile
     *   The raw file data.
     *
     * @return The batch file version for [rawFile].
     * @retval BatchFileFormatVersion::Invalid
     *   When [rawFile] is nor a batch file
     **/
    static BatchFileFormatVersion getBatchFileFormatVersion(
      const RawFile &rawFile);

    /**
     * @brief Returns the media file version for [rawFile]
     *
     * @param[in] rawFile
     *   The raw file data.
     *
     * @return The media file version for [rawFile].
     * @retval MediaFileFormatVersion::Invalid
     *   When [rawFile] is nor a media file
     **/
    static MediaFileFormatVersion getMediaFileFormatVersion(
      const RawFile &rawFile);

    /**
     * @brief Returns the ARINC 665 version for the given [fileType] and
     *   [formatVersionField].
     *
     * @param[in] fileType
     *   File type.
     * @param[in] formatVersionField
     *   Format version field of file.
     *
     * @return The ARINC 665 version.
     * @retval Invalid
     *   If the given information are inconsistent
     **/
    static Arinc665Version getArinc665Version(
      FileType fileType,
      uint16_t formatVersionField);

    /**
     * @brief Returns the ARINC 665 version for the given [fileType] and
     *   [arinc665Version].
     *
     * @param[in] fileType
     *   File type.
     * @param[in] arinc665Version
     *   The ARINC 665 version.
     *
     * @return The format version field of file.
     **/
    static uint16_t getFormatVersionField(
      FileType fileType,
      Arinc665Version arinc665Version);

    /**
     * @brief Detects the file type for the given filename.
     *
     * @param[in] filename
     *   The filename.
     *
     * @return The file type.
     * @retval FileType::Invalid
     *   If [filename] is not a ARINC 665 file type.
     **/
    static FileType getFileType( const path &filename);

    //! Default destructor
    virtual ~Arinc665File() noexcept = default;

    /**
     * @brief Assigns raw data to the file.
     *
     * @param[in] rawFile
     *   File raw data
     *
     * @return *this
     **/
    virtual Arinc665File& operator=( const RawFile &rawFile);

    /**
     * @brief Returns the ARINC 665 file as raw data.
     *
     * @return the protocol file as raw data.
     **/
    operator RawFile() const;

    /**
     * @brief Returns the ARINC 665 file type.
     *
     * @return The ARINC 665 file type.
     **/
    FileType getFileType() const;

    /**
     * @brief Returns the ARINC 665 version of this file.
     *
     * @return The ARINC 665 version of this file.
     **/
    Arinc665Version getArincVersion() const;

    /**
     * @brief Updates the ARINC 665 version of this file.
     *
     * @param[in] version
     *   The new ARINC 665 version.
     **/
    void setArincVersion( Arinc665Version version);

    /**
     * @brief Returns the CRC of this file.
     *
     * @return The CRC of this file.
     **/
    uint16_t getCrc() const noexcept;

    /**
     * @brief Updates the CRC of this file.
     *
     * @param[in] crc
     *   The new CRC of this file
     **/
    void setCrc( uint16_t crc)  noexcept;

    /**
     * @brief Calculates the CRC.
     *
     * Calculates the CRC and stores the value.
     **/
    void calculateCrc();

  protected:
    /**
     * @brief Initialises the ARINC 665 file.
     *
     * @param[in] fileType
     *   File type.
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] checksumPosition
     *   Checksum position.
     **/
    Arinc665File(
      FileType fileType,
      Arinc665Version version,
      std::size_t checksumPosition = DefaultChecksumPosition) noexcept;

    /**
     * @brief Initialises the ARINC 665 file from the given raw data.
     *
     * @param[in] fileType
     *   File type.
     * @param[in] rawFile
     *   The raw file.
     * @param[in] checksumPosition
     *   Checksum position.
     **/
    Arinc665File(
      FileType fileType,
      const RawFile &rawFile,
      std::size_t checksumPosition = DefaultChecksumPosition);

    /**
     * @brief Initialises the ARINC 665 file from the given raw data.
     *
     * @param[in] rawFile
     *   The raw file.
     *
     * @return *this
     **/
    Arinc665File& operator=( const Arinc665File &rawFile);

    /**
     * @brief Encodes the ARINC 665 file as raw data.
     *
     * @return The  ARINC 665 file as raw data.
     **/
    virtual RawFile encode() const = 0;

    /**
     * @brief Inserts the header data into [rawFile].
     *
     * @param[in,out] rawFile
     *   The raw file, where the header is encoded.
     **/
    void insertHeader( RawFile &rawFile) const;

  private:
    /**
     * @brief Initialises class with the given raw data.
     *
     * @param[in] rawFile
     **/
    void decodeHeader( const RawFile &rawFile);

    //! File type
    const FileType fileType;
    //! checksum position
    const std::size_t checksumPosition;
    //! ARINC 665 Version
    Arinc665Version arinc665Version;
    //! Stored CRC value
    uint16_t crc;
};

}
}

#endif

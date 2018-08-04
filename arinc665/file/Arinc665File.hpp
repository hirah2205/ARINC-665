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

#include <filesystem>
#include <cstdint>
#include <string>

namespace Arinc665::File {

/**
 * @brief Base class of ARIC 665 file
 **/
class Arinc665File
{
  public:
    //! String list type
    using StringList = std::list< std::string>;

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

    //! Offset of the File Length Field
    static constexpr std::size_t FileLengthFieldOffset = 0U;

    //! Offset of the File Format Version Field
    static constexpr std::size_t FileFormatVersionFieldOffset = 4U;

    // IN ARINC 665-3 the Spare field for LUH files are used --> move to file specific part.
    static constexpr std::size_t SpareFieldOffset = 6U;

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
      std::string &str);

    /**
     * @brief Encodes the ARINC 665 string to the stream.
     *
     * @param[in] str
     *  The string to encode.
     *
     * @return The encoded raw string.
     **/
    static RawFile encodeString( const std::string &str);

    /**
     * @brief Decodes the ARINC 665 string list from the stream.
     *
     * @param[in] it
     *   current position, where the raw string list starts.
     * @param[out] strList
     *   The decoded string list.
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
    static std::string encodePath( const std::filesystem::path &path);

    /**
     * @brief Decode the file length information from the given file.
     *
     * @param[in] file
     *   The raw ARINC 665 file.
     *
     * @return The encoded file length.
     *
     * @throw InvalidArinc665File
     *   If the file size is to small to represent an valid ARINC 665 file.
     **/
    static uint32_t fileLength( const RawFile &file);

    /**
     * @brief Decode the format version information from the given file.
     *
     * @param[in] file
     *   The raw ARINC 665 file.
     *
     * @return The encoded format version.
     **/
    static uint16_t formatVersion( const RawFile &file);

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
    static FileClassType fileType( const RawFile &rawFile);

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
    static LoadFileFormatVersion loadFileFormatVersion(
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
    static BatchFileFormatVersion batchFileFormatVersion(
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
    static MediaFileFormatVersion mediaFileFormatVersion(
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
    static Arinc665Version arinc665Version(
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
    static uint16_t formatVersionField(
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
    static FileType fileType( const std::filesystem::path &filename);

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
    FileType fileType() const;

    /**
     * @brief Returns the ARINC 665 version of this file.
     *
     * @return The ARINC 665 version of this file.
     **/
    Arinc665Version arincVersion() const;

    /**
     * @brief Updates the ARINC 665 version of this file.
     *
     * @param[in] version
     *   The new ARINC 665 version.
     **/
    void arincVersion( Arinc665Version version);

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
     * @brief Assignment operator
     *
     * @param[in] other
     *   The other file.
     *
     * @return *this
     **/
    Arinc665File& operator=( const Arinc665File &other);

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
    const FileType fileTypeV;
    //! checksum position
    const std::size_t checksumPosition;
    //! ARINC 665 Version
    Arinc665Version arinc665VersionValue;
};

}

#endif

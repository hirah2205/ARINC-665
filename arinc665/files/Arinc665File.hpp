/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::Arinc665File.
 **/

#ifndef ARINC665_FILES_ARINC665FILE_HPP
#define ARINC665_FILES_ARINC665FILE_HPP

#include <arinc665/files/Files.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <list>
#include <set>
#include <cstdint>

namespace Arinc665::Files {

/**
 * @brief ARINC 665 Protocol %File.
 **/
class ARINC665_EXPORT Arinc665File
{
  public:
    //! String List
    using StringList = std::list< std::string >;

    //! Base Header Size
    static constexpr ptrdiff_t BaseHeaderSize{
      sizeof( uint32_t ) +  // file length
      sizeof( uint16_t ) }; // ARINC Version

    //! Default Checksum Position
    static constexpr ptrdiff_t DefaultChecksumPosition{ 2 };

    //! Offset of the File Length Field
    static constexpr ptrdiff_t FileLengthFieldOffset{ 0 };

    //! Offset of the File Format Version Field
    static constexpr ptrdiff_t FileFormatVersionFieldOffset{ 4 };

    /**
     * @brief Encodes the given path for storage within ARINC 665 media set
     *   files.
     *
     * The main purpose is to replace all forward slashes '/' by
     * back-slashes '\'.
     * The path always contains a trailing '\'.
     *
     * @param[in] path
     *   Path, which should be converted.
     *
     * @return Converted Path.
     **/
    static std::string encodePath( const std::filesystem::path &path );

    /**
     * @brief Decode the file length information from the given file.
     *
     * @param[in] file
     *   Raw ARINC 665 file.
     *
     * @return Encoded file length.
     *
     * @throw InvalidArinc665File
     *   If the file size is too small to represent a valid ARINC 665 file.
     **/
    static uint32_t fileLength( const ConstRawFileSpan &file );

    /**
     * @brief Decode the format version information from the given file.
     *
     * @param[in] file
     *   Raw ARINC 665 file.
     *
     * @return Decoded format version.
     *
     * @throw InvalidArinc665File
     *   If the file size is too small to represent a valid ARINC 665 file.
     **/
    static uint16_t formatVersion( const ConstRawFileSpan &file );

    /**
     * @brief Calculates the checksum over the given file.
     *
     * @param[in] file
     *   Raw ARINC 665 file.
     *
     * @return Calculated checksum.
     **/
    static uint16_t calculateChecksum( const ConstRawFileSpan &file );

    /**
     * @brief Returns the ARINC 665 file class type.
     *
     * @param[in] rawFile
     *   Raw file data.
     *
     * @return ARINC 665 file class type.
     **/
    static FileClassType fileType( const ConstRawFileSpan &rawFile );

    /**
     * @brief Returns the load header file version for @p rawFile.
     *
     * @param[in] rawFile
     *   Raw file data.
     *
     * @return The load header file version for @p rawFile.
     * @retval LoadFileFormatVersion::Invalid
     *   When @p rawFile is not a load header file
     **/
    static LoadFileFormatVersion loadFileFormatVersion(
      const ConstRawFileSpan &rawFile );

    /**
     * @brief Returns the batch file version for @p rawFile.
     *
     * @param[in] rawFile
     *   Raw file data.
     *
     * @return Batch file version for @p rawFile.
     * @retval BatchFileFormatVersion::Invalid
     *   When @p rawFile is not a batch file
     **/
    static BatchFileFormatVersion batchFileFormatVersion(
      const ConstRawFileSpan &rawFile );

    /**
     * @brief Returns the media file version for @p rawFile
     *
     * @param[in] rawFile
     *   Raw file data.
     *
     * @return The media file version for [rawFile].
     * @retval MediaFileFormatVersion::Invalid
     *   When @p rawFile is not a media file
     **/
    static MediaFileFormatVersion mediaFileFormatVersion(
      const ConstRawFileSpan &rawFile );

    /**
     * @brief Returns the Supported ARINC 665 Version for the given @p fileType.
     *   and [formatVersionField].
     *
     * @param[in] fileType
     *   File Type.
     * @param[in] formatVersionField
     *   Format version field of file.
     *
     * @return The Supported ARINC 665 version.
     * @retval SupportedArinc665Version::Invalid
     *   If the given information are inconsistent or the Version is not
     *   supported.
     **/
    static SupportedArinc665Version arinc665Version(
      FileType fileType,
      uint16_t formatVersionField );

    /**
     * @brief Returns the ARINC 665 version for the given @p fileType and
     *   @p arinc665Version.
     *
     * @param[in] fileType
     *   File type.
     * @param[in] arinc665Version
     *   ARINC 665 version.
     *
     * @return Format version field of file.
     **/
    static uint16_t formatVersionField(
      FileType fileType,
      SupportedArinc665Version arinc665Version );

    /**
     * @brief Detects the file type for the given filename.
     *
     * @param[in] filename
     *   filename.
     *
     * @return The file type.
     * @retval FileType::Invalid
     *   If @p filename is not a ARINC 665 file type.
     **/
    static FileType fileType( const std::filesystem::path &filename );

    //! Destructor
    virtual ~Arinc665File() noexcept = default;

    /**
     * @brief Assigns raw data to the file.
     *
     * @param[in] rawFile
     *   File raw data
     *
     * @return *this
     **/
    virtual Arinc665File& operator=( const ConstRawFileSpan &rawFile );

    /**
     * @brief Returns the ARINC 665 file as raw data.
     *
     * @return the protocol file as raw data.
     **/
    [[nodiscard]] explicit operator RawFile() const;

    /**
     * @brief Returns the ARINC 665 file type.
     *
     * @return ARINC 665 file type.
     **/
    [[nodiscard]] virtual FileType fileType() const noexcept = 0;

    /**
     * @name ARINC 665 Version
     * @{
     **/

    /**
     * @brief Returns the ARINC 665 version of this file.
     *
     * @return ARINC 665 version of this file.
     **/
    [[nodiscard]] SupportedArinc665Version arincVersion() const;

    /**
     * @brief Updates the ARINC 665 version of this file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    void arincVersion( SupportedArinc665Version version );

    /** @} **/

  protected:
    /**
     * @brief Initialises the ARINC 665 file.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] checksumPosition
     *   Checksum position.
     **/
    explicit Arinc665File(
      SupportedArinc665Version version,
      ptrdiff_t checksumPosition = DefaultChecksumPosition ) noexcept;

    /**
     * @brief Initialises the ARINC 665 file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw File.
     * @param[in] expectedFileType
     *   Expected file type.
     * @param[in] checksumPosition
     *   Checksum position.
     **/
    explicit Arinc665File(
      const ConstRawFileSpan &rawFile,
      FileType expectedFileType,
      ptrdiff_t checksumPosition = DefaultChecksumPosition );

    /**
     * @brief Copy Constructor
     *
     * @param[in] other
     **/
    Arinc665File( const Arinc665File &other ) noexcept = default;

    //! @copydoc Arinc665File(const Arinc665File&)
    Arinc665File( Arinc665File &&other ) noexcept = default;

    /**
     * @brief Assignment operator
     *
     * @param[in] other
     *   Other file.
     *
     * @return *this
     **/
    Arinc665File& operator=( const Arinc665File &other ) noexcept;

    //! @copydoc operator=(const Arinc665File&)
    Arinc665File& operator=( Arinc665File &&other ) noexcept;

    /**
     * @brief Encodes the ARINC 665 file as raw data.
     *
     * @return ARINC 665 file as raw data.
     **/
    [[nodiscard]] virtual RawFile encode() const = 0;

    /**
     * @brief Inserts the header data and file CRC into @p rawFile.
     *
     * @param[in,out] rawFile
     *   Raw file, where the header is encoded.
     *
     * @throw InvalidArinc665File
     *   When file is too small
     * @throw InvalidArinc665File
     *   When file size is invalid
     **/
    void insertHeader( const RawFileSpan &rawFile ) const;

  private:
    /**
     * @brief Initialises class with the given raw data.
     *
     * @param[in] rawFile
     *   Raw File.
     * @param[in] expectedFileType
     *   Expected file type.
     *
     * @throw InvalidArinc665File
     *   When file is too small
     * @throw InvalidArinc665File
     *   When file size field is invalid
     * @throw InvalidArinc665File
     *   When file format is wrong
     * @throw InvalidArinc665File
     *   When CRC is invalid
     **/
    void decodeHeader(
      const ConstRawFileSpan &rawFile,
      FileType expectedFileType );

    //! Checksum Position
    const ptrdiff_t checksumPosition;
    //! ARINC 665 Version
    SupportedArinc665Version arinc665VersionV;
};

}

#endif

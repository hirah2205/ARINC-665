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

#include <cstdint>

namespace Arinc665 {
namespace File {

/**
 * @brief Base class of ARIC 665 file
 **/
class Arinc665File
{
  public:
    static constexpr size_t BaseHeaderOffset = 8U;

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

    //! Default destructor
    virtual ~Arinc665File() noexcept = default;

    /**
     * @brief Assigns raw data to the file.
     *
     * @param[in] file
     *   File raw data
     *
     * @return *this
     **/
    virtual Arinc665File& operator=( const RawFile &file);

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
    Arinc665File(
      FileType fileType,
      Arinc665Version version,
      std::size_t checksumPosition = 2U) noexcept;

    Arinc665File(
      FileType fileType,
      const RawFile &file,
      std::size_t checksumPosition = 2U);

    Arinc665File& operator=( const Arinc665File &file);

    virtual RawFile encode() const = 0;

    void insertHeader( RawFile &file) const;

    /**
     * @brief Initialises class with the given raw data.
     *
     * @param[in] file
     * @param[in] expectedFormatVersion
     * @param[in] checksumPosition
     **/
    void decodeHeader(
      const RawFile &file,
      Arinc665FileFormatVersion expectedFormatVersion = Arinc665FileFormatVersion::Invalid);

  private:
    const FileType fileType;
    const std::size_t checksumPosition;
    //! ARINC 665 Version
    Arinc665Version arinc665Version;
    //! Stored CRC value
    uint16_t crc;
};

}
}

#endif

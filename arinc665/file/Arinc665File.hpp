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
    static constexpr unsigned int BaseHeaderOffset = 8;

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
      unsigned int skipLastBytes = 2);

    //! Default destructor
    virtual ~Arinc665File() noexcept = default;

    /**
     * @brief Returns the ARINC 665 version of this file.
     *
     * @return The ARINC 665 version of this file.
     **/
    virtual Arinc665Version getArincVersion() const = 0;

    /**
     * @brief Returns the CRC of this file.
     *
     * @return The CRC of this file.
     **/
    uint16_t getCrc() const;

    /**
     * @brief Updates the CRC of this file.
     *
     * @param[in] crc
     *   The new CRC of this file
     **/
    void setCrc( uint16_t crc);

    // virtual void decode( const RawFile &data);

    // virtual RawFile encode( void) const = 0;

  protected:
    /**
     * @brief Default constructor
     **/
    Arinc665File();

    /**
     * @brief Initialises class with the given raw data.
     *
     * @param[in] file
     * @param[in] expectedFormatVersion
     * @param[in] checksumPosition
     **/
    Arinc665File(
      const RawFile &file,
      Arinc665FileFormatVersion expectedFormatVersion,
      unsigned int checksumPosition = 2);

  private:
    uint16_t crc;
};

}
}

#endif

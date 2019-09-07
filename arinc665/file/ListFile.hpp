/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::ListFile.
 **/


#ifndef ARINC665_LISTFILE_HPP
#define ARINC665_LISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>

namespace Arinc665::File {

/**
 * @brief Base Class for ARINC 665 Protocol List Files
 *
 * This intermediate class is defined to add common methods for all List Files.
 *
 * Children are:
 * * FileListFile
 * * LoadListFile
 * * BatchListFile
 **/
class ListFile: public Arinc665File
{
  public:
    /**
     * @brief Returns the Media Set Part Number, where the files belongs to.
     *
     * @return The Media Set Part Number.
     **/
    [[nodiscard]] virtual std::string_view mediaSetPn() const = 0;

    /**
     * @brief Sets the Media Part Number, where the files belongs to.
     *
     * @param[in] mediaSetPn
     *   The Media Set part Number.
     **/
    virtual void mediaSetPn( std::string_view mediaSetPn) = 0;

    //! @copydoc mediaSetPn(std::string_view)
    virtual void mediaSetPn( std::string &&mediaSetPn) = 0;

    /**
     * @brief Returns the media sequence number.
     *
     * @return The media sequence number
     **/
    [[nodiscard]] virtual uint8_t mediaSequenceNumber() const = 0;

    /**
     * @brief Updates the media sequence number
     *
     * @param[in] mediaSequenceNumber
     *   New media sequence number
     **/
    virtual void mediaSequenceNumber( uint8_t mediaSequenceNumber) = 0;

    /**
     * @brief Returns the number of media set members.
     *
     * @return The number of media set members.
     **/
    [[nodiscard]] virtual uint8_t numberOfMediaSetMembers() const = 0;

    /**
     * @brief Updates the number of media set members.
     *
     * @param[in] numberOfMediaSetMembers
     *   The new number of media set members
     **/
    virtual void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) = 0;

  protected:
    //! @copydoc Arinc665File::Arinc665File(SupportedArinc665Version,std::size_t)
    ListFile(
      SupportedArinc665Version version,
      std::size_t checksumPosition = 2U) noexcept;

    //! @copydoc Arinc665File::Arinc665File(const RawFile&,FileType,std::size_t)
    ListFile(
      const RawFile &rawFile,
      FileType expectedFileType,
      std::size_t checksumPosition = 2U);

    /**
     * @brief Copy Constructor
     *
     * @param[in] other
     **/
    ListFile( const ListFile &other) = default;

    //! @copydoc ListFile(const ListFile&)
    ListFile( ListFile &&other) = default;

    /**
     * @brief Assignment operator
     *
     * @param[in] other
     *   The other file.
     *
     * @return *this
     **/
    ListFile& operator=( const ListFile &other) = default;

    //! @copydoc operator=(const Arinc665File&)
    ListFile& operator=( ListFile &&other) = default;

};

}

#endif

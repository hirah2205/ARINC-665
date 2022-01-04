/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::ListFile.
 **/


#ifndef ARINC665_FILES_LISTFILE_HPP
#define ARINC665_FILES_LISTFILE_HPP

#include <arinc665/files/Files.hpp>
#include <arinc665/files/Arinc665File.hpp>

namespace Arinc665::Files {

/**
 * @brief Base Class for ARINC 665 Protocol List %Files.
 *
 * This intermediate class is defined to add common methods for all List Files.
 *
 * Children are:
 * - FileListFile
 * - LoadListFile
 * - BatchListFile
 **/
class ListFile: public Arinc665File
{
  public:
    /**
     * @name Media Set Part Number
     * @{
     **/

    /**
     * @brief Returns the Media Set Part Number, where the files belongs to.
     *
     * @return Media Set Part Number.
     **/
    [[nodiscard]] std::string_view mediaSetPn() const;

    /**
     * @brief Sets the Media Part Number, where the files belongs to.
     *
     * @param[in] mediaSetPn
     *   Media Set part Number.
     **/
    void mediaSetPn( std::string_view mediaSetPn );

    //! @copydoc mediaSetPn(std::string_view)
    void mediaSetPn( std::string &&mediaSetPn );

    /** @} **/

    /**
     * @name Media Sequence Number
     * @{
     **/

    /**
     * @brief Returns the Media Sequence Number.
     *
     * @return Media sequence number
     **/
    [[nodiscard]] uint8_t mediaSequenceNumber() const;

    /**
     * @brief Updates the media sequence number
     *
     * @param[in] mediaSequenceNumber
     *   New media sequence number
     **/
    void mediaSequenceNumber( uint8_t mediaSequenceNumber );

    /** @} **/

    /**
     * @name Number of Media Set Members
     * @{
     **/

    /**
     * @brief Returns the number of media set members.
     *
     * @return Number of media set members.
     **/
    [[nodiscard]] uint8_t numberOfMediaSetMembers() const;

    /**
     * @brief Updates the number of media set members.
     *
     * @param[in] numberOfMediaSetMembers
     *   New number of media set members
     **/
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers );

    /** @} **/

  protected:
    /**
     * @copydoc Arinc665File::Arinc665File(SupportedArinc665Version,ptrdiff_t)
     **/
    explicit ListFile(
      SupportedArinc665Version version,
      ptrdiff_t checksumPosition = DefaultChecksumPosition ) noexcept;

    /**
     * @copydoc Arinc665File::Arinc665File(SupportedArinc665Version,ptrdiff_t)
     * @param[in] mediaSetPn
     *   Media Set Part Number.
     * @param[in] mediaSequenceNumber
     *   Media Sequence Number [1..255].
     * @param[in] numberOfMediaSetMembers
     *   Number of Media Set Members [1..255] & mediaSequenceNumber <=
     *     @p numberOfMediaSetMembers
     **/
    ListFile(
      SupportedArinc665Version version,
      std::string_view mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      ptrdiff_t checksumPosition = DefaultChecksumPosition ) noexcept;

    //! @copydoc ListFile(SupportedArinc665Version,std::string_view,uint8_t,uint8_t,ptrdiff_t)
    ListFile(
      SupportedArinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      ptrdiff_t checksumPosition = DefaultChecksumPosition ) noexcept;

    //! @copydoc Arinc665File::Arinc665File(const ConstRawFileSpan&,FileType,ptrdiff_t)
    ListFile(
      const ConstRawFileSpan &rawFile,
      FileType expectedFileType,
      ptrdiff_t checksumPosition = DefaultChecksumPosition );

    /**
     * @brief Copy Constructor
     *
     * @param[in] other
     **/
    ListFile( const ListFile &other ) = default;

    //! @copydoc ListFile(const ListFile&)
    ListFile( ListFile &&other ) = default;

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

    /**
     * @brief Encodes the Media Information.
     *
     * @return Raw representation of Media Information.
     **/
    [[nodiscard]] RawFile encodeMediaInformation() const;

    /**
     * @brief Decodes the Media Information.
     *
     * @param[in] rawFile
     *   Raw List File
     * @param[in] mediaInformationPtr
     *   Media Information Pointer as stored within List File.
     *
     * @throw InvalidArinc665File
     *   When Offset is Invalid
     **/
    void decodeMediaInformation(
      const ConstRawFileSpan &rawFile,
      uint32_t mediaInformationPtr );

  private:
    //! Media Set Part Number
    std::string mediaSetPnV;
    //! Media Sequence Number
    uint8_t mediaSequenceNumberV;
    //! Number of Media Set Members.
    uint8_t numberOfMediaSetMembersV;
};

}

#endif

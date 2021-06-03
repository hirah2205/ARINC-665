/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::LoadListFile.
 **/

#ifndef ARINC665_LOADLISTFILE_HPP
#define ARINC665_LOADLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/LoadInfo.hpp>

#include <string_view>
#include <vector>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Load List %File (LOADS.LUM).
 *
 * @par File Format (ARINC 665-4)
 * Name of Field                   | Field Size (bits)
 * --------------------------------|:----------------:
 * LOADS.LUM File Length           | 32
 * Media File Format Version       | 16
 * Spare                           | 16
 * Pointer to Media Set PN Length  | 32
 * Pointer to Number of Loads      | 32
 * Pointer to User Defined Data    | 32
 * Expansion Point No. 1           |  0
 * Media Set PN Length             | 16
 * Media Set PN                    | 16
 * Media Sequence Number (X)       |  8
 * Number Of Media Set Members (Y) |  8
 * Number of Loads                 | 16
 * + Load Pointer                  | 16
 * + Load PN Length                | 16
 * + Load PN                       | 16
 * + Header File Name Length       | 16
 * + Header File Name              | 16
 * + Member Sequence Number        | 16
 * + Number of Target HW IDs       | 16
 * +* Target HW ID Length          | 16
 * +* Target HW ID                 | 16
 * + Expansion Point No. 2         |  0
 * Expansion Point No. 3           |  0
 * User Defined Data               | 16
 * LOADS.LUM File CRC              | 16
 **/
class LoadListFile: public ListFile
{
  public:
    //! User Defined Data.
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Spare field (since ARINC 665-2).
    static constexpr ptrdiff_t SpareFieldOffsetV2{ 6U };

    //! Offset of the Media Set Part Number Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t MediaSetPartNumberPointerFieldOffsetV1{ 6U };

    //! Offset of the Media Set Part Number Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t MediaSetPartNumberPointerFieldOffsetV2{ 8U };

    //! Offset of the Loads Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t LoadFilesPointerFieldOffsetV1{ 10U };

    //! Offset of the Loads Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t LoadFilesPointerFieldOffsetV2{ 12U };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t UserDefinedDataPointerFieldOffsetV1{ 14U };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t UserDefinedDataPointerFieldOffsetV2{ 16U };

    //! First Start of Pointer Data for ARINC 665 Load List Files (since ARINC 665-1).
    static constexpr ptrdiff_t FileHeaderSizeV1{ 18U };

    //! First Start of Pointer Data for ARINC 665 Load List Files (since ARINC 665-2).
    static constexpr ptrdiff_t FileHeaderSizeV2{ 20U };

    /**
     * @brief Creates an empty load list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    explicit LoadListFile( SupportedArinc665Version version );

    /**
     * @brief Creates load list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] mediaSetPn
     *   Media set part number.
     * @param[in] mediaSequenceNumber
     *   Media sequence number
     * @param[in] numberOfMediaSetMembers
     *   Number of media set members
     * @param[in] loads
     *   Loads information
     * @param[in] userDefinedData
     *   User defined data.
     **/
    LoadListFile(
      SupportedArinc665Version version,
      std::string_view mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const LoadsInfo &loads,
      const UserDefinedData &userDefinedData );

    //! @copydoc LoadListFile(SupportedArinc665Version,std::string_view,uint8_t,uint8_t,const LoadsInfo&,const UserDefinedData&)
    LoadListFile(
      SupportedArinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      LoadsInfo &&loads = {},
      UserDefinedData &&userDefinedData = {} );

    /**
     * @brief Creates a load list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit LoadListFile( const ConstRawFileSpan &rawFile );

    //! @copydoc ListFile::operator=(const ConstRawFileSpan&)
    LoadListFile& operator=( const ConstRawFileSpan &rawFile ) final;

    //! @copydoc ListFile::fileType() const noexcept
    [[nodiscard]] FileType fileType() const noexcept final;

    //! @copydoc ListFile::mediaSetPn() const
    [[nodiscard]] std::string_view mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn(std::string_view)
    void mediaSetPn( std::string_view mediaSetPn ) final;

    //! @copydoc ListFile::mediaSetPn(std::string&&)
    void mediaSetPn( std::string &&mediaSetPn ) final;

    //! @copydoc ListFile::mediaSequenceNumber() const
    [[nodiscard]] uint8_t mediaSequenceNumber() const final;

    //! @copydoc ListFile::mediaSequenceNumber(uint8_t)
    void mediaSequenceNumber( uint8_t mediaSequenceNumber ) final;

    //! @copydoc ListFile::numberOfMediaSetMembers() const
    [[nodiscard]] uint8_t numberOfMediaSetMembers() const final;

    //! @copydoc ListFile::numberOfMediaSetMembers(uint8_t)
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers ) final;

    /**
     * @brief Returns the number of loads.
     *
     * @return The number of loads.
     **/
    [[nodiscard]] size_t numberOfLoads() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    [[nodiscard]] const LoadsInfo& loads() const;

    /**
     * @brief Returns the loads.
     *
     * @return Loads.
     **/
    [[nodiscard]] LoadsInfo& loads();

    /**
     * @brief Adds the given load information.
     *
     * @param[in] load
     *   Load information.
     **/
    void load( const LoadInfo &load );

    //! @copydoc load(const LoadInfo&)
    void load( LoadInfo &&load );

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    [[nodiscard]] const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @note The user defined data must be multiple of 2-bytes in size.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData );

    /**
     * @brief Returns if the given load list file belongs to the same media set.
     *
     * Compares:
     * - Part number,
     * - number of media set members,
     * - the load list, and
     * - the user-defined data
     * for match.
     *
     * @param[in] other
     *   The other load list file
     *
     * @return If the given load list files belongs to the same media set
     **/
    [[nodiscard]] bool belongsToSameMediaSet( const LoadListFile &other ) const;

  private:
    //! @copydoc ListFile::encode
    [[nodiscard]] RawFile encode() const final;

    /**
     * @brief Decodes the body of the batch file.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     **/
    void decodeBody( const ConstRawFileSpan &rawFile );

    /**
     * @brief Encodes the batches information list.
     *
     * @return Raw representation of loads information list.
     **/
    [[nodiscard]] RawFile encodeLoadsInfo() const;

    /**
     * @brief Decodes the loads information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     * @param[in] offset
     *   Offset of the loads information list.
     *
     * @throw InvalidArinc665File When member sequence number is out of range
     **/
    void decodeLoadsInfo( const ConstRawFileSpan &rawFile, ptrdiff_t offset );

    /**
     * @brief Checks, if the User Defined Data is a multiple of 2 size.
     **/
    void checkUserDefinedData();

    //! Media Set Part Number
    std::string mediaSetPnV;
    //! Media Sequence Number
    uint8_t mediaSequenceNumberV;
    //! Number of Media Set Members
    uint8_t numberOfMediaSetMembersV;
    //! Load List
    LoadsInfo loadsV;
    //! User Defined Data
    UserDefinedData userDefinedDataV;
};

}

#endif

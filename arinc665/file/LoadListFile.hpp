/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::LoadListFile.
 **/

#ifndef ARINC665_LOADLISTFILE_HPP
#define ARINC665_LOADLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/LoadInfo.hpp>

#include <vector>
#include <map>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Load List File (LOADS.LUM).
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
    //! Loads information map
    using LoadsInfoMap = std::map< std::pair< uint8_t, std::string>, LoadInfo>;
    //! User defined data type.
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Spare field.
    static constexpr std::size_t SpareFieldOffset = 6U;

    //! Offset of the Media Set Part Number Pointer Field
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffset = 8U;

    //! Offset of the Loads Pointer Field
    static constexpr std::size_t LoadFilesPointerFieldOffset = 12U;

    //! Offset of the User Defined Data Pointer Field
    static constexpr std::size_t UserDefinedDataPointerFieldOffset = 16U;

    //! First Start of pointer data for ARINC 665 Load List Files.
    static constexpr std::size_t FileHeaderSize = 20U;

    /**
     * @brief Creates an empty load list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    LoadListFile( SupportedArinc665Version version);

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
      const std::string &mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const LoadsInfo &loads,
      const UserDefinedData &userDefinedData);

    //! @copydoc LoadListFile(SupportedArinc665Version,const std::string&,uint8_t,uint8_t,const LoadsInfo&,const UserDefinedData&)
    LoadListFile(
      SupportedArinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      LoadsInfo &&loads = {},
      UserDefinedData &&userDefinedData = {});

    /**
     * @brief Creates a load list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    LoadListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=
    LoadListFile& operator=( const RawFile &rawFile) final;

    //! @copydoc ListFile::mediaSetPn
    std::string mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( const std::string &mediaSetPn) final;

    //! @copydoc ListFile::mediaSetPn
    void mediaSetPn( std::string &&mediaSetPn) final;

    //! @copydoc ListFile::mediaSequenceNumber
    uint8_t mediaSequenceNumber() const final;

    //! @copydoc ListFile::mediaSequenceNumber
    void mediaSequenceNumber( uint8_t mediaSequenceNumber) final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    uint8_t numberOfMediaSetMembers() const final;

    //! @copydoc ListFile::numberOfMediaSetMembers
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) final;

    /**
     * @brief Returns the number of loads.
     *
     * @return The number of loads.
     **/
    size_t numberOfLoads() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    const LoadsInfo& loads() const;

    /**
     * @brief Returns the loads.
     *
     * @return The Loads.
     **/
    LoadsInfo& loads();

    /**
     * @brief Returns the loads as map for easy access.
     *
     * @return The loads as map.
     */
    LoadsInfoMap loadsAsMap() const;

    /**
     * @brief Adds the given load information.
     *
     * @param[in] load
     *   Load information.
     **/
    void load( const LoadInfo &load);

    //! @copydoc load(const LoadInfo&)
    void load( LoadInfo &&load);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @note The user defined data must be multiple of 2-bytes in size.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

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
    bool belongsToSameMediaSet( const LoadListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    RawFile encode() const final;

    /**
     * @brief Decodes the body of the batch file.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the batches information list.
     *
     * @return Raw representation of loads information list.
     **/
    RawFile encodeLoadsInfo() const;

    /**
     * @brief Decodes the loads information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw load list file representation.
     * @param[in] offset
     *   Offset of the loads information list.
     **/
    void decodeLoadsInfo( const RawFile &rawFile, std::size_t offset);

    //! Media set part number
    std::string mediaSetPnValue;
    //! media sequence number
    uint8_t mediaSequenceNumberValue;
    //! number of media set members
    uint8_t numberOfMediaSetMembersValue;
    //! The load list
    LoadsInfo loadsValue;
    //! user defined data
    UserDefinedData userDefinedDataValue;
};

}

#endif

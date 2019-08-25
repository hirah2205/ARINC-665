/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::LoadHeaderFile.
 **/

#ifndef ARINC665_FILE_LOAD_LOADHEADERFILE_HPP
#define ARINC665_FILE_LOAD_LOADHEADERFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/LoadFileInfo.hpp>

#include <map>
#include <list>
#include <vector>
#include <optional>
#include <cstdint>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Load Header File (*.LUH).
 *
 * @par File Format (ARINC 665-4)
 * Name of Field                                    | Field Size (bits)
 * -------------------------------------------------|:----------------:
 * Header File Length                               | 32
 * Load File Format Version                         | 16
 * Part Flags                                       | 16
 * Pointer to Load PN Length                        | 32
 * Pointer to Number of Target HW IDs               | 32
 * Pointer to Number of Data Files                  | 32
 * Pointer to Number of Support Files               | 32
 * Pointer to User Defined Data                     | 32
 * Pointer to Load Type Description Length          | 32
 * Pointer to Number of Target HW ID with Positions | 32
 * Pointer to Load Check Value Length               | 32
 * Expansion Point No.1                             |  0
 * Load PN Length                                   | 16
 * Load PN                                          | 16
 * Expansion Point No. 2                            |  0
 * Load Type Description Length                     | 16
 * Load Type Description                            | 16
 * Load Type ID                                     | 16
 * Expansion Point No. 3                            |  0
 * Number of Target HW IDs                          | 16
 * * Target HW ID Length                            | 16
 * * Target HW ID                                   | 16
 * Expansion Point No. 4                            |  0
 * Number of Target HW ID with Positions            | 16
 * % Target HW ID with Positions Length             | 16
 * % Target HW ID with Positions                    | 16
 * % Number of Target HW ID Positions               | 16
 * %& Position Length                               | 16
 * %& Position                                      | 16
 * Expansion Point No. 5                            |  0
 * Number of Data Files                             | 16
 * + Data File Pointer                              | 16
 * + Data File Name Length                          | 16
 * + Data File Name                                 | 16
 * + Data File PN Length                            | 16
 * + Data File PN                                   | 16
 * + Data File Length                               | 32
 * + Data File CRC                                  | 16
 * + Data File Length in Bytes                      | 64
 * + Data File Check Value Length                   | 16
 * + Data File Check Value Type                     | 16
 * + Data File Check Value                          | 16
 * + Expansion Point No. 6                          |  0
 * Expansion Point No. 7                            |  0
 * Number of Support Files                          | 16
 * # Support File Pointer                           | 16
 * # Support File Name Length                       | 16
 * # Support File Name                              | 16
 * # Support File PN Length                         | 16
 * # Support File PN                                | 16
 * # Support File Length                            | 32
 * # Support File CRC                               | 16
 * # Support File Check Value Length                | 16
 * # Support File Check Value Type                  | 16
 * # Support File Check Value                       | 16
 * # Expansion Point No. 8                          |  0
 * Expansion Point No. 9                            |  0
 * User Defined Data                                | 16
 * Expansion Point No. 10                           |  0
 * Load Check Value Length                          | 16
 * Load Check Value Type                            | 16
 * Load Check Value                                 | 16
 * Header File CRC                                  | 16
 * Load CRC                                         | 32
 **/
class LoadHeaderFile: public Arinc665File
{
  public:
    //! Target Hardware ID / Positions list type
    using TargetHardwareIdPositions =
      std::map< std::string, StringList>;
    //! User defined data type
    using UserDefinedData = std::vector< uint8_t>;
    //! Load Type (Description + ID)
    using LoadType =
      std::optional< std::pair< std::string, uint16_t>>;

    //! Offset of the Part Flags Field (ARINC 665-3) - Spare in older supplements
    static constexpr std::size_t PartFlagsFieldOffset = 6U;

    //! Offset of the Load Part Number Pointer Field
    static constexpr std::size_t LoadPartNumberPointerFieldOffset = 8U;

    //! Offset of the THW IDs Pointer Field
    static constexpr std::size_t ThwIdsPointerFieldOffset = 12U;

    //! Offset of the Data Files Pointer Field
    static constexpr std::size_t DataFilesPointerFieldOffset = 16U;

    //! Offset of the Support Files Pointer Field
    static constexpr std::size_t SupportFilesPointerFieldOffset = 20U;

    //! Offset of the User Defined Data Pointer Field
    static constexpr std::size_t UserDefinedDataPointerFieldOffset = 24U;

    //! Offset of the Load Type Description Pointer Field (ARINC 665-3)
    static constexpr std::size_t LoadTypeDescriptionPointerFieldOffset = 28U;

    //! Offset of the THW IDs with Positions Pointer Field (ARINC 665-3)
    static constexpr std::size_t ThwIdPositionsPointerFieldOffset = 32U;

    //! Offset of the Load Check Value Pointer Field (ARINC 665-3)
    static constexpr std::size_t LoadCheckValuePointerFieldOffset = 36U;

    //! First Start of pointer data for ARINC 665-2 Load Headers.
    static constexpr std::size_t LoadHeaderSizeV2 = 28U;

    //! First Start of pointer data for ARINC 665-3/4 Load Headers.
    static constexpr std::size_t LoadHeaderSizeV3 = 40U;

    //! Position of Load CRC from end of File
    static constexpr std::size_t LoadCrcOffset = 4U;

    //! Position of File CRC from end of File
    static constexpr std::size_t FileCrcOffset = 6U;

    //! Download Flag of Part Flag
    static constexpr std::uint16_t PartFlagDownload = 0x0001U;

    /**
     * @brief Creates an empty load header file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    LoadHeaderFile( SupportedArinc665Version version);

    /**
     * @brief Creates load header file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] partNumber
     *   Load Part Number
     * @param[in] targetHardwareIdPositions
     *   Target Hardware ID / Positions
     * @param[in] dataFilesInfo
     *   Data Files
     * @param[in] supportFilesInfo
     *   Support Files
     * @param[in] userDefinedData
     *   User Defined Data
     * @param[in] loadCrc
     *   Load CRC
     **/
    LoadHeaderFile(
      SupportedArinc665Version version,
      const std::string &partNumber,
      const TargetHardwareIdPositions &targetHardwareIdPositions,
      const LoadFilesInfo &dataFilesInfo,
      const LoadFilesInfo &supportFilesInfo,
      const UserDefinedData &userDefinedData,
      uint32_t loadCrc);

    //! @copydoc LoadHeaderFile(SupportedArinc665Version,const std::string&,const TargetHardwareIdPositions&,const LoadFilesInfo&,const LoadFilesInfo&,const UserDefinedData&,uint32_t)
    LoadHeaderFile(
      SupportedArinc665Version version,
      std::string &&partNumber,
      TargetHardwareIdPositions &&targetHardwareIdPositions,
      LoadFilesInfo &&dataFilesInfo,
      LoadFilesInfo &&supportFilesInfo,
      UserDefinedData &&userDefinedData,
      uint32_t loadCrc);

    /**
     * @brief Creates a load header file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit LoadHeaderFile( const RawFile &rawFile);

    //! @copydoc Arinc665File::operator=
    LoadHeaderFile& operator=( const RawFile &rawFile) final;

    /**
     * @brief Returns the part flags.
     *
     * The part flags exists since ARINC 665-3.
     * It is only encoded/ decoded within such protocol files.
     *
     * @return The part flags.
     **/
    uint16_t partFlags() const;

    /**
     * @brief Updates the part flags.
     *
     * @param[in] partFlags
     *   The new Part Flags.
     **/
    void partFlags( uint16_t partFlags);

    /**
     * @brief Returns the part number of the load header file.
     *
     * @return The part number of the load header file.
     **/
    std::string partNumber() const;

    /**
     * @brief Updates the part number of the load header file.
     *
     * @param[in] partNumber
     *   The new Part Number.
     **/
    void partNumber( const std::string &partNumber);

    //! @copydoc partNumber(const std::string&)
    void partNumber( std::string &&partNumber);

    /**
     * @brief Returns the Target Hardware ID/ Positions.
     *
     * @return The Target Hardware IDs
     **/
    const TargetHardwareIdPositions& targetHardwareIdPositions() const;

    //! @copydoc targetHardwareIdPositions() const
    TargetHardwareIdPositions& targetHardwareIdPositions();

    /**
     * @brief Sets the Target Hardware ID/ Positions.
     *
     * @param[in] targetHardwareIdPositions
     *   Target Hardware ID/ Positions.
     **/
    void targetHardwareIdPositions(
      const TargetHardwareIdPositions &targetHardwareIdPositions);

    //! @copydoc targetHardwareIdPositions(const TargetHardwareIdPositions&)
    void targetHardwareIdPositions(
      TargetHardwareIdPositions &&targetHardwareIdPositions);

    /**
     * @brief Returns only the Target Hardware IDs (without position information)
     *
     * @return The Target Hardware IDs.
     **/
    StringList targetHardwareIds() const;

    /**
     * @brief Add Target Hardware IDs without position information.
     *
     * @param[in] targetHardwareIds
     *   Target Hardware IDs.
     **/
    void targetHardwareIds( const StringList& targetHardwareIds);

    /**
     * @brief Add Target Hardware ID/ Positions.
     *
     * @param[in] targetHardwareId
     *   Target Hardware ID.
     * @param[in] positions
     *   Positions (can be empty)
     **/
    void targetHardwareId(
      const std::string &targetHardwareId,
      const StringList &positions = {});

    //! @copydoc targetHardwareId(const std::string&,const StringList&)
    void targetHardwareId(
      std::string &&targetHardwareId,
      StringList &&positions = {});

    /**
     * @brief Returns the Load Type information.
     *
     * This information is only used in ARIN 665-3/4 files.
     *
     * @return The Load Type information.
     **/
    const LoadType& loadType() const;

    /**
     * @brief Updates the Load Type information.
     *
     * This information is only used in ARIN 665-3/4 files.
     *
     * @param[in] type
     *   The Load Type information.
     **/
    void loadType( const LoadType &type);

    //! @copydoc loadType(const LoadType&)
    void loadType( LoadType &&type);

    /**
     * @brief Return the data files information.
     *
     * @return The data files information.
     **/
    const LoadFilesInfo& dataFiles() const;

    //! @copydoc dataFiles() const
    LoadFilesInfo& dataFiles();

    /**
     * @brief Add data file.
     *
     * @param[in] dataFileInfo
     *   The data file information.
     **/
    void dataFile( const LoadFileInfo &dataFileInfo);

    //! @copydoc dataFile(const LoadFileInfo&)
    void dataFile( LoadFileInfo &&dataFileInfo);

    /**
     * @brief Return the support files information.
     *
     * @return The support files information.
     **/
    const LoadFilesInfo& supportFiles() const;

    //! @copydoc supportFiles() const
    LoadFilesInfo& supportFiles();

    /**
     * @brief Add support file.
     *
     * @param[in] supportFileInfo
     *   The support file information.
     **/
    void supportFile( const LoadFileInfo &supportFileInfo);

    //! @copydoc supportFile(const LoadFileInfo&)
    void supportFile( LoadFileInfo &&supportFileInfo);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the user defined data.
     *
     * @param[in] userDefinedData
     *   The user defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

    /**
     * @brief Returns the Load CRC.
     *
     * @return The Load CRC
     **/
    uint32_t loadCrc() const;

    /**
     * @brief Updates the Load CRC.
     *
     * @param[in] loadCrc
     *   The Load CRC
     **/
    void loadCrc( uint32_t loadCrc);

    /**
     * @brief Returns the Load Check Value.
     *
     * @return The Load Check Value.
     **/
    const std::optional< CheckValue>& loadCheckValue() const;

    /**
     * @brief Updates the Load Check Value
     *
     * @param[in] value
     *   Load Check Value.
     **/
    void loadCheckValue( const std::optional< CheckValue> &value);

    //! @copydoc loadCheckValue(const std::optional<CheckValue>&)
    void loadCheckValue( std::optional< CheckValue> &&value);

  private:
    //! File List Type
    enum class FileListType
    {
      Data,
      Support
    };

    //! @copydoc Arinc665File::encode
    RawFile encode() const final;

    /**
     * @brief Decodes the body of the load header file.
     *
     * @param[in] rawFile
     *   Raw Load Header File representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the files information list.
     *
     * @todo split between data and support files - the format differs in later
     * ARINC 665 supplements.
     *
     * @param[in] loadFilesInfo
     *   The load files information to encode.
     * @param[in] type
     *   The file list type (data or support files)
     * @param[in] encodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is encoded.
     *
     * @return Raw representation of files information list.
     **/
    RawFile encodeFileList(
      const LoadFilesInfo &loadFilesInfo,
      FileListType type,
      bool encodeV3Data) const;

    /**
     * @brief Decodes the files information list from the raw data.
     *
     * @todo split between data and support files - the format differs in later
     * ARINC 665 supplements.
     *
     * @param[in] rawFile
     *   Raw Load Header File representation.
     * @param[in] offset
     *   Offset of the files information list.
     * @param[in] type
     *   The file list type (data or support files)
     * @param[in] decodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is decoded.
     *
     * @return The decoded load files information.
     **/
    LoadFilesInfo decodeFileList(
      const RawFile &rawFile,
      std::size_t offset,
      FileListType type,
      bool decodeV3Data);

    //! Part Flags
    uint16_t partFlagsValue;
    //! Part number of the load
    std::string partNumberValue;
    //! List of compatible Target Hardware ID/ Positions
    TargetHardwareIdPositions targetHardwareIdPositionsValue;
    //! Load Type
    LoadType typeValue;
    //! List of data files
    LoadFilesInfo dataFilesValue;
    //! List of Support files
    LoadFilesInfo supportFilesValue;
    //! User defined data
    UserDefinedData userDefinedDataValue;
    //! CRC of the complete load
    uint32_t loadCrcValue;
    //! The Load Check Value (since ARINC 665-3)
    std::optional< CheckValue> loadCheckValueValue;
};

}

#endif

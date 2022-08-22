/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::LoadHeaderFile.
 **/

#ifndef ARINC665_FILES_LOADHEADERFILE_HPP
#define ARINC665_FILES_LOADHEADERFILE_HPP

#include <arinc665/files/Files.hpp>
#include <arinc665/files/Arinc665File.hpp>
#include <arinc665/files/LoadFileInfo.hpp>

#include <arinc645/Arinc645.hpp>

#include <string_view>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <optional>
#include <cstdint>

namespace Arinc665::Files {

/**
 * @brief ARINC 665 Load Header %File (*.LUH).
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
class ARINC665_EXPORT LoadHeaderFile : public Arinc665File
{
  public:
    //! Positions
    using Positions = std::set< std::string, std::less<> >;
    //! Target Hardware ID / Positions
    using TargetHardwareIdPositions =
      std::map< std::string, Positions, std::less<> >;
    //! Target Hardware IDs
    using TargetHardwareIds = std::set< std::string, std::less<> >;
    //! User Defined Data
    using UserDefinedData = std::vector< uint8_t>;
    //! Load Type (Description + ID)
    using LoadType =
      std::optional< std::pair< std::string, uint16_t > >;

    //! Offset of the Spare Field (since ARINC 665-2)
    static constexpr ptrdiff_t SpareFieldOffsetV2{ 6 };

    //! Offset of the Part Flags Field (since ARINC 665-3) - Spare in older supplements
    static constexpr ptrdiff_t PartFlagsFieldOffsetV3{ 6 };

    //! Offset of the Load Part Number Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t LoadPartNumberPointerFieldOffsetV1{ 6 };

    //! Offset of the Load Part Number Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t LoadPartNumberPointerFieldOffsetV2{ 8 };

    //! Offset of the THW IDs Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t ThwIdsPointerFieldOffsetV1{ 10 };

    //! Offset of the THW IDs Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t ThwIdsPointerFieldOffsetV2{ 12 };

    //! Offset of the Data Files Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t DataFilesPointerFieldOffsetV1{ 14 };

    //! Offset of the Data Files Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t DataFilesPointerFieldOffsetV2{ 16 };

    //! Offset of the Support Files Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t SupportFilesPointerFieldOffsetV1{ 18 };

    //! Offset of the Support Files Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t SupportFilesPointerFieldOffsetV2{ 20 };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-1)
    static constexpr ptrdiff_t UserDefinedDataPointerFieldOffsetV1{ 22 };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-2)
    static constexpr ptrdiff_t UserDefinedDataPointerFieldOffsetV2{ 24 };

    //! Offset of the Load Type Description Pointer Field (since ARINC 665-3)
    static constexpr ptrdiff_t LoadTypeDescriptionPointerFieldOffsetV3{ 28 };

    //! Offset of the THW IDs with Positions Pointer Field (since ARINC 665-3)
    static constexpr ptrdiff_t ThwIdPositionsPointerFieldOffsetV3{ 32 };

    //! Offset of the Load Check Value Pointer Field (since ARINC 665-3)
    static constexpr ptrdiff_t LoadCheckValuePointerFieldOffsetV3{ 36 };

    //! First Start of pointer data for ARINC 665-1 Load Headers.
    static constexpr ptrdiff_t LoadHeaderSizeV1{ 26 };

    //! First Start of pointer data for ARINC 665-2 Load Headers.
    static constexpr ptrdiff_t LoadHeaderSizeV2{ 28 };

    //! First Start of pointer data for ARINC 665-3/4 Load Headers.
    static constexpr ptrdiff_t LoadHeaderSizeV3{ 40 };

    //! Position of Load CRC from end of File
    static constexpr ptrdiff_t LoadCrcOffset{ 4 };

    //! Position of File CRC from end of File
    static constexpr ptrdiff_t FileCrcOffset{ 6 };

    //! Download Flag of Part Flag
    static constexpr std::uint16_t PartFlagDownload{ 0x0001U };

    /**
     * @brief Creates an empty load header file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    explicit LoadHeaderFile( SupportedArinc665Version version );

    /**
     * @brief Creates a load header file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit LoadHeaderFile( const ConstRawFileSpan &rawFile );

    //! @copydoc Arinc665File::operator=
    LoadHeaderFile& operator=( const ConstRawFileSpan &rawFile ) final;

    //! @copydoc Arinc665File::fileType() const noexcept
    [[nodiscard]] FileType fileType() const noexcept final;

    /**
     * @name Load Part Flags
     * @{
     **/

    /**
     * @brief Returns the Part Flags.
     *
     * The part flags exists since ARINC 665-3.
     * It is only encoded/ decoded within such protocol files.
     *
     * @return Part Flags.
     **/
    [[nodiscard]] uint16_t partFlags() const noexcept;

    /**
     * @brief Updates the Part Flags.
     *
     * @param[in] partFlags
     *   New Part Flags.
     **/
    void partFlags( uint16_t partFlags ) noexcept;

    /** @} **/

    /**
     * @name Load Part Number
     * @{
     **/

    /**
     * @brief Returns the Part Number of the load header file.
     *
     * @return Part Number of the load header file.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Part Number of the load header file.
     *
     * @param[in] partNumber
     *   New Part Number.
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber );

    /** @} **/

    /**
     * @name Target Hardware IDs with Positions
     * @sa TargetHardwareIdPositions
     * @{
     **/

    /**
     * @brief Returns the Target Hardware ID/ Positions.
     *
     * @return The Target Hardware IDs
     **/
    [[nodiscard]] const TargetHardwareIdPositions&
    targetHardwareIdPositions() const;

    //! @copydoc targetHardwareIdPositions() const
    TargetHardwareIdPositions& targetHardwareIdPositions();

    /**
     * @brief Sets the Target Hardware ID/ Positions.
     *
     * @param[in] targetHardwareIdPositions
     *   Target Hardware ID/ Positions.
     **/
    void targetHardwareIdPositions(
      const TargetHardwareIdPositions &targetHardwareIdPositions );

    //! @copydoc targetHardwareIdPositions(const TargetHardwareIdPositions&)
    void targetHardwareIdPositions(
      TargetHardwareIdPositions &&targetHardwareIdPositions );

    /** @} **/

    /**
     * @name Target Hardware IDs
     * @{
     **/

    /**
     * @brief Returns only the Target Hardware IDs (without position information)
     *
     * @return Target Hardware IDs.
     **/
    [[nodiscard]] TargetHardwareIds targetHardwareIds() const;

    /**
     * @brief Add Target Hardware IDs without position information.
     *
     * @param[in] targetHardwareIds
     *   Target Hardware IDs.
     **/
    void targetHardwareIds( const TargetHardwareIds& targetHardwareIds );

    /**
     * @brief Add Target Hardware ID/ Positions.
     *
     * @param[in] targetHardwareId
     *   Target Hardware ID.
     * @param[in] positions
     *   Positions (can be empty)
     **/
    void targetHardwareId(
      std::string_view targetHardwareId,
      const Positions &positions = {} );

    //! @copydoc targetHardwareId(std::string_view,const Positions&)
    void targetHardwareId(
      std::string &&targetHardwareId,
      Positions &&positions = {} );

    /** @} **/

    /**
     * @name Load Type
     * @sa LoadType
     * @{
     **/

    /**
     * @brief Returns the Load Type Information.
     *
     * This information is only used in ARIN 665-3/4 files.
     *
     * @return Load Type information.
     **/
    [[nodiscard]] const LoadType& loadType() const;

    /**
     * @brief Updates the Load Type Information.
     *
     * This information is only used in ARIN 665-3/4 files.
     *
     * @param[in] type
     *   Load Type Information.
     **/
    void loadType( const LoadType &type );

    //! @copydoc loadType(const LoadType&)
    void loadType( LoadType &&type );

    /** @} **/

    /**
     * @name Data Files
     * @sa LoadFilesInfo, LoadFileInfo
     * @{
     **/

    /**
     * @brief Return the Data Files.
     *
     * @return Data files information.
     **/
    [[nodiscard]] const LoadFilesInfo& dataFiles() const noexcept;

    //! @copydoc dataFiles() const
    LoadFilesInfo& dataFiles() noexcept;

    /**
     * @brief Add Data %File.
     *
     * @param[in] dataFileInfo
     *   Data file information.
     **/
    void dataFile( const LoadFileInfo &dataFileInfo );

    //! @copydoc dataFile(const LoadFileInfo&)
    void dataFile( LoadFileInfo &&dataFileInfo );

    /** @} **/

    /**
     * @name Support Files
     * @sa LoadFilesInfo, LoadFileInfo
     * @{
     **/

    /**
     * @brief Return the Support Files.
     *
     * @return Support Files.
     **/
    [[nodiscard]] const LoadFilesInfo& supportFiles() const noexcept;

    //! @copydoc supportFiles() const
    LoadFilesInfo& supportFiles() noexcept;

    /**
     * @brief Add Support %File.
     *
     * @param[in] supportFileInfo
     *   Support File.
     **/
    void supportFile( const LoadFileInfo &supportFileInfo );

    //! @copydoc supportFile(const LoadFileInfo&)
    void supportFile( LoadFileInfo &&supportFileInfo );

    /** @} **/

    /**
     * @name User Defined Data
     * @{
     **/

    /**
     * @brief Returns the User Defined Data.
     *
     * @return User defined data.
     **/
    [[nodiscard]] const UserDefinedData& userDefinedData() const;

    /**
     * @brief Updates the User Defined Data.
     *
     * @param[in] userDefinedData
     *   User defined data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData );

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData );

    /** @} **/

    /**
     * @name Load CRC
     * @{
     **/

    /**
     * @brief Returns the Load CRC.
     *
     * @return Load CRC
     **/
    [[nodiscard]] uint32_t loadCrc() const noexcept;

    /**
     * @brief Updates the Load CRC.
     *
     * @param[in] loadCrc
     *   Load CRC
     **/
    void loadCrc( uint32_t loadCrc ) noexcept;

    /** @} **/

    /**
     * @name Load Check Value
     *
     * Check Value for the whole Load.
     * If not used set to @ref Arinc645::NoCheckValue.
     *
     * @sa CheckValue
     * @{
     **/

    /**
     * @brief Returns the Load Check Value.
     *
     * @return Load Check Value.
     **/
    [[nodiscard]] const Arinc645::CheckValue& loadCheckValue() const;

    /**
     * @brief Updates the Load Check Value
     *
     * @param[in] value
     *   Load Check Value.
     **/
    void loadCheckValue( const Arinc645::CheckValue &value );

    //! @copydoc loadCheckValue(const Arinc645::CheckValue&)
    void loadCheckValue( Arinc645::CheckValue &&value );

    /** @} **/

  private:
    //! @copydoc Arinc665File::encode
    [[nodiscard]] RawFile encode() const final;

    /**
     * @brief Decodes the body of the load header file.
     *
     * @param[in] rawFile
     *   Raw Load Header File representation.
     **/
    void decodeBody( const ConstRawFileSpan &rawFile );

    /**
     * @brief Encodes the Data Files Information List.
     *
     * @param[in] encodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is encoded.
     *
     * @return Raw representation of files information list.
     **/
    [[nodiscard]] RawFile encodeDataFiles( bool encodeV3Data ) const;

    /**
     * @brief Encodes the Support Files Information List.
     *
     * @param[in] encodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is encoded.
     *
     * @return Raw representation of files information list.
     **/
    [[nodiscard]] RawFile encodeSupportFiles( bool encodeV3Data ) const;

    /**
     * @brief Decodes the Data Files List from the raw data.
     *
     * @param[in] rawFile
     *   Raw Load Header File representation.
     * @param[in] decodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is decoded.
     **/
    void decodeDataFiles( const ConstRawFileSpan &rawFile, bool decodeV3Data );

    /**
     * @brief Decodes the Support Files List from the raw data.
     *
     * @param[in] rawFile
     *   Raw Load Header File representation.
     * @param[in] decodeV3Data
     *   If set to true, additional data as stated in ARINC 665-3 is decoded.
     **/
    void decodeSupportFiles(
      const ConstRawFileSpan &rawFile,
      bool decodeV3Data );

    /**
     * @brief Checks, if the User Defined Data is a multiple of 2 size.
     **/
    void checkUserDefinedData();

    //! Part Flags
    uint16_t partFlagsV{ 0U };
    //! Part Number of the Load
    std::string partNumberV;
    //! List of compatible Target Hardware ID/ Positions
    TargetHardwareIdPositions targetHardwareIdPositionsV;
    //! Load Type
    LoadType typeV;
    //! List of Data Files
    LoadFilesInfo dataFilesV;
    //! List of Support files
    LoadFilesInfo supportFilesV;
    //! User Defined Data
    UserDefinedData userDefinedDataV;
    //! CRC of the Complete Load
    uint32_t loadCrcV{ 0U };
    //! Load Check Value (since ARINC 665-3)
    Arinc645::CheckValue loadCheckValueV{ Arinc645::NoCheckValue };
};

}

#endif

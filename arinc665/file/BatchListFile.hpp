/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchListFile.
 **/

#ifndef ARINC665_BATCHLISTFILE_HPP
#define ARINC665_BATCHLISTFILE_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/ListFile.hpp>
#include <arinc665/file/BatchInfo.hpp>

#include <list>
#include <vector>
#include <map>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch List File (BATCHES.LUM).
 *
 * @par File Format (ARINC 665-4)
 * Name of Field                   | Field Size (bits)
 * --------------------------------|:----------------:
 * BATCHES.LUM File Length         | 32
 * Media File Format Version       | 16
 * Spare                           | 16
 * Pointer to Media Set PN Length  | 32
 * Pointer to Number of Batches    | 32
 * Pointer to User Defined Data    | 32
 * Expansion Point No. 1           |  0
 * Media Set PN Length             | 16
 * Media Set PN                    | 16
 * Media Sequence Number (X)       |  8
 * Number of Media Set Members (Y) |  8
 * Number of Batches               | 16
 * + Batch Pointer                 | 16
 * + Batch PN Length               | 16
 * + Batch PN                      | 16
 * + Batch File Name Length        | 16
 * + Batch File Name               | 16
 * + Member Sequence Number        | 16
 * + Expansion Point No. 2         |  0
 * Expansion Point No. 3           |  0
 * User Defined Data               | 16
 * BATCHES.LUM File CRC            | 16
 **/
class BatchListFile: public ListFile
{
  public:
    //! Batch Information Map
    using BatchInfoMap = std::map< std::pair< uint8_t, std::string>, BatchInfo>;
    //! User Defined Data.
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Spare field.
    static constexpr std::size_t SpareFieldOffset = 6U;

    //! Offset of the Media Set Part Number Pointer Field
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffset = 8U;

    //! Offset of the Batches Pointer Field
    static constexpr std::size_t BatchFilesPointerFieldOffset = 12U;

    //! Offset of the User Defined Data Pointer Field
    static constexpr std::size_t UserDefinedDataPointerFieldOffset = 16U;

    //! First Start of pointer data for ARINC 665 Batch List Files.
    static constexpr std::size_t FileHeaderSize = 20U;

    /**
     * @brief Creates an empty batch list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    explicit BatchListFile( SupportedArinc665Version version) noexcept;

    /**
     * @brief Creates batch list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] mediaSetPn
     *   Media Set Part Number.
     * @param[in] mediaSequenceNumber
     *   Media Set sequnce number.
     * @param[in] numberOfMediaSetMembers
     *   Number of Media Set members.
     * @param[in] batches
     *   Batches information.
     * @param[in] userDefinedData
     *   Additional user defined data.
     **/
    BatchListFile(
      SupportedArinc665Version version,
      std::string_view mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const BatchesInfo &batches,
      const UserDefinedData &userDefinedData);

    //! @copydoc BatchListFile(SupportedArinc665Version,std::string_view,uint8_t,uint8_t,const BatchesInfo&,const UserDefinedData&)
    BatchListFile(
      SupportedArinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      BatchesInfo &&batches,
      UserDefinedData &&userDefinedData);

    /**
     * @brief Creates a batch list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit BatchListFile( const RawFile &rawFile);

    //! @copydoc ListFile::operator=(const RawFile&)
    BatchListFile& operator=( const RawFile &rawFile) final;

    //! @copydoc ListFile::fileType() const noexcept
    [[nodiscard]] FileType fileType() const noexcept override;

    //! @copydoc ListFile::mediaSetPn() const
    [[nodiscard]] std::string_view mediaSetPn() const final;

    //! @copydoc ListFile::mediaSetPn(std::string_view)
    void mediaSetPn( std::string_view mediaSetPn) final;

    //! @copydoc ListFile::mediaSetPn(std::string&&)
    void mediaSetPn( std::string &&mediaSetPn) final;

    //! @copydoc ListFile::mediaSequenceNumber() const
    [[nodiscard]] uint8_t mediaSequenceNumber() const final;

    //! @copydoc ListFile::mediaSequenceNumber(uint8_t)
    void mediaSequenceNumber( uint8_t mediaSequenceNumber) final;

    //! @copydoc ListFile::numberOfMediaSetMembers() const
    [[nodiscard]] uint8_t numberOfMediaSetMembers() const final;

    //! @copydoc ListFile::numberOfMediaSetMembers(uint8_t)
    void numberOfMediaSetMembers( uint8_t numberOfMediaSetMembers) final;

    /**
     * @brief Return the number of batches.
     *
     * @return The number of batches.
     **/
    [[nodiscard]] size_t numberOfBatches() const;

    /**
     * @brief Returns the batches information.
     *
     * @return The batches information.
     **/
    [[nodiscard]] const BatchesInfo& batches() const;

    //! @copydoc batches() const
    BatchesInfo& batches();

    /**
     * @brief Returns the batches information as map.
     *
     * @return The batches information as map.
     */
    [[nodiscard]] BatchInfoMap batchesAsMap() const;

    /**
     * @brief Adds the given batch information.
     *
     * @param[in] batch
     *   The batch information to add.
     **/
    void batch( const BatchInfo &batch);

    //! @copydoc batch(const BatchInfo&)
    void batch( BatchInfo &&batch);

    /**
     * @brief Returns the user defined data.
     *
     * @return The user defined data.
     **/
    [[nodiscard]] const UserDefinedData& userDefinedData() const;

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
     * @brief Returns if the given batch list file belongs to the same media set.
     *
     * @param[in] other
     *   The other batch list file to compare to this.
     *
     * @return If the given batch list file belongs to the same media set.
     **/
    [[nodiscard]] bool belongsToSameMediaSet( const BatchListFile &other) const;

  private:
    //! @copydoc ListFile::encode
    [[nodiscard]] RawFile encode() const final;

    /**
     * @brief Decodes the body of the batch list file.
     *
     * @param[in] rawFile
     *   Raw batch list file representation.
     **/
    void decodeBody( const RawFile &rawFile);

    /**
     * @brief Encodes the batches information list.
     *
     * @return Raw representation of batches information list.
     **/
    [[nodiscard]] RawFile encodeBatchesInfo() const;

    /**
     * @brief Decodes the batches information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw batch list file representation.
     * @param[in] offset
     *   Offset of the batches information list.
     **/
    void decodeBatchesInfo( const RawFile &rawFile, std::size_t offset);

    //! Media Set Part Number
    std::string mediaSetPnValue;
    //! Media Sequence Number
    uint8_t mediaSequenceNumberValue;
    //! Number of Media Set Members
    uint8_t numberOfMediaSetMembersValue;
    //! Batches Information
    BatchesInfo batchesValue;
    //! User Defined Data
    UserDefinedData userDefinedDataValue;
};

}

#endif

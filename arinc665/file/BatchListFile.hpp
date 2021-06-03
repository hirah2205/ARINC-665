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

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch List %File (BATCHES.LUM).
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
    //! User Defined Data.
    using UserDefinedData = std::vector< uint8_t>;

    //! Offset of the Spare field (since ARINC 665-2).
    static constexpr std::size_t SpareFieldOffsetV2{ 6U };

    //! Offset of the Media Set Part Number Pointer Field (since ARINC 665-1)
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffsetV1{ 6U };

    //! Offset of the Media Set Part Number Pointer Field (since ARINC 665-2)
    static constexpr std::size_t MediaSetPartNumberPointerFieldOffsetV2{ 8U };

    //! Offset of the Batches Pointer Field (since ARINC 665-1)
    static constexpr std::size_t BatchFilesPointerFieldOffsetV1{ 10U };

    //! Offset of the Batches Pointer Field (since ARINC 665-2)
    static constexpr std::size_t BatchFilesPointerFieldOffsetV2{ 12U };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-1)
    static constexpr std::size_t UserDefinedDataPointerFieldOffsetV1{ 14U };

    //! Offset of the User Defined Data Pointer Field (since ARINC 665-2)
    static constexpr std::size_t UserDefinedDataPointerFieldOffsetV2{ 16U };

    //! First Start of pointer data for ARINC 665 Batch List Files (since ARINC 665-1).
    static constexpr std::size_t FileHeaderSizeV1{ 18U };

    //! First Start of pointer data for ARINC 665 Batch List Files (since ARINC 665-2).
    static constexpr std::size_t FileHeaderSizeV2{ 20U };

    /**
     * @brief Creates an empty batch list file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    explicit BatchListFile( SupportedArinc665Version version ) noexcept;

    /**
     * @brief Creates batch list file with the given data.
     *
     * @param[in] version
     *   ARINC 665 Version.
     * @param[in] mediaSetPn
     *   Media Set Part Number.
     * @param[in] mediaSequenceNumber
     *   Media Set Sequence Number.
     * @param[in] numberOfMediaSetMembers
     *   Number of Media Set Members.
     * @param[in] batches
     *   Batches Information.
     * @param[in] userDefinedData
     *   Additional User Defined Data.
     **/
    BatchListFile(
      SupportedArinc665Version version,
      std::string_view mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      const BatchesInfo &batches,
      const UserDefinedData &userDefinedData );

    //! @copydoc BatchListFile(SupportedArinc665Version,std::string_view,uint8_t,uint8_t,const BatchesInfo&,const UserDefinedData&)
    BatchListFile(
      SupportedArinc665Version version,
      std::string &&mediaSetPn,
      uint8_t mediaSequenceNumber,
      uint8_t numberOfMediaSetMembers,
      BatchesInfo &&batches,
      UserDefinedData &&userDefinedData );

    /**
     * @brief Creates a batch list file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit BatchListFile( const ConstRawFileSpan &rawFile );

    //! @copydoc ListFile::operator=(const ConstRawFileSpan&)
    BatchListFile& operator=( const ConstRawFileSpan &rawFile ) final;

    //! @copydoc ListFile::fileType() const noexcept
    [[nodiscard]] FileType fileType() const noexcept override;

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
     * @brief Return the Number of Batches.
     *
     * @return Number of batches.
     **/
    [[nodiscard]] size_t numberOfBatches() const;

    /**
     * @brief Returns the Batches Information.
     *
     * @return Batches Information.
     **/
    [[nodiscard]] const BatchesInfo& batches() const;

    //! @copydoc batches() const
    BatchesInfo& batches();

    /**
     * @brief Adds the given Batch Information.
     *
     * @param[in] batch
     *   Batch Information.
     **/
    void batch( const BatchInfo &batch);

    //! @copydoc batch(const BatchInfo&)
    void batch( BatchInfo &&batch);

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
     *   User Defined Data.
     **/
    void userDefinedData( const UserDefinedData &userDefinedData);

    //! @copydoc userDefinedData(const UserDefinedData&)
    void userDefinedData( UserDefinedData &&userDefinedData);

    /**
     * @brief Returns if the given batch list file belongs to the same media set.
     *
     * @param[in] other
     *   Other batch list file to compare to this.
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
    void decodeBody( const ConstRawFileSpan &rawFile);

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
    void decodeBatchesInfo(
      const ConstRawFileSpan &rawFile,
      std::size_t offset );

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
    //! Batches Information
    BatchesInfo batchesV;
    //! User Defined Data
    UserDefinedData userDefinedDataV;
};

}

#endif

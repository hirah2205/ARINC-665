/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::BatchListFile.
 **/

#ifndef ARINC665_FILES_BATCHLISTFILE_HPP
#define ARINC665_FILES_BATCHLISTFILE_HPP

#include <arinc665/files/Files.hpp>
#include <arinc665/files/ListFile.hpp>
#include <arinc665/files/BatchInfo.hpp>

#include <list>
#include <vector>

namespace Arinc665::Files {

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
class ARINC665_EXPORT BatchListFile : public ListFile
{
  public:
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
    explicit BatchListFile(
      SupportedArinc665Version version =
        SupportedArinc665Version::Supplement345 ) noexcept;

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
    [[nodiscard]] FileType fileType() const noexcept final;

    /**
     * @name Batches
     * @{
     **/

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
    void batch( const BatchInfo &batch ) ;

    //! @copydoc batch(const BatchInfo&)
    void batch( BatchInfo &&batch );

    /** @} **/

    /**
     * @name User Defined Data
     * @{
     **/

    /**
     * @brief Returns the User Defined Data.
     *
     * @return User Defined Data.
     **/
    [[nodiscard]] ConstUserDefinedDataSpan userDefinedData() const;

    /**
     * @brief Updates the User Defined Data.
     *
     * @param[in] userDefinedData
     *   User Defined Data.
     **/
    void userDefinedData( ConstUserDefinedDataSpan userDefinedData );

    //! @copydoc userDefinedData(ConstUserDefinedDataSpan)
    void userDefinedData( UserDefinedData &&userDefinedData );

    /** @} **/

    /**
     * @brief Returns if the given batch list file belongs to the same media set.
     *
     * @param[in] other
     *   Other batch list file to compare to this.
     *
     * @return If the given batch list file belongs to the same media set.
     **/
    [[nodiscard]] bool belongsToSameMediaSet(
      const BatchListFile &other ) const;

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

    //! Batches Information
    BatchesInfo batchesV;
    //! User Defined Data
    UserDefinedData userDefinedDataV;
};

}

#endif

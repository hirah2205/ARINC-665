/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchFile.
 **/

#ifndef ARINC665_FILE_BATCH_HPP
#define ARINC665_FILE_BATCH_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/Arinc665File.hpp>
#include <arinc665/file/BatchTargetInfo.hpp>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch %File (*.LUB).
 *
 * @par File Format (ARINC 665-4)
 * Name of Field                                      | Field Size (bits)
 * ---------------------------------------------------|:-----------------:
 * Batch File Length                                  | 32
 * Batch File Format Version                          | 16
 * Spare                                              | 16
 * Pointer to Batch File PN Length                    | 32
 * Pointer to Number of Target HW ID Load-List Blocks | 32
 * Expansion Point 1                                  |  0
 * Batch File PN Length                               | 16
 * Batch File PN                                      | 16
 * Comment Length                                     | 16
 * Comment                                            | 16
 * Expansion Point 2                                  |  0
 * Number of Target HW ID Load-List Blocks            | 16
 * + Pointer to Next Target HW ID Load-List Block     | 16
 * + Target HW ID POS Length                          | 16
 * + Target HW ID POS                                 | 16
 * + Number of Loads for Target HW ID POS             | 16
 * +# Header File Name Length                         | 16
 * +# Header File Name                                | 16
 * +# Load PN Length                                  | 16
 * +# Load PN                                         | 16
 * Expansion Point 3                                  |  0
 * Batch File CRC                                     | 16
 **/
class BatchFile: public Arinc665File
{
  public:
    ///! Offset of the Spare field (since ARINC 665-2).
    static constexpr std::ptrdiff_t SpareFieldOffsetV2{ 6 };

    //! Offset of the Batch Part Number Pointer Field (since ARINC 665-1)
    static constexpr std::ptrdiff_t BatchPartNumberPointerFieldOffsetV1{ 6 };

    //! Offset of the Batch Part Number Pointer Field (since ARINC 665-2)
    static constexpr std::ptrdiff_t BatchPartNumberPointerFieldOffsetV2{ 8 };

    //! Offset of the THW IDs Pointer Field (since ARINC 665-1)
    static constexpr std::ptrdiff_t ThwIdsPointerFieldOffsetV1{ 10 };

    //! Offset of the THW IDs Pointer Field (since ARINC 665-2)
    static constexpr std::ptrdiff_t ThwIdsPointerFieldOffsetV2{ 12 };

    //! First Start of Pointer Data for ARINC 665-1 Batch Files.
    static constexpr std::ptrdiff_t BatchFileHeaderSizeV1{ 14 };

    //! First Start of Pointer Data for ARINC 665-2 Batch Files.
    static constexpr std::ptrdiff_t BatchFileHeaderSizeV2{ 16 };

    /**
     * @brief Creates an empty batch file.
     *
     * @param[in] version
     *   ARINC 665 version.
     **/
    explicit BatchFile( SupportedArinc665Version version );

    /**
     * @brief Creates batch file with the given data.
     *
     * @param[in] version
     *   ARINC 665 version.
     * @param[in] partNumber
     *   The part number.
     * @param[in] comment
     *   Batch comment.
     * @param[in] targets
     *   List of target information.
     **/
    BatchFile(
      SupportedArinc665Version version,
      std::string_view partNumber,
      std::string_view comment,
      const BatchTargetsInfo &targets );

    //! @copydoc BatchFile::BatchFile(SupportedArinc665Version,std::string_view,std::string_view,const BatchTargetsInfo&)
    BatchFile(
      SupportedArinc665Version version,
      std::string &&partNumber,
      std::string &&comment,
      BatchTargetsInfo &&targets );

    /**
     * @brief Creates a batch file from the given raw data.
     *
     * @param[in] rawFile
     *   Raw data file representation.
     **/
    explicit BatchFile( const ConstRawFileSpan &rawFile );

    //! @copydoc Arinc665File::operator=
    BatchFile& operator=( const ConstRawFileSpan &rawFile) final;

    //! @copydoc Arinc665File::fileType() const noexcept
    [[nodiscard]] FileType fileType() const noexcept final;

    /**
     * @brief Returns the part number of the batch file.
     *
     * @return The part number of the batch file.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the part number of the batch file.
     *
     * @param[in] partNumber
     *   The new part number.
     **/
    void partNumber( std::string_view partNumber);

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber);

    /**
     * @brief Returns the comment text of the batch file.
     *
     * @return The comment text of the batch file.
     **/
    [[nodiscard]] std::string_view comment() const;

    /**
     * @brief Updates the comment text of the batch file.
     *
     * @param[in] comment
     *   The new comment text.
     **/
    void comment( std::string_view comment);

    //! @copydoc comment(std::string_view)
    void comment( std::string &&comment);

    /**
     * @brief Returns the Targets Hardware Information.
     *
     * @return Target Hardware Information.
     **/
    [[nodiscard]] const BatchTargetsInfo& targetsHardware() const;

    //! @copydoc targetsHardware() const
    [[nodiscard]] BatchTargetsInfo& targetsHardware();

    /**
     * @brief Adds the given target hardware information to the batch file.
     *
     * @param[in] targetHardwareInfo
     *   The target hardware information to add.
     **/
    void targetHardware( const BatchTargetInfo &targetHardwareInfo );

    //! @copydoc targetHardware(const BatchTargetInfo&)
    void targetHardware( BatchTargetInfo &&targetHardwareInfo );

  private:
    //! @copydoc Arinc665File::encode
    [[nodiscard]] RawFile encode() const final;

    /**
     * @brief Decodes the body of the batch file.
     *
     * @param[in] rawFile
     *   Raw batch file representation.
     **/
    void decodeBody( const ConstRawFileSpan &rawFile);

    /**
     * @brief Encodes the target hardware information list.
     *
     * @return Raw representation of target hardware information list.
     **/
    [[nodiscard]] RawFile encodeBatchTargetsInfo() const;

    /**
     * @brief Decodes the target hardware information list from the raw data.
     *
     * @param[in] rawFile
     *   Raw batch file representation.
     * @param[in] offset
     *   Offset of the target hardware information list.
     **/
    void decodeBatchTargetsInfo(
      const ConstRawFileSpan &rawFile,
      ptrdiff_t offset );

    //! Part number
    std::string partNumberValue;
    //! Comment
    std::string commentValue;
    //! Targets Hardware Information
    BatchTargetsInfo targetsHardwareV;
};

}

#endif

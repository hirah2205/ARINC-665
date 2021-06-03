/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchInfo.
 **/

#ifndef ARINC665_FILE_BATCHINFO_HPP
#define ARINC665_FILE_BATCHINFO_HPP

#include <arinc665/file/File.hpp>

#include <vector>
#include <string>
#include <string_view>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch Information.
 *
 * Batch Information stored within the batch list of the ARINC 665 Batch List
 * File.
 *
 * @sa BatchListFile
 **/
class BatchInfo
{
  public:
    /**
     * @brief Initialises the batch information with te given data.
     *
     * @param[in] partNumber
     *   Part Number
     * @param[in] filename
     *   Filename
     * @param[in] memberSequenceNumber
     *   Member Sequence Number
     **/
    BatchInfo(
      std::string_view partNumber,
      std::string_view filename,
      uint16_t memberSequenceNumber );

    //! @copydoc BatchInfo::BatchInfo(std::string_view,std::string_view,uint16_t)
    BatchInfo(
      std::string &&partNumber,
      std::string &&filename,
      uint16_t memberSequenceNumber );

    /**
     * @brief Returns the Batch Part Number.
     *
     * @return The batch Batch part number
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the Batch Part Number.
     *
     * @param[in] partNumber
     *   Batch Part Number.
     **/
    void partNumber( std::string_view partNumber );

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber );

    /**
     * @brief Returns the Batch Filename.
     *
     * @return Batch Filename
     **/
    [[nodiscard]] std::string_view filename() const;

    /**
     * @brief Updates the Batch Filename.
     *
     * @param[in] filename
     *   Btch Filename.
     **/
    void filename( std::string_view filename );

    //! @copydoc filename(std::string_view)
    void filename( std::string &&filename );

    /**
     * @brief Returns the Member Sequence Number.
     *
     * @return Member sequence number
     **/
    [[nodiscard]] uint16_t memberSequenceNumber() const;

    /**
     * @brief Updates the Member Sequence Number.
     *
     * @param[in] memberSequenceNumber
     *   Member sequence number.
     **/
    void memberSequenceNumber( uint16_t memberSequenceNumber );

    /**
     * @brief Compares batch info against each other
     *
     * @param[in] other
     *   Right-hand side, which should be compared with @p this.
     *
     * @return @p this is equal to @p other.
     **/
    bool operator ==( const BatchInfo &other ) const;

    /**
     * @brief Compares batch info against each other to be unequal.
     *
     * @param[in] other
     *   Right-hand side, which should be compared with @p this.
     *
     * @return @p this is un-equal to @p other.
     **/
    bool operator !=( const BatchInfo &other ) const;

    /**
     * @brief Compares batch info against file info.
     *
     * @param[in] other
     *   Right-hand side, which should be compared with @p this.
     *
     * @return @p this is equal to @p other.
     **/
    bool operator ==( const FileInfo &other ) const;

    /**
     * @brief Compares batch info against file info to be unequal.
     *
     * @param[in] other
     *   Right-hand side, which should be compared with @p this.
     *
     * @return @p this is un-equal to @p other.
     **/
    bool operator !=( const FileInfo &other) const;

  private:
    //! Part Number
    std::string partNumberV;
    //! File Name
    std::string filenameV;
    //! Member Sequence Number
    uint16_t memberSequenceNumberV;
};

/**
 * @brief Compares batch info against file info.
 *
 * @param fileInfo
 *   File info, which should be compared with @p batchInfo.
 * @param batchInfo
 *   Batch info, which should be compared with @p fileInfo.
 *
 * @return @p fileInfo is equal to @p batchInfo.
 **/
bool operator==( const FileInfo &fileInfo, const BatchInfo &batchInfo );

/**
 * @brief Compares batch info against file info.
 *
 * @param fileInfo
 *   File info, which should be compared with @p batchInfo.
 * @param batchInfo
 *   Batch info, which should be compared with @p fileInfo.
 *
 * @return @p fileInfo is un-equal to @p batchInfo.
 **/
bool operator!=( const FileInfo &fileInfo, const BatchInfo &batchInfo );

}

#endif

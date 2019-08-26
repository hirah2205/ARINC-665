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

namespace Arinc665::File {

/**
 * @brief Batch Information stored within the batch list of the ARINC 665 Batch
 *   List File.
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
     *   Part number
     * @param[in] filename
     *   filename
     * @param[in] memberSequenceNumber
     *   member sequence number
     **/
    BatchInfo(
      std::string_view partNumber,
      std::string_view filename,
      uint16_t memberSequenceNumber);

    //! @copydoc BatchInfo::BatchInfo(std::string_view,std::string_view,uint16_t)
    BatchInfo(
      std::string &&partNumber,
      std::string &&filename,
      uint16_t memberSequenceNumber);

    /**
     * @brief Returns the Batch part number.
     *
     * @return The batch Batch part number
     **/
    std::string_view partNumber() const;

    /**
     * @brief Updates the Batch part number.
     *
     * @param[in] partNumber
     *   Batch part number.
     **/
    void partNumber( std::string_view partNumber);

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber);

    /**
     * @brief Returns the batch filename.
     *
     * @return The batch filename
     **/
    std::string_view filename() const;

    /**
     * @brief Updates the batch filename.
     *
     * @param[in] filename
     *   The batch filename.
     **/
    void filename( std::string_view filename);

    //! @copydoc filename(std::string_view)
    void filename( std::string &&filename);

    /**
     * @brief Returns the member sequence number.
     *
     * @return The member sequence number
     **/
    [[nodiscard]] uint16_t memberSequenceNumber() const;

    /**
     * @brief Updates the  member sequence number.
     *
     * @param[in] memberSequenceNumber
     *   The  member sequence number.
     **/
    void memberSequenceNumber( uint16_t memberSequenceNumber);

    /**
     * @brief Compares batch info against each other
     *
     * @param[in] other
     *   Right hand side, which should be compared with [this].
     *
     * @return If [this] is equal to [other].
     **/
    bool operator ==( const BatchInfo &other) const;

    /**
     * @brief Compares batch info against each other to be unequal.
     *
     * @param[in] other
     *   Right hand side, which should be compared with [this].
     *
     * @return If [this] is un-equal to [other].
     **/
    bool operator !=( const BatchInfo &other) const;

    /**
     * @brief Compares batch info against file info.
     *
     * @param[in] other
     *   Right hand side, which should be compared with [this].
     *
     * @return If [this] is equal to [other].
     **/
    bool operator ==( const FileInfo &other) const;

    /**
     * @brief Compares batch info against file info to be unequal.
     *
     * @param[in] other
     *   Right hand side, which should be compared with [this].
     *
     * @return If [this] is un-equal to [other].
     **/
    bool operator !=( const FileInfo &other) const;

  private:
    //! part number
    std::string partNumberValue;
    //! file name
    std::string filenameValue;
    //! member sequence number
    uint16_t memberSequenceNumberValue;
};

/**
 * @brief Compares batch info against file info.
 *
 * @param fileInfo
 *   File info, which should be compared with [batchInfo].
 * @param batchInfo
 *   Batch info, which should be compared with [fileInfo].
 *
 * @return If [fileInfo] is equal to [batchInfo].
 **/
bool operator==( const FileInfo &fileInfo, const BatchInfo &batchInfo);

/**
 * @brief Compares batch info against file info.
 *
 * @param fileInfo
 *   File info, which should be compared with [batchInfo].
 * @param batchInfo
 *   Batch info, which should be compared with [fileInfo].
 *
 * @return If [fileInfo] is un-equal to [batchInfo].
 **/
bool operator!=( const FileInfo &fileInfo, const BatchInfo &batchInfo);

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchLoadInfo.
 **/

#ifndef ARINC665_FILE_BATCHLOADINFO_HPP
#define ARINC665_FILE_BATCHLOADINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>
#include <string_view>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch Load Information.
 *
 * Load Information stored within the taraget list of the ARINC 665 Batch File.
 *
 * @sa BatchFile
 **/
class BatchLoadInfo
{
  public:
    BatchLoadInfo() noexcept = default;

    /**
     * @brief Initialises the batch load information with the given data.
     *
     * @param[in] headerFilename
     *   Load Header filename.
     * @param[in] partNumber
     *   Load Part Number.
     **/
    BatchLoadInfo(
      std::string_view headerFilename,
      std::string_view partNumber);

    //! @copydoc BatchLoadInfo::BatchLoadInfo(std::string_view,std::string_view)
    BatchLoadInfo(
      std::string &&headerFilename,
      std::string &&partNumber);

    /**
     * @brief Returns the load header filename.
     *
     * @return The Load Header filename.
     **/
    [[nodiscard]] std::string_view headerFilename() const;

    /**
     * @brief Sets the load header filename.
     *
     * @param[in] headerFilename
     *   The load header filename.
     **/
    void headerFilename( std::string_view headerFilename);

    //! @copydoc headerFilename(std::string_view)
    void headerFilename( std::string &&headerFilename);

    /**
     * @brief Returns the load part number.
     *
     * @return The load part number.
     **/
    [[nodiscard]] std::string_view partNumber() const;

    /**
     * @brief Updates the load part number.
     *
     * @param[in] partNumber
     *   The load part number.
     **/
    void partNumber( std::string_view partNumber);

    //! @copydoc partNumber(std::string_view)
    void partNumber( std::string &&partNumber);

  private:
    //! The load header filename.
    std::string headerFilenameV;
    //! The load part number.
    std::string partNumberV;
};

}

#endif

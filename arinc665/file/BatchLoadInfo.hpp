/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of class Arinc665::File::BatchLoadInfo.
 **/

#ifndef ARINC665_FILE_BATCHLOADINFO_HPP
#define ARINC665_FILE_BATCHLOADINFO_HPP

#include <arinc665/file/File.hpp>

#include <string>

namespace Arinc665 {
namespace File {

/**
 * @brief The load info the batch load entry.
 **/
class BatchLoadInfo
{
  public:
    /**
     * @brief Initialises the batch load information with the given data.
     *
     * @param[in] headerFilename
     *   Load header filename.
     * @param[in] partNumber
     *   Load part number.
     **/
    BatchLoadInfo(
      const std::string &headerFilename,
      const std::string &partNumber);

    //! @copydoc BatchLoadInfo::BatchLoadInfo(const std::string&,const std::string&)
    BatchLoadInfo(
      std::string &&headerFilename,
      std::string &&partNumber);

    /**
     * @brief Returns the load header filename.
     *
     * @return The load header filename.
     **/
    std::string headerFilename() const;

    /**
     * @brief Sets the load header filename.
     *
     * @param[in] headerFilename
     *   The load header filename.
     **/
    void headerFilename( const std::string &headerFilename);

    //! @copydoc headerFilename(const std::string&)
    void headerFilename( std::string &&headerFilename);

    /**
     * @brief Returns the load part number.
     *
     * @return The load part number.
     **/
    std::string partNumber() const;

    /**
     * @brief Updates the load part number.
     *
     * @param[in] partNumber
     *   The load part number.
     **/
    void partNumber( const std::string &partNumber);

    //! @copydoc partNumber(const std::string&)
    void partNumber( std::string &&partNumber);

  private:
    //! The load header filename.
    std::string headerFilenameV;
    //! The load part number.
    std::string partNumberV;
};

}
}

#endif

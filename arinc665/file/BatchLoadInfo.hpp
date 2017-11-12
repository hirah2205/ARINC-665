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
    //! String type
    using string = std::string;

    /**
     * @brief Initialises the batch load information with the given data.
     *
     * @param[in] headerFilename
     *   Load header filename.
     * @param[in] partNumber
     *   Load part number.
     **/
    BatchLoadInfo(
      const string &headerFilename,
      const string &partNumber);

    //! @copydoc BatchLoadInfo::BatchLoadInfo(const string&,const string&)
    BatchLoadInfo(
      string &&headerFilename,
      string &&partNumber);

    /**
     * @brief Returns the load header filename.
     *
     * @return The load header filename.
     **/
    string getHeaderFilename() const;

    /**
     * @brief Sets the load header filename.
     *
     * @param[in] headerFilename
     *   The load header filename.
     **/
    void setHeaderFilename( const string &headerFilename);

    /**
     * @brief Returns the load part number.
     *
     * @return The load part number.
     **/
    string getPartNumber() const;

    /**
     * @brief Updates the load part number.
     *
     * @param[in] partNumber
     *   The load part number.
     **/
    void setPartNumber( const string &partNumber);

  private:
    //! The load header filename.
    string headerFilename;
    //! The load part number.
    string partNumber;
};

}
}

#endif

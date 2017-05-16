/*
 * $Date$
 * $Revision$
 */
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
    using string = std::string;

    explicit BatchLoadInfo(
      const string &headerFilename = {},
      const string &partNumber = {});

    explicit BatchLoadInfo(
      string &&headerFilename = {},
      string &&partNumber = {});

    string getHeaderFilename() const;

    void setHeaderFilename( const string &headerFilename);

    string getPartNumber() const;

    void setPartNumber( const string &partNumber);

  private:
    string headerFilename;
    string partNumber;
};

}
}

#endif

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
 * Load Information stored within the target list of the ARINC 665 Batch File.
 *
 * @sa BatchFile
 **/
struct BatchLoadInfo
{
  public:
    //! Load header filename.
    std::string headerFilename;
    //! Load part number.
    std::string partNumber;
};

}

#endif

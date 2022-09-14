/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Files::MediaSetInformation.
 **/

#ifndef ARINC665_FILES_MEDIASETINFORMATION_HPP
#define ARINC665_FILES_MEDIASETINFORMATION_HPP

#include <arinc665/files/Files.hpp>

namespace Arinc665::Files {

struct MediaSetInformation
{
  //! Media Set Part Number
  std::string partNumber;
  //! Media Sequence Number
  uint8_t mediaSequenceNumber;
  //! Number of Media Set Members
  uint8_t numberOfMediaSetMembers;
};

}

#endif

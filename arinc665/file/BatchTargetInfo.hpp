/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::File::BatchTargetInfo.
 **/

#ifndef ARINC665_FILE_BATCHTARGETINFO_HPP
#define ARINC665_FILE_BATCHTARGETINFO_HPP

#include <arinc665/file/File.hpp>
#include <arinc665/file/BatchLoadInfo.hpp>

#include <string>

namespace Arinc665::File {

/**
 * @brief ARINC 665 Batch Target Information.
 *
 * The target hardware information, which is part of the batch file.
 *
 * Within ARINC 665-2 The Target Hardware ID Position Field is named
 * Target HW ID.
 *
 * @sa BatchListFile
 * @sa BatchLoadInfo, BatchLoadsInfo
 **/
struct BatchTargetInfo
{
  public:
  //! Target Hardware ID and Position
  std::string targetHardwareIdPosition;
  //! List of Loads for Batch
  BatchLoadsInfo loads;
};

}

#endif

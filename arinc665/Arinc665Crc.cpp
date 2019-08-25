/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of ARINC 665 CRCs.
 **/

#include "Arinc665Crc.hpp"

namespace Arinc665 {

const std::map< CheckValueType, uint16_t> CheckValueSize{
  { CheckValueType::NotUsed, 0},

  { CheckValueType::Crc8,    2},
  { CheckValueType::Crc16,   2},
  { CheckValueType::Crc32,   4},
  { CheckValueType::Md5,    16},
  { CheckValueType::Sha1,   20},
  { CheckValueType::Sha256, 32},
  { CheckValueType::Sha512, 64},
  { CheckValueType::Crc64,   8},

  { CheckValueType::Invalid, 0}
};

}

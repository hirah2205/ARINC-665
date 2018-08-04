/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of ARINC 665 CRCs.
 **/

#include "Arinc665Crc.hpp"

namespace Arinc665 {

const std::map< CheckValue, uint16_t> CheckValueSize{
  { CheckValue::NotUsed, 0},

  { CheckValue::Crc8,    2},
  { CheckValue::Crc16,   2},
  { CheckValue::Crc32,   4},
  { CheckValue::Md5,    16},
  { CheckValue::Sha1,   20},
  { CheckValue::Sha256, 32},
  { CheckValue::Sha512, 64},
  { CheckValue::Crc64,   8},

  { CheckValue::Invalid, 0}
};

}

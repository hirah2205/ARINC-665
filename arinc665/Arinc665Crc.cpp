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
  { CheckValueType::NotUsed, 0U},

  { CheckValueType::Crc8,    2U},
  { CheckValueType::Crc16,   2U},
  { CheckValueType::Crc32,   4U},
  { CheckValueType::Md5,    16U},
  { CheckValueType::Sha1,   20U},
  { CheckValueType::Sha256, 32U},
  { CheckValueType::Sha512, 64U},
  { CheckValueType::Crc64,   8U},

  { CheckValueType::Invalid, 0U}
};

}

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Declaration of ARINC 665 CRCs.
 **/

#ifndef ARINC665_ARINC665CRC_HPP
#define ARINC665_ARINC665CRC_HPP

#include <arinc665/Arinc665.hpp>

#include <map>

#include <boost/crc.hpp>

namespace Arinc665 {

//! ARINC 665 CRC 16 definition
using Arinc665Crc16 =
  boost::crc_optimal< 16, Crc16Polynom, Crc16Init, Crc16FinalXor, Crc16ReflectIn, Crc16ReflectOut>;

//! ARINC 665 CRC 32 definition
using Arinc665Crc32 =
  boost::crc_optimal< 32, Crc32Polynom, Crc32Init, Crc32FinalXor, Crc32ReflectIn, Crc32ReflectOut>;

//! ARINC 665 CRC 64 definition
using Arinc665Crc64 =
  boost::crc_optimal< 64, Crc64Polynom, Crc64Init, Crc64FinalXor, Crc64ReflectIn, Crc64ReflectOut>;

extern const std::map< CheckValue, uint16_t> CheckValueSize;

}

#endif


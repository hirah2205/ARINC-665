// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Logger Arinc665::Logger.
 **/

#ifndef ARINC_665_LOGGER_HPP
#define ARINC_665_LOGGER_HPP

#include <arinc_665/Arinc665.hpp>

#include <helper/Logger.hpp>

#include <boost/log/sources/global_logger_storage.hpp>

namespace Arinc665 {

//! ARINC 665 Logger.
BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(
  Logger,
  Helper::ChannelSeverityLoggerMt,
  ( boost::log::keywords::channel = "Arinc665" ) )

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Logger Arinc665Qt::Arinc665QtLogger.
 **/

#ifndef ARINC665_QT_ARINC665QTLOGGER_HPP
#define ARINC665_QT_ARINC665QTLOGGER_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <helper/Logger.hpp>

#include <boost/log/sources/global_logger_storage.hpp>

namespace Arinc665Qt {

BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(
  Arinc665QtLogger,
  Helper::ChannelSeverityLoggerMt,
  ( boost::log::keywords::channel = "Arinc665Qt" ) )

}

#endif

/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::SupportedArinc665VersionDescription.
 **/

#include "SupportedArinc665VersionDescription.hpp"

#include <boost/exception/exception.hpp>
#include <boost/program_options.hpp>

namespace Arinc665 {

SupportedArinc665VersionDescription::SupportedArinc665VersionDescription():
  Description{
    { "Supplement2",  SupportedArinc665Version::Supplement2},
    { "Supplement34", SupportedArinc665Version::Supplement34},

    { "Invalid",      SupportedArinc665Version::Invalid}
  }
{
}

std::ostream& operator<<(
  std::ostream &stream,
  const SupportedArinc665Version version)
{
  return (stream << SupportedArinc665VersionDescription::instance().name(
    version));
}

std::istream& operator>>(
  std::istream& stream,
  SupportedArinc665Version &version)
{
  std::string versionStr;

  // extract string from stream
  stream >> versionStr;

  // Decode
  version = SupportedArinc665VersionDescription::instance().enumeration(
    versionStr);

  if ( SupportedArinc665Version::Invalid == version)
  {
    BOOST_THROW_EXCEPTION(
      boost::program_options::invalid_option_value( versionStr));
  }

  return stream;
}

}

// SPDX-License-Identifier: MPL-2.0
/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::SupportedArinc665VersionDescription.
 **/

#include "SupportedArinc665VersionDescription.hpp"

#include <boost/exception/exception.hpp>
#include <boost/program_options.hpp>

#include <istream>
#include <ostream>

namespace Arinc665 {

SupportedArinc665VersionDescription::SupportedArinc665VersionDescription() :
  Description{
    { "Supplement2",   SupportedArinc665Version::Supplement2 },
    { "Supplement345", SupportedArinc665Version::Supplement345 }
  }
{
}

std::ostream& operator<<( std::ostream &stream, const SupportedArinc665Version version )
{
  return ( stream << SupportedArinc665VersionDescription::instance().name( version ) );
}

std::istream& operator>>( std::istream &stream, SupportedArinc665Version &version )
{
  std::string versionStr;

  // extract string from stream
  stream >> versionStr;

  // Decode
  const auto optionalVersion{ SupportedArinc665VersionDescription::instance().enumeration( versionStr ) };

  if ( !optionalVersion )
  {
    BOOST_THROW_EXCEPTION( boost::program_options::invalid_option_value( versionStr ) );
  }

  version = *optionalVersion;

  return stream;
}

}

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
 * @brief Definition of Class Arinc665::MediumNumber.
 **/

#include "MediumNumber.hpp"

#include <format>

namespace Arinc665 {

MediumNumber::MediumNumber( const uint8_t mediumNumber ) noexcept :
  mediumNumberV{ mediumNumber }
{
}

std::string MediumNumber::toString() const
{
  return std::format( "{:03d}", mediumNumberV );
}

MediumNumber MediumNumber::operator+( const uint8_t increment )
{
  mediumNumberV += increment;
  return *this;
}

MediumNumber MediumNumber::operator++()
{
  ++mediumNumberV;
  return *this;
}

MediumNumber MediumNumber::operator++( int )
{
  auto old{ *this };
  ++mediumNumberV;
  return old;
}

MediumNumber::operator uint8_t() const
{
  return mediumNumberV;
}

MediumNumber::operator std::string() const
{
  return toString();
}

MediumNumber& MediumNumber::operator =( const uint8_t mediumNumber )
{
  mediumNumberV = mediumNumber;
  return *this;
}

std::ostream& operator <<(
  std::ostream &stream,
  const MediumNumber &mediumNumber )
{
  return stream << mediumNumber.toString();
}

}

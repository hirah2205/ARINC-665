/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, Thomas@Thomas-Vogt.de
 *
 * @brief Definition of class Arinc665::Media::Medium.
 **/

#include "Medium.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <cassert>

namespace Arinc665 {
namespace Media {

Medium::Medium( MediaSetPtr mediaSet, const uint8_t mediumNumber):
  mediaSetValue( mediaSet),
  mediumNumberValue( mediumNumber)
{
  assert( mediaSet && (mediumNumber > 0));
}

ConstMediaSetPtr Medium::mediaSet() const
{
  return mediaSetValue.lock();
}

MediaSetPtr Medium::mediaSet()
{
  return mediaSetValue.lock();
}

Medium::Type Medium::type() const
{
  return Type::Medium;
}

const Medium::string& Medium::name() const
{
  return mediaSetValue.lock()->name();
}

Medium::string Medium::partNumber() const
{
  return mediaSetValue.lock()->partNumber();
}

Medium::fpath Medium::path() const
{
  return "/";
}

uint8_t Medium::mediumNumber() const
{
  return mediumNumberValue;
}

}
}

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
  mediaSet( mediaSet),
  mediumNumber( mediumNumber)
{
  assert( mediaSet && (mediumNumber > 0));
}

ConstMediaSetPtr Medium::getMediaSet() const
{
  return mediaSet.lock();
}

MediaSetPtr Medium::getMediaSet()
{
  return mediaSet.lock();
}

Medium::Type Medium::getType() const
{
  return Type::Medium;
}

const Medium::string& Medium::getName() const
{
  return mediaSet.lock()->getName();
}

Medium::string Medium::getPartNumber() const
{
  return mediaSet.lock()->getPartNumber();
}

Medium::path Medium::getPath() const
{
  return "/";
}

uint8_t Medium::getMediumNumber() const
{
  return mediumNumber;
}

}
}

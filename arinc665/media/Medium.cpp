/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Definition of Class Arinc665::Media::Medium.
 **/

#include "Medium.hpp"

#include <arinc665/media/Directory.hpp>
#include <arinc665/media/MediaSet.hpp>

#include <cassert>

namespace Arinc665::Media {

Medium::Medium( MediaSetPtr mediaSet, const uint8_t mediumNumber):
  mediaSetV{ mediaSet},
  mediumNumberV{ mediumNumber}
{
  assert( mediaSet && (mediumNumber > 0));
}

ConstMediaSetPtr Medium::mediaSet() const
{
  return mediaSetV.lock();
}

MediaSetPtr Medium::mediaSet()
{
  return mediaSetV.lock();
}

Medium::Type Medium::type() const
{
  return Type::Medium;
}

std::filesystem::path Medium::path() const
{
  return std::filesystem::path{ { std::filesystem::path::preferred_separator } };
}

ConstContainerEntityPtr Medium::parent() const
{
  return {};
}

ContainerEntityPtr Medium::parent()
{
  return {};
}

ConstMediumPtr Medium::medium() const
{
  return std::dynamic_pointer_cast< const Medium>( shared_from_this());
}

MediumPtr Medium::medium()
{
  return std::dynamic_pointer_cast< Medium>( shared_from_this());
}

uint8_t Medium::mediumNumber() const
{
  return mediumNumberV;
}

}

/*
 * $Date$
 * $Revision$
 */
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

namespace Arinc665 {
namespace Media {

Medium::Medium( MediaSetPtr mediaSet):
  mediaSet( mediaSet)
{
}

ConstMediaSetPtr Medium::getMediaSet( void) const
{
  return mediaSet.lock();
}

MediaSetPtr Medium::getMediaSet( void)
{
  return mediaSet.lock();
}

}
}

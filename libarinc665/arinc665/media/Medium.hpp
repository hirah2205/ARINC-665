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
 * @brief Declaration of class Arinc665::Media::Medium.
 **/

#ifndef ARINC665_MEDIA_MEDIUM_HPP
#define ARINC665_MEDIA_MEDIUM_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/ContainerEntity.hpp>

namespace Arinc665 {
namespace Media {

/**
 * @brief Represents a media within an ARINC 665 media set
 **/
class Medium : public ContainerEntity
{
  public:
    Medium( MediaSetPtr mediaSet);

    ConstMediaSetPtr getMediaSet( void) const;

    MediaSetPtr getMediaSet( void);

  private:
    // Pointer to media set
    WeakMediaSetPtr mediaSet;
};

}
}

#endif

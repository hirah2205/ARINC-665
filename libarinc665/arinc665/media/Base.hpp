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
 * @brief Declaration of class Arinc665::Media::Base.
 **/

#ifndef ARINC665_MEDIA_BASE_HPP
#define ARINC665_MEDIA_BASE_HPP

#include <arinc665/media/Media.hpp>

namespace Arinc665 {
namespace Media {

class Base
{
  public:
    enum class Type
    {
      MediaSet,
      Medium,
      Directory,
      File
    };

    virtual ~Base( void) noexcept = default;

    virtual ConstMediaSetPtr getMediaSet( void) const = 0;

    virtual MediaSetPtr getMediaSet( void) = 0;

    virtual Type getType( void) const = 0;
};

}
}

#endif

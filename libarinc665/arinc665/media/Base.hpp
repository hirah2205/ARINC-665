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

#include <string>

namespace Arinc665 {
namespace Media {

class Base
{
  public:
    using string = std::string;

    //! item type
    enum class Type
    {
      MediaSet,
      Medium,
      Directory,
      File
    };

    virtual ~Base( void) noexcept = default;

    /**
     * @brief Returns the owning media set.
     *
     * @return The owning media set
     **/
    virtual ConstMediaSetPtr getMediaSet( void) const = 0;

    /**
     * @brief Returns the owning media set.
     *
     * @return The owning media set
     **/
    virtual MediaSetPtr getMediaSet( void) = 0;

    /**
     * @brief Returns the item type.
     *
     * @return The item type
     **/
    virtual Type getType( void) const = 0;

    /**
    * @brief Returns the name of the file.
    *
    * @return The name of the file.
    **/
   virtual const string& getName( void) const = 0;

   /**
    * @brief Return the part number of the entity.
    *
    * If the entity itself does not have an part number (e.i. media and
    * directories) the parent part number is returned
    *
    * @return The part number of the entity
    **/
   virtual const string& getPartNumber( void) const = 0;
};

}
}

#endif

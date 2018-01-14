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

/**
 * @brief %Base class for media set classes.
 *
 * Provides common attributes.
 */
class Base
{
  public:
    //! String type
    using string = std::string;

    //! item type
    enum class Type
    {
      MediaSet,
      Medium,
      Directory,
      File
    };

    //! Default destructor
    virtual ~Base() noexcept = default;

    /**
     * @brief Returns the owning media set.
     *
     * @return The owning media set
     **/
    virtual ConstMediaSetPtr mediaSet() const = 0;

    //! @copydoc getMediaSet() const
    virtual MediaSetPtr mediaSet() = 0;

    /**
     * @brief Returns the item type.
     *
     * @return The item type
     **/
    virtual Type type() const = 0;

    /**
    * @brief Returns the name of the file.
    *
    * @return The name of the file.
    **/
   virtual const string& name() const = 0;

   /**
    * @brief Return the part number of the entity.
    *
    * If the entity itself does not have an part number (e.i. media and
    * directories) the parent part number is returned
    *
    * @return The part number of the entity
    **/
   virtual string partNumber() const = 0;
};

}
}

#endif

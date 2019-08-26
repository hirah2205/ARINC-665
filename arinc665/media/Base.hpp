/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::Base.
 **/

#ifndef ARINC665_MEDIA_BASE_HPP
#define ARINC665_MEDIA_BASE_HPP

#include <arinc665/media/Media.hpp>

#include <string>
#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Base class for media set classes.
 *
 * Provides common attributes.
 */
class Base
{
  public:
    //! item type
    enum class Type
    {
      MediaSet,
      Medium,
      Directory,
      File
    };

    //! Destructor
    virtual ~Base() noexcept = default;

    /**
     * @brief Returns the owning media set.
     *
     * @return The owning media set
     **/
    [[nodiscard]] virtual ConstMediaSetPtr mediaSet() const = 0;

    //! @copydoc mediaSet() const
    virtual MediaSetPtr mediaSet() = 0;

    /**
     * @brief Returns the item type.
     *
     * @return The item type
     **/
    [[nodiscard]] virtual Type type() const = 0;

    /**
     * @brief Return the part number of the entity.
     *
     * If the entity itself does not have an part number (e.i. media and
     * directories) the parent part number is returned
     *
     * @return The part number of the entity
     **/
    [[nodiscard]] virtual std::string_view partNumber() const = 0;
};

}

#endif

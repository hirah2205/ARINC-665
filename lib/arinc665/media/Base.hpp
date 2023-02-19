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

#include <filesystem>
#include <string>
#include <string_view>

namespace Arinc665::Media {

/**
 * @brief %Media Set Item.
 *
 * %Base class for media set classes.
 * Provides common attributes.
 */
class Base : public std::enable_shared_from_this< Base >
{
  public:
    //! Default Constructor
    Base() noexcept = default;

    //! Copy Constructor (deleted)
    Base( const Base& ) = delete;
    //! Move Constructor (deleted)
    Base( Base&& ) = delete;
    //! Copy Assignment Operator (deleted)
    Base& operator =( const Base& ) = delete;
    //! Move Assignment Operator (deleted)
    Base& operator =( Base&& ) = delete;

    //! Destructor
    virtual ~Base() noexcept = default;

    /**
     * @brief Returns the Owning %Media Set.
     *
     * @return Owning %Media Set
     **/
    [[nodiscard]] virtual ConstMediaSetPtr mediaSet() const = 0;

    //! @copydoc mediaSet() const
    [[nodiscard]] virtual MediaSetPtr mediaSet() = 0;

    /**
     * @brief Returns the Item Type.
     *
     * @return Item Type
     **/
    [[nodiscard]] virtual Type type() const = 0;

    /**
     * @name Parent
     * @{
     **/

    /**
     * @brief Returns the parent.
     *
     * @return Parent of this container.
     **/
    [[nodiscard]] virtual ConstContainerEntityPtr parent() const = 0;

    //! @copydoc parent() const
    [[nodiscard]] virtual ContainerEntityPtr parent() = 0;

    /** @} **/

    /**
     * @brief Returns the path up to the media set root.
     *
     * @return The path up to the media set root.
     **/
    [[nodiscard]] virtual std::filesystem::path path() const = 0;
};

}

#endif

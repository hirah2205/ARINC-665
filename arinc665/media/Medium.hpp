/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt, thomas@thomas-vogt.de
 *
 * @brief Declaration of Class Arinc665::Media::Medium.
 **/

#ifndef ARINC665_MEDIA_MEDIUM_HPP
#define ARINC665_MEDIA_MEDIUM_HPP

#include <arinc665/media/Media.hpp>
#include <arinc665/media/ContainerEntity.hpp>

namespace Arinc665::Media {

/**
 * @brief %Medium within an ARINC 665 %Media Set.
 **/
class ARINC665_EXPORT Medium : public ContainerEntity
{
  public:
    /**
     * @brief Creates the medium.
     *
     * @param[in] mediaSet
     *   The owning the media set
     * @param[in] mediumNumber
     *   The medium number.
     **/
    Medium( MediaSetPtr mediaSet, uint8_t mediumNumber );

    //! @copydoc ContainerEntity::mediaSet
    ConstMediaSetPtr mediaSet() const final;

    //! @copydoc ContainerEntity::mediaSet
    MediaSetPtr mediaSet() final;

    /**
     * @copydoc ContainerEntity::type
     *
     * @retval Type::Medium
     *   Always
     **/
    Type type() const final;

    //! @copydoc ContainerEntity::path() const
    std::filesystem::path path() const final;

    /**
     * @copydoc ContainerEntity::parent() const
     * @retval {} always.
     **/
    ConstContainerEntityPtr parent() const final;

    //! @copydoc ContainerEntity::parent() const
    ContainerEntityPtr parent() final;

    //! @copydoc ContainerEntity::medium() const
    ConstMediumPtr medium() const final;

    //! @copydoc ContainerEntity::medium()
    MediumPtr medium() final;

    /**
     * @brief Returns the medium number.
     *
     * @return The medium number
     **/
    uint8_t mediumNumber() const;

  private:
    //! Pointer to Media Set
    MediaSetPtr::weak_type mediaSetV;
    //! Medium number
    const uint8_t mediumNumberV;
};

}

#endif

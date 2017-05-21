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
    /**
     * @brief Creates the medium.
     *
     * @param[in] mediaSet
     *   The owning the media set
     * @param[in] mediumNumber
     *   The medium number.
     **/
    Medium( MediaSetPtr mediaSet, uint8_t mediumNumber);

    //! @copydoc ContainerEntity::getMediaSet
    virtual ConstMediaSetPtr getMediaSet() const override final;

    //! @copydoc ContainerEntity::getMediaSet
    virtual MediaSetPtr getMediaSet() override final;

    /**
     * @copydoc ContainerEntity::getType
     *
     * @retval Type::Medium
     *   Always
     **/
    virtual Type getType() const override final;

    /**
     * @copydoc ContainerEntity::getName
     * @return The name of the owning media set.
     **/
    virtual const string& getName() const override final;

    /**
     * @copydoc ContainerEntity::getPartNumber
     *
     * @return The part number of the owning media set.
     **/
    virtual string getPartNumber() const override final;

    //! @copydoc ContainerEntity::getPath() const
    virtual path getPath() const override final;

    /**
     * @brief Returns the medium number.
     *
     * @return The medium number
     **/
    uint8_t getMediumNumber() const;

  private:
    //! Pointer to media set
    WeakMediaSetPtr mediaSet;
    //! The medium number
    const uint8_t mediumNumber;
};

}
}

#endif
